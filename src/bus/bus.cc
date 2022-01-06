/*
 * This file is part of the NES-6502 distribution (https://github.com/temaweb/NES-6502).
 * Copyright (c) 2021 Artem Okonechnikov.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "bus.h"

#include "fmt/core.h"
#include "fmt/color.h"

uint8_t Bus::read (uint16_t index) const 
{
    return ram[index];
}

void Bus::write (uint16_t index, uint8_t data) 
{
    ram[index] = data;
}

/*
    Print memory dump from custom range
    Default: 0x00 - 0xFF
*/
void Bus::printDump(uint16_t from, uint16_t to) const
{  
    // Iterate memory from closest zero-nibble to 
    // last nibble supplimented to F
    for (uint16_t x = from & ~0xF; x <= (to | 0xF); x++)
    {
        // Print row label for each zero-nibble
        if ((x & 0xF) == 0x00) {
            fmt::print(fg(fmt::color::gray), "\n{:04X}: ", x);
        }

        if (x < from || x > to) 
        {
            // Print empty space if value is out of range
            fmt::print("{:^3}", "");
        } 
        else 
        {
            auto byte = read(x);
            fmt::print(fg(fmt::color::dark_gray), "{:02X} ", byte);
        }
    }
}