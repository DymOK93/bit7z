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

#ifndef CALLBACK_HPP
#define CALLBACK_HPP

#include <string>

#include <Common/MyCom.h>

#include "../include/bitarchivehandler.hpp"

CONSTEXPR auto kUnsupportedMethod = "Unsupported Method";
CONSTEXPR auto kCRCFailed         = "CRC Failed";
CONSTEXPR auto kDataError         = "Data Error";
CONSTEXPR auto kUnknownError      = "Unknown Error";
CONSTEXPR auto kEmptyFileAlias    = TSTRING("[Content]");

namespace bit7z {
    using std::wstring;

    class Callback : protected CMyUnknownImp {
        public:
            virtual ~Callback() = default;
            virtual std::string getErrorMessage() const;

        protected:
            explicit Callback( const BitArchiveHandler& handler ); // Protected constructor => Abstract class

            const BitArchiveHandler& mHandler;
            std::string mErrorMessage;

    };
}
#endif // CALLBACK_HPP
