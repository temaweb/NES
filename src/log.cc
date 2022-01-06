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

#include "log.h"

#include "cpu/cpu.h"
#include "cpu/cmd.h"
#include "bus/bus.h"

#include "fmt/core.h"
#include "fmt/color.h"


// Background text style
static const fmt::text_style code  = fg(fmt::color::green);

// Background text style
static const fmt::text_style dark  = fg(fmt::color::gray);

// Foreground text style
static const fmt::text_style light = fg(fmt::color::dark_gray);


/*
    Default constructor
*/
Log::Log(std::shared_ptr<Bus> bus) : bus(bus) 
{ }


/*
    Disassembly operation and print details
*/
void Log::step (uint16_t pc, const Cmd & cmd, const Cpu * cpu) const
{
    // Programm counter & Operation code
    fmt::print(dark, "{:#06x} ", pc);
    fmt::print(dark, "{:#04x} ", bus -> read(pc));

    // Command name
    fmt::print(code, "{} ", cmd.name);

    // Command arguments    
    printArgs(pc, cmd.bytes); 

    // Registers
    fmt::print(light, 
        "A:{:02X} X:{:02X} Y:{:02X} S:{:02X} ", 
            cpu -> a, 
            cpu -> x, 
            cpu -> y, 
            cpu -> s
    );

    // Status register
    fmt::print(dark, 
        "N:{} Z:{} C:{} I:{} D:{} V:{} B:{}\n", 
            cpu -> p.getNegative(),
            cpu -> p.getZero(),
            cpu -> p.getCarry(),
            cpu -> p.getInterrupt(),
            cpu -> p.getDecimal(),
            cpu -> p.getOverflow(),
            cpu -> p.getBreak()
    );
}


/*
    Pring command arguments
*/
void Log::printArgs(uint16_t pc, uint8_t size) const
{
    for (int i = 1; i < size; i++) {
        fmt::print(light, "{:#04x} ", bus -> read(++pc));
    }

    fmt::print("{:^{}}", "", (3 - size) * 5);   
}