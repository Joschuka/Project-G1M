#pragma once

#ifndef G1TL_H
#define G1TL_H

///////////////////////////////
//     G1T PALETTE FILES     //
///////////////////////////////

enum class tplPaletteFormats_e : int {
    NONE = -1,
    PIA8 = 0,
    PRGB565,
    PRGB5A3
};

struct S_G1TL_ENTRY
{
    uint8_t PACKED_SIZE;
    uint8_t KTGL_PIXEL_FORMAT;
    BYTE* PALETTE;
    uint32_t SIZE;
    bool bPanic = false;
    tplPaletteFormats_e WiiPALETTE_TYPE = tplPaletteFormats_e::NONE;

    S_G1TL_ENTRY(BYTE* buffer, uint32_t& offset, int bufferLen, uint32_t& dataStart)
    {
        PACKED_SIZE = (*(uint8_t*)(buffer + offset)); offset += 1;
        KTGL_PIXEL_FORMAT = (*(uint8_t*)(buffer + offset)); offset += 1;
        PALETTE = buffer + dataStart;
        SIZE = (1 << PACKED_SIZE) * 2;
        WiiPALETTE_TYPE = tplPaletteFormats_e::PIA8;
        switch (KTGL_PIXEL_FORMAT)
        {
        case 0x1C:
            WiiPALETTE_TYPE = tplPaletteFormats_e::PRGB565;
            break;
        case 0x25:
            WiiPALETTE_TYPE = tplPaletteFormats_e::PRGB5A3;
            break;
        default:
            break;
        }

        switch (SIZE)
        {
        case 32768:
        case 512:
        case 32:
            break;
        default:
            bPanic = true;
            return;
            break;
        }

        if ((dataStart + (int)SIZE) > bufferLen)
        {
            bPanic = true;
            return;
        }
        PALETTE = buffer + dataStart;
        dataStart += SIZE;

    }
};


template <bool bBigEndian>
struct S_G1TL_HEADER
{
    char MAGIC[4];
    uint32_t VERSION = 0;
    uint32_t VERSION_NUMBER = 0;
    uint32_t HEADER_SIZE = 16;
    uint16_t ENTRY_COUNT = 1;
    uint16_t FLAG; // must be 3
    std::vector<S_G1TL_ENTRY> ENTRIES;
    bool bPanic = false;

    S_G1TL_HEADER(BYTE* buffer, uint32_t& offset, int bufferLen)
    {
        // panic check
        if (HEADER_SIZE > (uint32_t)bufferLen)
        {
            bPanic = true;
            return;
        }

        MAGIC[0] = (*(char*)(buffer + offset)); offset += 1;
        MAGIC[1] = (*(char*)(buffer + offset)); offset += 1;
        MAGIC[2] = (*(char*)(buffer + offset)); offset += 1;
        MAGIC[3] = (*(char*)(buffer + offset)); offset += 1;

        const char EXPECTED_BIG_MAGIC[4] = { 'G', '1', 'T', 'L' };
        const char EXPECTED_LITTLE_MAGIC[4] = { 'L', 'T', '1', 'G' };

        // panic check
        if (!(memcmp(MAGIC, EXPECTED_BIG_MAGIC, 4) == 0 ||
            memcmp(MAGIC, EXPECTED_LITTLE_MAGIC, 4) == 0)
            )
        {
            bPanic = true;
            return;
        }

        VERSION = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(VERSION);

        // get the version number (should be 0)
        VERSION_NUMBER =
            (((VERSION >> 24) & 0xFF) - 0x30) * 1000 +
            (((VERSION >> 16) & 0xFF) - 0x30) * 100 +
            (((VERSION >> 8) & 0xFF) - 0x30) * 10 +
            (VERSION & 0xFF) - 0x30;

        HEADER_SIZE = (*(uint32_t*)(buffer + offset)); offset += 4; if (bBigEndian) LITTLE_BIG_SWAP(HEADER_SIZE);

        // panic check
        if (HEADER_SIZE > (uint32_t)bufferLen)
        {
            bPanic = true;
            return;
        }

        if (HEADER_SIZE != 16)
        {
            bPanic = true;
            return;
        }

        ENTRY_COUNT = (*(uint16_t*)(buffer + offset)); offset += 2; if (bBigEndian) LITTLE_BIG_SWAP(ENTRY_COUNT);

        FLAG = (*(uint16_t*)(buffer + offset)); offset += 2; if (bBigEndian) LITTLE_BIG_SWAP(FLAG);

        // panic check
        if (FLAG != 3)
        {
            bPanic = true;
            return;
        }

        uint32_t dataStart = offset + (ENTRY_COUNT * 2);

        for (size_t i = 0; i < ENTRY_COUNT; i++)
        {
            // panic check
            if ((offset + 2) > (uint32_t)bufferLen)
            {
                bPanic = true;
                return;
            }
            S_G1TL_ENTRY ENTRY = S_G1TL_ENTRY(buffer, offset, bufferLen, dataStart);
            if (ENTRY.bPanic)
            {
                bPanic = true;
                return;
            }
            ENTRIES.push_back(ENTRY);
        }
    }
};

#endif // !G1T_H