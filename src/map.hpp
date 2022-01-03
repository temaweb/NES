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

#ifndef MAP_HPP
#define MAP_HPP

#include <array>
#include "cmd.hpp"

//
// Command mapping
//

class Map
{
private:

    //
    // 6502 Instruction set
    // Includes all common/undocumented instructions
    //

    std::array<Cmd, 256> cmd;

public:

    Map();

    // Returns command by operation code
    const Cmd & getCommand(uint8_t opcode) const;
};

#endif