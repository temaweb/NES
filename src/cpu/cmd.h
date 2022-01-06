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

#ifndef CMD_HPP
#define CMD_HPP

#include <cstdint>
#include <string>

#include "cpu.h"

struct Cmd
{
    /*
        Command name (BRK, ORA etc)
    */
    std::string name;

    /*
        Command length with arguments
    */
    uint8_t bytes;

    /*
        Programm cycles need to execute command
    */
    uint8_t cycles;

    /*
        Memory mode & command
    */
    void (Cpu::*code) (void);
    void (Cpu::*mode) (void);


    /*
        Execute command
    */
    uint8_t execute(Cpu * cpu)
    {
        // Switch address mode
        (cpu->*mode)();

        // Execute command and return programm cycles
        // with addition cycles depends on memory mode
        (cpu->*code)();

        return 0x00;
    }
};

#endif