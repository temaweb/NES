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

#include "cpu.hpp"
#include "bus.hpp"
#include "map.hpp"

class Cpu::Imp
{
private:

    // Bus communication interface
    // Interact with each other devices i.e. RAM, APU, PPU etc.
    std::shared_ptr<Bus> bus;


public:

    Imp(std::shared_ptr<Bus> bus) : bus(bus)
    { }

    // Read data from bus
    uint8_t read(uint16_t index)
    {
        return bus -> read(index);
    }

    // ABS
    // Returns data in absolute mode and shift programm counter
    uint8_t abs(uint16_t & pc, uint8_t reg = 0x00)
    {
        uint16_t hi = read(pc++);
        uint16_t lo = read(pc++);

        return read( ((hi << 8) | lo) + reg );      
    }
};


/*
    Default constructor
*/

Cpu::Cpu(std::shared_ptr<Bus> bus)
{
    map = std::make_unique<Map>();
    imp = std::make_unique<Imp>(bus);
}


/*
    Read operation code and execute command
    Returns total programm cycles per operation
*/

int Cpu::clock ()
{
    auto code = imp -> read(pc++);  
    auto oper = map -> getCommand(code);

    // Execute command and returns programm cycles
    return oper.execute(this);
}


/*
    Reset CPU and clear all registers & flags
*/

void Cpu::reset ()
{
    x = 0x00;
    y = 0x00;
    p = 0x00;
    s = 0x00;
    a = 0x00;

    pc = 0x0000;
}


/*
    Immediate Addressing (Immediate)

    With immediate addressing, the operand is contained in the
    second byte of the instruction; no further memory address-
    ing is required.
*/

void Cpu::IMM () 
{ 
    op = imp -> read(pc++);
}


/*
    Absolute Addressing (Absolute)

    For absolute addressing, the second byte of the instruction
    specifies the eight low-order bits of the effective address,
    while the third byte specifies the eight high-order bits.
    Therefore, this addressing mode allows access to the total
    64K bytes of addressable memory.
*/

void Cpu::ABS () 
{
    op = imp -> abs(pc);
}


/*     
    Absolute Indexed Addressing (ABS, X or ABS, Y)

    Absolute indexed addressing is used in conjunction with X
    or Y index register and is referred to as "Absolute, X" and
    “Absolute, Y" The effective address is formed by adding the
    contents of X or Y to the address contained in the second
    and third bytes of the instruction. This mode allows the
    index register to contain the index or count value and the
    instruction to contain the base address. This type of index-
    ing allows any location referencing and the index to modify
    multiple fields, resulting in reduced coding and execution
    time. 
*/

void Cpu::ABSX () 
{ 
    op = imp -> abs(pc, x);
}

void Cpu::ABSY () 
{ 
    op = imp -> abs(pc, y);
}


/* 
    Zero Page Addressing (Zero Page)

    Zero page addressing allows shorter code and execution
    times by only fetching the second byte of the instruction
    and assuming a zero high address byte. The careful use of
    zero page addressing can result in significant increase in
    code efficiency. 
*/

void Cpu::ZPG () 
{

}


/* 
    Zero Page Indexed Addressing (ZPG, X or ZPG, Y)

    Zero page absolute addressing is used in conjunction with
    the index register and is referred to as "Zero Page, X" or
    "Zero Page, Y" The effective address is calculated by adding
    the second byte to the contents of the index register. Since
    this is a form of "Zero Page" addressing, the content of the
    second byte references a location in page zero. Additionally,
    due to the "Zero Page" addressing nature of this mode, no
    carry is added to the high-order eight bits of memory, and
    crossing of page boundaries does not occur. 
*/

void Cpu::ZPGX () { }
void Cpu::ZPGY () { }


/* 
    Implied Addressing (Implied)

    In the implied addressing mode, the address containing the
    operand is implicitly stated in the operation code of the
    instruction. 
*/

void Cpu::IMP () { }


/* 
    Accumulator Addressing (Accum)

    This form of addressing is represented with a one byte
    instruction and implies an operation on the accumulator. 
*/

void Cpu::ACC () { }


/* 
    Absolute Indirect Addressing (Jump Instruction Only)

    The second byte of the instruction contains the low-order
    eight bits of a memory location. The high-order eight bits of
    that memory location is contained in the third byte of the
    instruction. The contents of the fully specified memory loca-
    tion is the low-order byte of the effective address. The next
    memory location contains the high-order byte of the effec-
    tive address which is loaded into the 16 bit program
    counter. 
*/

void Cpu::IND () { }


/* 
    Absolute Indexed Indirect Addressing (Jump Instruction Only)

    With absolute indexed indirect addressing the contents of
    the second and third instruction bytes are added to the X
    register. The result of this addition, points to a memory loca-
    tion containing the lower-order eight bits of the effective
    address. The next memory location contains the higher-
    order eight bits of the effective address. 
*/

void Cpu::INDX () { }


/* 
    Indirect Indexed Addressing [(IND), Y]

    This form of addressing is usually referred to as Indirect, Y.
    The second byte of the instruction points to a memory loca-
    tion in page zero. The contents of this memory location are
    added to the contents of the Y index register, the result
    being the low-order eight bits of the effective address. The
    carry from this addition is added to the contents of the next
    page zero memory location, the result being the high-order
    eight bits of the effective address. 
*/

void Cpu::INDY () { }


/* 
    Relative Addressing (Relative)

    Relative addressing is used only with branch instructions; it
    establishes a destination for the conditional branch. The
    second byte of the instruction becomes the operand which
    is an “Offset” added to the contents of the Jower eight bits
    of the program counter when the counter is set at the next
    instruction. The range of the offset is -128 to +127 bytes
    from the next instruction. 
*/

void Cpu::REL () { }


/*
    ADC
    Add Memory to Accumulator with Carry
 
    A + M + C -> A, C                          N Z C I D V
                                               + + + - - +
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | immediate    | ADC #oper    | 69  | 2     | 2      |
    | zeropage     | ADC oper     | 65  | 2     | 3      |
    | zeropage,X   | ADC oper,X   | 75  | 2     | 4      |
    | absolute     | ADC oper     | 6D  | 3     | 4      |
    | absolute,X   | ADC oper,X   | 7D  | 3     | 4*     |
    | absolute,Y   | ADC oper,Y   | 79  | 3     | 4*     |
    | (indirect,X) | ADC (oper,X) | 61  | 2     | 6      |
    | (indirect),Y | ADC (oper),Y | 71  | 2     | 5*     |
    +--------------+--------------+-----+-------+--------+
*/

uint8_t Cpu::ADC() 
{ 
    if (p.isDecimal())
    {
        // BCD mode
    }

    return 0x00; 
}


/*
    ALR (ASR)
    AND oper + LSR

    A AND oper, 0 -> [76543210] -> C      N Z C I D V
                                          + + + - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | immediate  | ALR #oper | 4B  | 2     | 2      |
    +------------+-----------+-----+-------+--------+
*/

uint8_t Cpu::ALR() 
{
    return 0x00; 
}


/*
    ANC
    AND oper + set C as ASL

    A AND oper, bit(7) -> C               N Z C I D V
                                          + + + - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | immediate  | ANC #oper | 0B  | 2     | 2      |
    +------------+-----------+-----+-------+--------+
*/

uint8_t Cpu::ANC() 
{ 
    return 0x00; 
}

/*
    AND
    And Memory with Accumulator

    A AND M -> A                               N Z C I D V
                                               + + - - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | immediate    | AND #oper    | 29  | 2     | 2      |
    | zeropage     | AND oper     | 25  | 2     | 3      |
    | zeropage,X   | AND oper,X   | 35  | 2     | 4      |
    | absolute     | AND oper     | 2D  | 3     | 4      |
    | absolute,X   | AND oper,X   | 3D  | 3     | 4*     |
    | absolute,Y   | AND oper,Y   | 39  | 3     | 4*     |
    | (indirect,X) | AND (oper,X) | 21  | 2     | 6      |
    | (indirect),Y | AND (oper),Y | 31  | 2     | 5*     |
    +--------------+--------------+-----+-------+--------+
*/

uint8_t Cpu::AND() 
{ 
    return 0x00; 
}

uint8_t Cpu::ANE() { return 0x00; }
uint8_t Cpu::ARR() { return 0x00; }
uint8_t Cpu::ASL() { return 0x00; }
uint8_t Cpu::BCC() { return 0x00; }
uint8_t Cpu::BCS() { return 0x00; }
uint8_t Cpu::BEQ() { return 0x00; }
uint8_t Cpu::BIT() { return 0x00; }
uint8_t Cpu::BMI() { return 0x00; }
uint8_t Cpu::BNE() { return 0x00; }
uint8_t Cpu::BPL() { return 0x00; }
uint8_t Cpu::BRK() { return 0x00; }
uint8_t Cpu::BVC() { return 0x00; }
uint8_t Cpu::BVS() { return 0x00; }
uint8_t Cpu::CLC() { return 0x00; }
uint8_t Cpu::CLD() { return 0x00; }
uint8_t Cpu::CLI() { return 0x00; }
uint8_t Cpu::CLV() { return 0x00; }
uint8_t Cpu::CMP() { return 0x00; }
uint8_t Cpu::CPX() { return 0x00; }
uint8_t Cpu::CPY() { return 0x00; }
uint8_t Cpu::DCP() { return 0x00; }
uint8_t Cpu::DEC() { return 0x00; }
uint8_t Cpu::DEX() { return 0x00; }
uint8_t Cpu::DEY() { return 0x00; }
uint8_t Cpu::EOR() { return 0x00; }
uint8_t Cpu::INC() { return 0x00; }
uint8_t Cpu::INX() { return 0x00; }
uint8_t Cpu::INY() { return 0x00; }
uint8_t Cpu::ISC() { return 0x00; }
uint8_t Cpu::JAM() { return 0x00; }
uint8_t Cpu::JMP() { return 0x00; }
uint8_t Cpu::JSR() { return 0x00; }
uint8_t Cpu::LAS() { return 0x00; }
uint8_t Cpu::LAX() { return 0x00; }
uint8_t Cpu::LDA() { return 0x00; }
uint8_t Cpu::LDX() { return 0x00; }
uint8_t Cpu::LDY() { return 0x00; }
uint8_t Cpu::LSR() { return 0x00; }
uint8_t Cpu::LXA() { return 0x00; }
uint8_t Cpu::NOP() { return 0x00; }
uint8_t Cpu::ORA() { return 0x00; }
uint8_t Cpu::PHA() { return 0x00; }
uint8_t Cpu::PHP() { return 0x00; }
uint8_t Cpu::PLA() { return 0x00; }
uint8_t Cpu::PLP() { return 0x00; }
uint8_t Cpu::RLA() { return 0x00; }
uint8_t Cpu::ROL() { return 0x00; }
uint8_t Cpu::ROR() { return 0x00; }
uint8_t Cpu::RRA() { return 0x00; }
uint8_t Cpu::RTI() { return 0x00; }
uint8_t Cpu::RTS() { return 0x00; }
uint8_t Cpu::SAX() { return 0x00; }
uint8_t Cpu::SBC() { return 0x00; }
uint8_t Cpu::SBX() { return 0x00; }
uint8_t Cpu::SEC() { return 0x00; }
uint8_t Cpu::SED() { return 0x00; }
uint8_t Cpu::SEI() { return 0x00; }
uint8_t Cpu::SHA() { return 0x00; }
uint8_t Cpu::SHY() { return 0x00; }
uint8_t Cpu::SHX() { return 0x00; }
uint8_t Cpu::SLO() { return 0x00; }
uint8_t Cpu::SRE() { return 0x00; }
uint8_t Cpu::STA() { return 0x00; }
uint8_t Cpu::STX() { return 0x00; }
uint8_t Cpu::STY() { return 0x00; }
uint8_t Cpu::TAS() { return 0x00; }
uint8_t Cpu::TAX() { return 0x00; }
uint8_t Cpu::TAY() { return 0x00; }
uint8_t Cpu::TSX() { return 0x00; }
uint8_t Cpu::TXA() { return 0x00; }
uint8_t Cpu::TXS() { return 0x00; }
uint8_t Cpu::TYA() { return 0x00; }
uint8_t Cpu::USBC() { return 0x00; }