// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
 * bit7z - A C++ static library to interface with the 7-zip DLLs.
 * Copyright (c) 2014-2019  Riccardo Ostani - All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * Bit7z is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bit7z; if not, see https://www.gnu.org/licenses/.
 */

#include "../include/opencallback.hpp"

#include "../include/bitpropvariant.hpp"
#include "../include/cfileinstream.hpp"
#include "../include/fsutil.hpp"
#include "../include/util.hpp"

using namespace bit7z;
using namespace bit7z::filesystem;

/* Most of this code is taken from the COpenCallback class in Client7z.cpp of the 7z SDK
 * Main changes made:
 *  + Use of wstring instead of UString (see Callback base interface)
 *  + Error messages are not showed (see comments in ExtractCallback) */

OpenCallback::OpenCallback( const BitArchiveHandler& handler, const tstring& filename )
    : Callback( handler ), mSubArchiveMode( false ), mSubArchiveName(), mFileItem( filename ) {}

STDMETHODIMP OpenCallback::SetTotal( const UInt64* /* files */, const UInt64* /* bytes */ ) {
    return S_OK;
}

STDMETHODIMP OpenCallback::SetCompleted( const UInt64* /* files */, const UInt64* /* bytes */ ) {
    return S_OK;
}

STDMETHODIMP OpenCallback::GetProperty( PROPID propID, PROPVARIANT* value ) {
    BitPropVariant prop;
    if ( mSubArchiveMode ) {
        switch ( propID ) {
            case kpidName:
                prop = fs::path( mSubArchiveName ).wstring();
                break;
                // case kpidSize:  prop = _subArchiveSize; break; // we don't use it now
        }
    } else {
        switch ( propID ) {
            case kpidName:
                prop = fs::path( mFileItem.name() ).wstring();
                break;
            case kpidIsDir:
                prop = mFileItem.isDir();
                break;
            case kpidSize:
                prop = mFileItem.size();
                break;
            case kpidAttrib:
                prop = mFileItem.attributes();
                break;
            case kpidCTime:
                prop = mFileItem.creationTime();
                break;
            case kpidATime:
                prop = mFileItem.lastAccessTime();
                break;
            case kpidMTime:
                prop = mFileItem.lastWriteTime();
                break;
        }
    }
    *value = prop;
    prop.bstrVal = nullptr;
    return S_OK;
}

STDMETHODIMP OpenCallback::GetStream( const wchar_t* name, IInStream** inStream ) {
    try {
        *inStream = nullptr;
        if ( mSubArchiveMode ) {
            return S_FALSE;
        }
        if ( mFileItem.isDir() ) {
            return S_FALSE;
        }
        auto stream_path = mFileItem.path();
        if ( name != nullptr ) {
            stream_path = stream_path.parent_path();
            stream_path.append(name); 
            auto stream_status = fs::status( stream_path );
            if ( !fs::exists( stream_status ) || fs::is_directory( stream_status ) ) {  // avoid exceptions using status
                return S_FALSE;
            }
        }
        CMyComPtr< CFileInStream > inStreamTemp = new CFileInStream( stream_path );
        if ( inStreamTemp->fail() ) {
            return HRESULT_FROM_WIN32( ERROR_OPEN_FAILED );
        }
        *inStream = inStreamTemp.Detach();
        return S_OK;
    } catch ( ... ) {
        return E_OUTOFMEMORY;
    }
}

STDMETHODIMP OpenCallback::SetSubArchiveName( const wchar_t* name ) {
    mSubArchiveMode = true;
#ifdef _WIN32
    mSubArchiveName = name;
#else
    mSubArchiveName = fs::path( name ).string();
#endif
    return S_OK;
}

STDMETHODIMP OpenCallback::CryptoGetTextPassword( BSTR* password ) {
    wstring pass;
    if ( !mHandler.isPasswordDefined() ) {
        // You can ask real password here from user
        // Password = GetPassword(OutStream);
        // PasswordIsDefined = true;
        if ( mHandler.passwordCallback() ) {
            pass = WIDEN( mHandler.passwordCallback()() );
        }

        if ( pass.empty() ) {
            mErrorMessage = "Password is not defined";
            return E_ABORT;
        }
    } else {
        pass = WIDEN( mHandler.password() );
    }

    return StringToBstr( pass.c_str(), password );
}
