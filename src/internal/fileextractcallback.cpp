// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
 * bit7z - A C++ static library to interface with the 7-zip DLLs.
 * Copyright (c) 2014-2021  Riccardo Ostani - All Rights Reserved.
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

#include "internal/fileextractcallback.hpp"

#include "bitexception.hpp"
#include "internal/fsutil.hpp"

using namespace std;
using namespace NWindows;
using namespace bit7z;

constexpr auto kCannotOpenOutput   = "Cannot open output file";
constexpr auto kCannotDeleteOutput = "Cannot delete output file";

/* Most of this code, though heavily modified, is taken from the CExtractCallback class in Client7z.cpp of the 7z SDK
 * Main changes made:
 *  + Use of wstring instead of UString
 *  + Error messages are not showed. Instead, they are memorized into a wstring and used by BitFileExtractor to throw
 *    exceptions (see also Callback interface). Note that this class doesn't throw exceptions, as other classes in bit7,
 *    because it must implement interfaces with nothrow methods.
 *  + The work performed originally by the Init method is now performed by the class constructor */

FileExtractCallback::FileExtractCallback( const BitInputArchive& inputArchive,
                                          const tstring& directoryPath )
    : ExtractCallback( inputArchive ),
      mInFilePath( inputArchive.archivePath() ),
      mDirectoryPath( directoryPath ),
      mRetainDirectories( inputArchive.handler().retainDirectories() ),
      mProcessedFileInfo() {}

void FileExtractCallback::releaseStream() {
    mFileOutStream.Release(); // We need to release the file to change its modified time!
}

void FileExtractCallback::finishOperation() {
    if ( mFileOutStream != nullptr ) {
        if ( mFileOutStream->fail() ) {
            ++mNumErrors; // SetOperationResult, which called releaseStream(), will return E_FAIL since mNumErrors > 0.
            return;
        }
        mFileOutStream.Release(); // We need to release the file to change its modified time!

        if ( mProcessedFileInfo.MTimeDefined ) {
            filesystem::fsutil::setFileModifiedTime( mDiskFilePath, mProcessedFileInfo.MTime );
        }
    }

    if ( mExtractMode && mProcessedFileInfo.AttribDefined ) {
        filesystem::fsutil::setFileAttributes( mDiskFilePath, mProcessedFileInfo.Attrib );
    }
}

void FileExtractCallback::throwException( HRESULT error ) {
    if ( mErrorMessage != nullptr ) {
        throw BitException( mErrorMessage, make_hresult_code( error ), mDiskFilePath.native() );
    }
    Callback::throwException( error );
}

HRESULT FileExtractCallback::getOutStream( uint32_t index, ISequentialOutStream** outStream, Int32 askExtractMode ) {
    // Get Name
    BitPropVariant prop = mInputArchive.itemProperty( index, BitProperty::Path );

    fs::path filePath;
    if ( prop.isEmpty() ) {
        filePath = !mInFilePath.empty() ? mInFilePath.stem() : fs::path( kEmptyFileAlias );
    } else if ( prop.isString() ) {
        filePath = fs::path( prop.getString() );
        if ( !mRetainDirectories ) {
            filePath = filePath.filename();
        }
    } else {
        return E_FAIL;
    }
    mDiskFilePath = mDirectoryPath / filePath;

    if ( askExtractMode != NArchive::NExtract::NAskMode::kExtract ) {
        return S_OK;
    }

    // Get Attrib
    BitPropVariant prop2 = mInputArchive.itemProperty( index, BitProperty::Attrib );

    if ( prop2.isEmpty() ) {
        mProcessedFileInfo.Attrib = 0;
        mProcessedFileInfo.AttribDefined = false;
    } else {
        if ( !prop2.isUInt32() ) {
            return E_FAIL;
        }

        mProcessedFileInfo.Attrib = prop2.getUInt32();
        mProcessedFileInfo.AttribDefined = true;
    }

    mProcessedFileInfo.isDir = mInputArchive.isItemFolder( index );

    // Get Modified Time
    BitPropVariant prop3 = mInputArchive.itemProperty( index, BitProperty::MTime );
    mProcessedFileInfo.MTimeDefined = false;

    switch ( prop3.type() ) {
        case BitPropVariantType::Empty:
            break;

        case BitPropVariantType::FileTime:
            mProcessedFileInfo.MTime = prop3.getFileTime();
            mProcessedFileInfo.MTimeDefined = true;
            break;

        default:
            return E_FAIL;
    }

    if ( !mProcessedFileInfo.isDir ) { // File
        if ( mHandler.fileCallback() ) {
            mHandler.fileCallback()( filePath );
        }

        std::error_code ec;
        fs::create_directories( mDiskFilePath.parent_path(), ec );

        if ( fs::exists( mDiskFilePath, ec ) && !fs::remove( mDiskFilePath, ec ) ) {
            mErrorMessage = kCannotDeleteOutput;
            return E_ABORT;
        }

        auto outStreamLoc = bit7z::make_com< CFileOutStream >( mDiskFilePath, true );
        if ( outStreamLoc->fail() ) {
            mErrorMessage = kCannotOpenOutput;
            return E_ABORT;
        }

        mFileOutStream = outStreamLoc;
        *outStream = outStreamLoc.Detach();
    } else if ( mRetainDirectories ) { // Directory, and we must retain it
        error_code ec;
        fs::create_directories( mDiskFilePath, ec );
    }
    return S_OK;
}
