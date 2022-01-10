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

#include "status.h"

/*
    Default constructor
*/
Status::Status() : status(Flags::Default)
{ }

/*
    Restore status from uint8_t
*/
Status::Status(uint8_t value) : status(value | Flags::Default) 
{ }

/*
    Set/Unset flag
*/
void Status::setFlag(Flags flag, bool value) 
{
    if (value) {
        status |= flag;
    } else {
        status &= ~flag;
    }
}

/*
    Get flag as integer value
*/
uint8_t Status::getFlag(Flags flag) const {
    return !!(status & flag);
}

/*
    Returns true if flag is set
*/
bool Status::isSet(Flags flag) const {
    return (status & flag) == flag;
}

/*
    Returns true if Decimal flag is set
*/
bool Status::isDecimal() const {
    return isSet(Flags::Decimal);
}

/*
    Returns Carry flag
*/
uint8_t Status::getCarry() const {
    return getFlag(Flags::Carry);
}

/*
    Returns Negative flag
*/
uint8_t Status::getNegative() const {
    return getFlag(Flags::Negative);
}

/*
    Returns Overflow flag
*/
uint8_t Status::getOverflow() const {
    return getFlag(Flags::Overflow);
}

/*
    Returns Break flag
*/
uint8_t Status::getBreak() const {
    return getFlag(Flags::Break);
}

/*
    Returns Interrupt flag
*/
uint8_t Status::getInterrupt() const {
    return getFlag(Flags::Interrupt);
}

/*
    Returns Decimal flag
*/
uint8_t Status::getDecimal() const {
    return getFlag(Flags::Decimal);
}

/*
    Returns Zero flag
*/
uint8_t Status::getZero() const {
    return getFlag(Flags::Zero);
}

/*
    Explicit cast to uint8_t
*/
Status::operator uint8_t() const {
    return status;
}