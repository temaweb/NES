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

#ifndef MEM_H
#define MEM_H

#include <memory>
#include <cstdint>

class Bus;

class Mem
{
private:

    /*
        Bus communication interface
        Interact with each other devices i.e. RAM, APU, PPU etc.
    */
    std::shared_ptr<Bus> bus;


public:

    /*
        Initialize with bus
    */
    Mem(std::shared_ptr<Bus> bus);

    /*
        Read byte from bus
    */
    uint8_t read(uint16_t index) const;

    /* 
        Read 2-bytes address from memory direct 
        Shift program counter twice
    */
    uint16_t direct(uint16_t & pc);
   
    /* 
        Read 2-bytes address from memory indirect 
        Shift program counter twice
    */
    uint16_t indirect(uint16_t & pc);

    /*       
        Absolute mode
    */
    uint16_t abs(uint16_t & pc, uint8_t rg = 0x00);

    /*
        Zeropage mode
    */
    uint8_t zpg(uint16_t & pc, uint8_t rg = 0x00);

    /*
        Zeropage indexed indirect
    */
    uint16_t indexed(uint16_t & pc, uint8_t rg = 0x00);

    /*
        Write byte to bus without carry
    */
    void write(uint16_t address, uint8_t data);
};

#endif