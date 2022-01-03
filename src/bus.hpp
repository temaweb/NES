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

#ifndef BUS_HPP
#define BUS_HPP

#include <array>
#include <cstdint>

class Bus
{
private:

    // Temporary 64KB RAM
    std::array<uint8_t, 64 * 1024> ram {};

public:
    // Read byte on address
    uint8_t read (uint16_t index) const;
    
    // Write byte on address
    void write (uint16_t index, uint8_t data);
};

#endif