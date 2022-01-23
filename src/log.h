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

#ifndef LOG_H
#define LOG_H

#include <cstdint>
#include <memory>
#include <string>

class Cpu;
class Bus; 
class Cmd;

class Log
{
private:
    std::shared_ptr<Bus> bus;
    
    void printArgs(uint16_t pc, uint8_t size) const;

public:
    Log(std::shared_ptr<Bus> bus);

    /*
        Disassembly operation
    */
    void step (uint16_t pc, const Cmd & cmd, const Cpu * cpu) const;
};

#endif