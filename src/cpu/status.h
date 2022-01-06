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

#ifndef STATUS_HPP
#define STATUS_HPP

#include <cstdint>

//
// 6502 Status Register (SR) flags
//

class Status
{
private:

    enum Flags : uint8_t
    {
        Carry     = 1,      // P0 - (C)  - Carry flag  
        Zero      = 1 << 1, // P1 - (Z)  - Zero
        Interrupt = 1 << 2, // P2 - (I)  - Interrupt (IRQ disable)
        Decimal   = 1 << 3, // P3 - (D)  - Decimal (use BCD for arithmetics)
        Break     = 1 << 4, // P4 - (B)  - Break
        Default   = 1 << 5, // P5 - (1)  - Ignored (always 1)
        Overflow  = 1 << 6, // P6 - (V)  - Overflow
        Negative  = 1 << 7  // P7 - (N)  - Negative
    };

    uint8_t status = Status::Default;

    void setFlag(Status::Flags flag, bool value)
    {
        if (value) {
            status |= flag;
        } else {
            status &= ~flag;
        }
    }

    uint8_t getFlag(Status::Flags flag) const { 
        return isSet(flag) ? 1 : 0;
    }

    bool isSet(Flags flag) const {
        return (status & flag) == flag;
    }

public:

    // Default constructor
    Status() = default;

    // Implicit cast uint8_t to Status type
    Status(uint8_t status) {
        this -> status = status | Status::Default;
    }

    // Implicit cast to uint8_t
    operator uint8_t () const {
        return this -> status;
    }

    bool isDecimal() const {
        return isSet(Status::Decimal);
    }

    template<typename T>
    void setCarry(const T & value) {
        setFlag(Status::Carry, value & 0x100);
    }

    template<typename T>
    void setNegative(const T & value) {
        setFlag(Status::Negative, value & 0x80);
    }

    template<typename T>
    void setZero(const T & value) {
        setFlag(Status::Zero, value == 0x00);
    }

    uint8_t getCarry() const {
        return getFlag(Status::Carry);
    }

    uint8_t getNegative() const {
        return getFlag(Status::Negative);
    }

    uint8_t getOverflow() const {
        return getFlag(Status::Overflow);
    }

    uint8_t getBreak() const {
        return getFlag(Status::Break);
    }

    uint8_t getInterrupt() const {
        return getFlag(Status::Interrupt);
    }

    uint8_t getDecimal() const {
        return getFlag(Status::Decimal);
    }

    uint8_t getZero() const {
        return getFlag(Status::Zero);
    }
};

#endif