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

#include "mem.h"
#include "bus/bus.h"

Mem::Mem(std::shared_ptr<Bus> bus) : bus(bus)
{ }


/*
    Read byte from bus
*/
uint8_t Mem::read(uint16_t index) const
{
    return bus -> read(index);
}


/* 
    Read 2-bytes address from memory direct 
    Shift program counter twice
*/

uint16_t Mem::direct(uint16_t & pc)
{
    uint16_t lo = read(pc++);
    uint16_t hi = read(pc++);

    return (hi << 8) | lo;
}


/* 
    Read 2-bytes address from memory indirect 
    Shift program counter twice
*/

uint16_t Mem::indirect(uint16_t & pc)
{
    auto index = direct(pc);
    return direct(index);
}


/*       
    Absolute mode
*/

uint16_t Mem::abs(uint16_t & pc, uint8_t rg)
{
    auto index = direct(pc);
    return 0xFFFF & (index + rg);      
} 


/*
    Zeropage mode
*/

uint8_t Mem::zpg(uint16_t & pc, uint8_t rg)
{
    auto lo = read(pc++);

    // Zeropage only
    return 0x00FF & (lo + rg);
}


/*
    Zeropage indexed indirect
*/

uint16_t Mem::indexed(uint16_t & pc, uint8_t rg)
{
    uint16_t zp = zpg(pc, rg);

    uint16_t lo = read(0x00FF & zp++);
    uint16_t hi = read(0x00FF & zp++);

    return (hi << 8) | lo;
}


/*
    Write byte to bus without carry
*/
void Mem::write(uint16_t address, uint8_t data)
{
    bus -> write(address, data);
}


/*
    Push data on stack
*/
void Mem::push(uint8_t & sp, uint8_t data)
{
    write(beg + sp, data);
    sp--;
}


/*
    Pull data from stack
*/
uint8_t Mem::pop(uint8_t & sp)
{
    sp++;
    return read(beg + sp);
}