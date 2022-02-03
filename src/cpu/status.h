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

#ifndef STATUS_H
#define STATUS_H

#include <cstdint>

/*
    6502 Status Register (SR) flags
*/

class Status
{
private:

    enum Flags : uint8_t
    {
        Carry     = 1 << 0,       
        Zero      = 1 << 1, 
        Interrupt = 1 << 2,
        Decimal   = 1 << 3, 
        Break     = 1 << 4, 
        Default   = 1 << 5,
        Overflow  = 1 << 6, 
        Negative  = 1 << 7  
    };

    /*
        Status flags value
    */
    uint8_t status;

    /*
        Returns true if flag is set
    */
    bool isSet(Flags flag) const;

    /*
        Set/Unset flag
    */
    void setFlag(Flags flag, bool value);

    /*
        Get flag as integer value
    */
    uint8_t getFlag(Flags flag) const;


public:

    /*
        Default constructor
    */
    Status();

    /*
        Restore status from uint8_t
    */
    Status(uint8_t status);

    /*
        Test Carry flag by value
    */
    template<typename T> 
    void setCarry(const T & value) {
        setFlag(Flags::Carry, value & 0x100);
    }

    /*
        Set/Unset Carry flag
    */
    void setCarry(bool isSet);

    /*
        Test Negative flag by value
    */
    template<typename T> 
    void setNegative(const T & value) {
        setFlag(Flags::Negative, value & 0x80);
    }

    /*
        Set/Unset Negative flag
    */
    void setNegative(bool isSet);

    /*
        Test Zero flag by value
    */
    template<typename T> 
    void setZero(const T & value) {
        setFlag(Flags::Zero, value == 0x00);
    }

    /*
        Set/Unset Zero flag
    */
    void setZero(bool isSet);

    /*
        Set/Unset Overflow flag
    */
    void setOverflow(bool isSet);

    /*
        Set/Unset Decimal flag
    */
    void setDecimal(bool isSet);

    /*
        Set/Unset Interrupt flag
    */
    void setInterrupt(bool isSet);

    /*
        Returns Carry flag
    */
    uint8_t getCarry() const;

    /*
        Returns Negative flag
    */
    uint8_t getNegative() const;

    /*
        Returns Overflow flag
    */
    uint8_t getOverflow() const;

    /*
        Returns Break flag
    */
    uint8_t getBreak() const;

    /*
        Returns Interrupt flag
    */
    uint8_t getInterrupt() const;

    /*
        Returns Decimal flag
    */
    uint8_t getDecimal() const;

    /*
        Returns Zero flag
    */
    uint8_t getZero() const;

    /*
        Returns true if Decimal flag is set
    */
    bool isDecimal() const;

    /*
        Returns true if Carry flag is set
    */
    bool isCarry() const;

    /*
        Returns true if Negative flag is set
    */
    bool isNegative() const;

    /*
        Returns true if Overflow flag is set
    */
    bool isOverflow() const;

    /*
        Returns true if Carry flag is set
    */
    bool isZero() const;

    /*
        Explicit cast to uint8_t
    */
    operator uint8_t() const;
};

#endif