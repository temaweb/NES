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

#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <cstdint>
#include <tuple>

class Device
{
public:
    virtual ~Device() = default;

    // Device should be process data in the space
    virtual std::tuple<uint16_t, uint16_t> getSpace() = 0;
};

// Read-only devices
class RDevice : virtual public Device
{
public:
    virtual uint8_t read(uint16_t address) const = 0;
};

// Write-only devices
class WDevice : virtual public Device
{
public:
    virtual void write(uint16_t address, uint8_t data) = 0;
};

// R/W device
class IODevice : public RDevice, public WDevice
{

};

#endif