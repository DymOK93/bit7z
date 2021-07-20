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

#include "bitguids.hpp"

namespace bit7z {
    // GUIDs of Interfaces
#ifndef _MSVC_VER
    const GUID IID_IUnknown = {0x00000000, 0x0000, 0x0000, {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};
#endif

    // IStream.h
    const GUID IID_ISequentialInStream  = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00}};
    const GUID IID_ISequentialOutStream = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00}};
    const GUID IID_IInStream            = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00}};
    const GUID IID_IOutStream           = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00}};
    const GUID IID_IStreamGetSize       = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00}};
    const GUID IID_IStreamGetProps      = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x03, 0x00, 0x08, 0x00, 0x00}};
    const GUID IID_IStreamGetProps2     = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00}};

    // ICoder.h
    const GUID IID_ICompressProgressInfo = {
        0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00}
    };

    // IPassword.h
    const GUID IID_ICryptoGetTextPassword = {
        0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00}
    };
    const GUID IID_ICryptoGetTextPassword2 = {
        0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x05, 0x00, 0x11, 0x00, 0x00}
    };

    // IArchive.h
    const GUID IID_ISetProperties      = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x00, 0x00}};
    const GUID IID_IInArchive          = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0x60, 0x00, 0x00}};
    const GUID IID_IOutArchive         = {0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0xA0, 0x00, 0x00}};
    const GUID IID_IArchiveExtractCallback = {
        0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0x20, 0x00, 0x00}
    };
    const GUID IID_IArchiveOpenVolumeCallback = {
        0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0x30, 0x00, 0x00}
    };
    const GUID IID_IArchiveOpenSetSubArchiveName = {
        0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0x50, 0x00, 0x00}
    };
    const GUID IID_IArchiveUpdateCallback = {
        0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0x00, 0x00}
    };
    const GUID IID_IArchiveUpdateCallback2 = {
        0x23170F69, 0x40C1, 0x278A, {0x00, 0x00, 0x00, 0x06, 0x00, 0x82, 0x00, 0x00}
    };
}
