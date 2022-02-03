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
#include <fstream>

#include "log.h"

#include "cpu/cpu.h"
#include "bus/bus.h"

#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/color.h"

#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

// Caption text style
static const fmt::text_style caption = fg(fmt::color::dark_gray) | fmt::emphasis::underline;

// Memory bus
std::shared_ptr<Bus> bus = std::make_shared<Bus>();

/*
    Load ROM to memory
*/
void load(std::string path)
{
    std::ifstream file(path, std::ios::in | std::ios::binary);

    if (!file.is_open())
    {
        std::cerr << "File not found " << path;
        exit(EXIT_FAILURE);
        
        return;
    }
    
    #ifdef WIN32

        // ~

    #else
        file.read ( 
            (std::ifstream::char_type *) bus -> begin(), 
            (std::streamsize) bus -> size()
        );
    #endif

    file.close();
}


/*
    Load ROM
*/
void load_rom(std::string rom)
{
    auto path = fmt::format("../ext/asm/bin_files/{}", rom);
    load(path);
}


/*
    Run CPU
*/
void run(int cycles)
{
    auto log = std::make_shared<Log>(bus);
    auto cpu = std::make_unique<Cpu>(bus);

    fmt::print(caption, "\nDissassembly\n\n");
        
    while (cycles--) {
        cpu -> clock();
    }
}


/*
    Print memory dump
*/
void dump(uint16_t from, uint16_t to)
{
    fmt::print(caption, "\n\nMemory dump from {:#04x} to {:#04x}\n", 0x00, 0xFF);

    bus -> printDump(from, to);
    fmt::print("\n\n");
}

/*
    ~
*/
int main(int argc, char** argv)
{
    CLI::App app {"MOS 6502 CPU Emulator"};

    uint16_t c;
    uint16_t f;
    uint16_t t; 

    app.add_option ("-c", c, "CPU loop cycles")                
        -> default_val(1000);

    app.add_option ("-f", f, "Print memory dump from address") 
        -> default_val(0x0000);

    app.add_option ("-t", t, "Print memory dump to address")   
        -> default_val(0x00FF);

    try
    {
        app.parse(argc, argv);
        load_rom("6502_functional_test.bin");

        // Run CPU loop
        run (c);
 
        // Print memory dump
        dump (f, t);
    }
    catch(const CLI::ParseError & e) {
        return app.exit(e);
    }
    catch(const std::exception & e) {
        std::cerr << e.what() << '\n';
    }
}