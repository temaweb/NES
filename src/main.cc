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

#include <memory>
#include <iostream>

#include "log.h"

#include "cpu/cpu.h"
#include "bus/bus.h"

#include "fmt/core.h"
#include "fmt/color.h"

// Caption text style
static const fmt::text_style caption = fg(fmt::color::dark_gray) | fmt::emphasis::underline;

int main()
{
    fmt::print(caption, "\nDissassembly\n\n");

    auto bus = std::make_shared<Bus>();
    auto log = std::make_shared<Log>(bus);
    auto cpu = std::make_unique<Cpu>(bus);

    bus -> write(0x0100, 0xA9); // LDA #$01 ; load accumulator with memory
    bus -> write(0x0101, 0x01);

    bus -> write(0x0102, 0x99); // STA $0002,Y ; store accumulator in memory
    bus -> write(0x0103, 0x02);
    bus -> write(0x0104, 0x00);

    bus -> write(0x0105, 0x69); // ADC #$01 ; add memory to accumulator with carry
    bus -> write(0x0106, 0x01); 

    auto steps = 5;

    while (steps--) {
        cpu -> clock();
    }

    // Memory dump
    fmt::print(caption, "\n\nMemory dump from {:#04x} to {:#04x}\n", 0x00, 0xFF);

    bus -> printDump(0x02, 0xF2);
    fmt::print("\n\n");
}