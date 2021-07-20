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

#ifndef CSTDOUTSTREAM_HPP
#define CSTDOUTSTREAM_HPP

#include <ostream>
#include <cstdint>

#include <7zip/IStream.h>
#include <Common/MyCom.h>

namespace bit7z {
    using std::ostream;

    class CStdOutStream : public IOutStream, public CMyUnknownImp {
        public:
            explicit CStdOutStream( std::ostream& outputStream );

            virtual ~CStdOutStream() = default;

            MY_UNKNOWN_IMP1( IOutStream )

            // IOutStream
            STDMETHOD( Write )( void const* data, UInt32 size, UInt32* processedSize );

            STDMETHOD( Seek )( Int64 offset, UInt32 seekOrigin, UInt64* newPosition );

            STDMETHOD( SetSize )( UInt64 newSize );

        private:
            ostream& mOutputStream;
    };
}

#endif // CSTDOUTSTREAM_HPP
