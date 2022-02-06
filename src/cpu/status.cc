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
    Returns true if Carry flag is set
*/
bool Status::isCarry() const {
    return isSet(Flags::Carry);
}

/*
    Returns true if Break flag is set
*/
bool Status::isBreak() const {
    return isSet(Flags::Break);
}

/*
    Returns true if Zero flag is set
*/
bool Status::isZero() const {
    return isSet(Flags::Zero);
}

/*
    Returns true if Negative flag is set
*/
bool Status::isNegative() const {
    return isSet(Flags::Negative);
}

/*
    Returns true if Overflow flag is set
*/
bool Status::isOverflow() const {
    return isSet(Flags::Overflow);
}

/*
    Set/Unset Carry flag
*/
void Status::setCarry(bool isSet) {
    setFlag(Flags::Carry, isSet);
}

/*
    Set/Unset Negative flag
*/
void Status::setNegative(bool isSet) {
    setFlag(Flags::Negative, isSet);
}

/*
    Set/Unset Negative flag
*/
void Status::setOverflow(bool isSet) {
    setFlag(Flags::Overflow, isSet);
}

/*
    Set/Unset Decimal flag
*/
void Status::setDecimal(bool isSet) {
    setFlag(Flags::Decimal, isSet);
}

/*
    Set/Unset Interrupt flag
*/
void Status::setInterrupt(bool isSet) {
    setFlag(Flags::Interrupt, isSet);
}

/*
    Set/Unset Zero flag
*/
void Status::setZero(bool isSet) {
    setFlag(Flags::Zero, isSet);
}

/*
    Set/Unset Break flag
*/
void Status::setBreak(bool isSet) {
    setFlag(Flags::Break, isSet);
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
    Returns Default flag
*/
uint8_t Status::getDefault() const {
    return getFlag(Flags::Default);
}

/*
    Explicit cast to uint8_t
*/
Status::operator uint8_t() const {
    return status | Flags::Break | Flags::Default;
}