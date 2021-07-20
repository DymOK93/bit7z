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

#ifndef FILEEXTRACTCALLBACK_HPP
#define FILEEXTRACTCALLBACK_HPP

#include <string>

#include "bitguids.hpp"
#include "cfileoutstream.hpp"
#include "extractcallback.hpp"

namespace bit7z {
    using std::wstring;

    class FileExtractCallback : public ExtractCallback {
        public:
            FileExtractCallback( const BitInputArchive& inputArchive,
                                 const tstring& directoryPath );

            ~FileExtractCallback() override = default;

            void throwException( HRESULT error ) override;

        private:
            fs::path mInFilePath;     // Input file path
            fs::path mDirectoryPath;  // Output directory
            fs::path mDiskFilePath;   // full path to file on disk
            bool mRetainDirectories;

            struct CProcessedFileInfo {
                FILETIME MTime;
                UInt32 Attrib;
                bool isDir;
                bool AttribDefined;
                bool MTimeDefined;
            } mProcessedFileInfo;

            CMyComPtr< CFileOutStream > mFileOutStream;

            void finishOperation() override;

            void releaseStream() override;

            HRESULT getOutStream( uint32_t index, ISequentialOutStream** outStream, int32_t askExtractMode ) override;
    };
}
#endif // FILEEXTRACTCALLBACK_HPP
