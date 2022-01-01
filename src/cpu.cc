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

Cpu::Cpu(Bus * bus) : bus(bus)
{
    #pragma region instructions

    cmd =
    {{
        // 0x00 - 0x0F

        { &Cpu::BRK, &Cpu::IMP  }, // 0x00
        { &Cpu::ORA, &Cpu::INDX }, // 0x01
        { &Cpu::JAM, &Cpu::IMP  }, // 0x02 *
        { &Cpu::SLO, &Cpu::INDX }, // 0x03 *
        { &Cpu::NOP, &Cpu::ZPG  }, // 0x04 *
        { &Cpu::ORA, &Cpu::ZPG  }, // 0x05
        { &Cpu::ASL, &Cpu::ZPG  }, // 0x06
        { &Cpu::SLO, &Cpu::ZPG  }, // 0x07 *
        { &Cpu::PHP, &Cpu::IMP  }, // 0x08
        { &Cpu::ORA, &Cpu::IMM  }, // 0x09
        { &Cpu::ASL, &Cpu::ACC  }, // 0x0A
        { &Cpu::ANC, &Cpu::IMM  }, // 0x0B *
        { &Cpu::NOP, &Cpu::ABS  }, // 0x0C *
        { &Cpu::ORA, &Cpu::ABS  }, // 0x0D
        { &Cpu::ASL, &Cpu::ABS  }, // 0x0E
        { &Cpu::SLO, &Cpu::ABS  }, // 0x0F *

        // 0x10 - 0x1F

        { &Cpu::BPL, &Cpu::REL  }, // 0x10
        { &Cpu::ORA, &Cpu::INDY }, // 0x11
        { &Cpu::JAM, &Cpu::IMP  }, // 0x12 *
        { &Cpu::SLO, &Cpu::INDY }, // 0x13 *
        { &Cpu::NOP, &Cpu::ZPGX }, // 0x14 *
        { &Cpu::ORA, &Cpu::ZPGX }, // 0x15
        { &Cpu::ASL, &Cpu::ZPGX }, // 0x16
        { &Cpu::SLO, &Cpu::ZPGX }, // 0x17 *
        { &Cpu::CLC, &Cpu::IMP  }, // 0x18
        { &Cpu::ORA, &Cpu::ABSY }, // 0x19
        { &Cpu::NOP, &Cpu::IMP  }, // 0x1A *
        { &Cpu::SLO, &Cpu::ABSY }, // 0x1B *
        { &Cpu::NOP, &Cpu::ABSX }, // 0x1C *
        { &Cpu::ORA, &Cpu::ABSX }, // 0x1D
        { &Cpu::ASL, &Cpu::ABSX }, // 0x1E
        { &Cpu::SLO, &Cpu::ABSX }, // 0x1F *

        // 0x20 - 0x2F

        { &Cpu::JSR, &Cpu::ABS  }, // 0X20
        { &Cpu::AND, &Cpu::INDX }, // 0X21
        { &Cpu::JAM, &Cpu::IMP  }, // 0X22 *
        { &Cpu::RLA, &Cpu::INDX }, // 0X23 *
        { &Cpu::BIT, &Cpu::ZPG  }, // 0X24
        { &Cpu::AND, &Cpu::ZPG  }, // 0X25
        { &Cpu::ROL, &Cpu::ZPG  }, // 0X26
        { &Cpu::RLA, &Cpu::ZPG  }, // 0X27 *
        { &Cpu::PLP, &Cpu::IMP  }, // 0X28
        { &Cpu::AND, &Cpu::IMM  }, // 0X29
        { &Cpu::ROL, &Cpu::ACC  }, // 0X2A
        { &Cpu::ANC, &Cpu::IMM  }, // 0X2B *
        { &Cpu::BIT, &Cpu::ABS  }, // 0X2C
        { &Cpu::AND, &Cpu::ABS  }, // 0X2D
        { &Cpu::ROL, &Cpu::ABS  }, // 0X2E
        { &Cpu::RLA, &Cpu::ABS  }, // 0X2F *

        // 0x30 - 0x3F

        { &Cpu::BMI, &Cpu::REL  }, // 0x30
        { &Cpu::AND, &Cpu::INDY }, // 0x31
        { &Cpu::JAM, &Cpu::IMP  }, // 0x32 *
        { &Cpu::RLA, &Cpu::INDY }, // 0x33 *
        { &Cpu::NOP, &Cpu::ZPGX }, // 0x34 *
        { &Cpu::AND, &Cpu::ZPGX }, // 0x35
        { &Cpu::ROL, &Cpu::ZPGX }, // 0x36
        { &Cpu::RLA, &Cpu::ZPGX }, // 0x37 *
        { &Cpu::SEC, &Cpu::IMP  }, // 0x38
        { &Cpu::AND, &Cpu::ABSY }, // 0x39
        { &Cpu::NOP, &Cpu::IMP  }, // 0x3A *
        { &Cpu::RLA, &Cpu::ABSY }, // 0x3B *
        { &Cpu::NOP, &Cpu::ABSX }, // 0x3C *
        { &Cpu::AND, &Cpu::ABSX }, // 0x3D
        { &Cpu::ROL, &Cpu::ABSX }, // 0x3E
        { &Cpu::RLA, &Cpu::ABSX }, // 0x3F *

        // 0x40 - 0x4F

        { &Cpu::RTI, &Cpu::IMP  }, // 0x40
        { &Cpu::EOR, &Cpu::INDX }, // 0x41
        { &Cpu::JAM, &Cpu::IMP  }, // 0x42 *
        { &Cpu::SRE, &Cpu::INDX }, // 0x43 *
        { &Cpu::NOP, &Cpu::ZPG  }, // 0x44 *
        { &Cpu::EOR, &Cpu::ZPG  }, // 0x45
        { &Cpu::LSR, &Cpu::ZPG  }, // 0x46
        { &Cpu::SRE, &Cpu::ZPG  }, // 0x47 *
        { &Cpu::PHA, &Cpu::IMP  }, // 0x48
        { &Cpu::EOR, &Cpu::IMM  }, // 0x49
        { &Cpu::LSR, &Cpu::ACC  }, // 0x4A
        { &Cpu::ALR, &Cpu::IMM  }, // 0x4B *
        { &Cpu::JMP, &Cpu::ABS  }, // 0x4C
        { &Cpu::EOR, &Cpu::ABS  }, // 0x4D
        { &Cpu::LSR, &Cpu::ABS  }, // 0x4E
        { &Cpu::SRE, &Cpu::ABS  }, // 0x4F *

        // 0x50 - 0x5F

        { &Cpu::BVC, &Cpu::REL  }, // 0x50
        { &Cpu::EOR, &Cpu::INDY }, // 0x51
        { &Cpu::JAM, &Cpu::IMP  }, // 0x52 *
        { &Cpu::SRE, &Cpu::INDY }, // 0x53 *
        { &Cpu::NOP, &Cpu::ZPGX }, // 0x54 *
        { &Cpu::EOR, &Cpu::ZPGX }, // 0x55
        { &Cpu::LSR, &Cpu::ZPGX }, // 0x56
        { &Cpu::SRE, &Cpu::ZPGX }, // 0x57 *
        { &Cpu::CLI, &Cpu::IMP  }, // 0x58
        { &Cpu::EOR, &Cpu::ABSY }, // 0x59
        { &Cpu::NOP, &Cpu::IMP  }, // 0x5A *
        { &Cpu::SRE, &Cpu::ABSY }, // 0x5B *
        { &Cpu::NOP, &Cpu::ABSX }, // 0x5C *
        { &Cpu::EOR, &Cpu::ABSX }, // 0x5D
        { &Cpu::LSR, &Cpu::ABSX }, // 0x5E
        { &Cpu::SRE, &Cpu::ABSX }, // 0x5F *

        // 0x60 - 0x6F

        { &Cpu::RTS, &Cpu::IMP  }, // 0x60
        { &Cpu::ADC, &Cpu::INDX }, // 0x61
        { &Cpu::JAM, &Cpu::IMP  }, // 0x62 *
        { &Cpu::RRA, &Cpu::INDX }, // 0x63 *
        { &Cpu::NOP, &Cpu::ZPG  }, // 0x64 *
        { &Cpu::ADC, &Cpu::ZPG  }, // 0x65
        { &Cpu::ROR, &Cpu::ZPG  }, // 0x66
        { &Cpu::RRA, &Cpu::ZPG  }, // 0x67 *
        { &Cpu::PLA, &Cpu::IMP  }, // 0x68
        { &Cpu::ADC, &Cpu::IMM  }, // 0x69
        { &Cpu::ROR, &Cpu::ACC  }, // 0x6A
        { &Cpu::ARR, &Cpu::IMM  }, // 0x6B *
        { &Cpu::JMP, &Cpu::IND  }, // 0x6C
        { &Cpu::ADC, &Cpu::ABS  }, // 0x6D
        { &Cpu::ROR, &Cpu::ABS  }, // 0x6E
        { &Cpu::RRA, &Cpu::ABS  }, // 0x6F *

        // 0x70 - 0x7F

        { &Cpu::BVS, &Cpu::REL  }, // 0x70
        { &Cpu::ADC, &Cpu::INDY }, // 0x71
        { &Cpu::JAM, &Cpu::IMP  }, // 0x72 *
        { &Cpu::RRA, &Cpu::INDY }, // 0x73 *
        { &Cpu::NOP, &Cpu::ZPGX }, // 0x74 *
        { &Cpu::ADC, &Cpu::ZPGX }, // 0x75
        { &Cpu::ROR, &Cpu::ZPGX }, // 0x76
        { &Cpu::RRA, &Cpu::ZPGX }, // 0x77 *
        { &Cpu::SEI, &Cpu::IMP  }, // 0x78
        { &Cpu::ADC, &Cpu::ABSY }, // 0x79
        { &Cpu::NOP, &Cpu::IMP  }, // 0x7A *
        { &Cpu::RRA, &Cpu::ABSY }, // 0x7B *
        { &Cpu::NOP, &Cpu::ABSX }, // 0x7C *
        { &Cpu::ADC, &Cpu::ABSX }, // 0x7D
        { &Cpu::ROR, &Cpu::ABSX }, // 0x7E
        { &Cpu::RRA, &Cpu::ABSX }, // 0x7F *

        // 0x80 - 0x8F

        { &Cpu::NOP, &Cpu::IMM  }, // 0x80 *
        { &Cpu::STA, &Cpu::INDX }, // 0x81
        { &Cpu::NOP, &Cpu::IMM  }, // 0x82 *
        { &Cpu::SAX, &Cpu::INDX }, // 0x83 *
        { &Cpu::STY, &Cpu::ZPG  }, // 0x84
        { &Cpu::STA, &Cpu::ZPG  }, // 0x85
        { &Cpu::STX, &Cpu::ZPG  }, // 0x86
        { &Cpu::SAX, &Cpu::ZPG  }, // 0x87 *
        { &Cpu::DEY, &Cpu::IMP  }, // 0x88
        { &Cpu::NOP, &Cpu::IMM  }, // 0x89 *
        { &Cpu::TXA, &Cpu::IMP  }, // 0x8A
        { &Cpu::ANE, &Cpu::IMM  }, // 0x8B *
        { &Cpu::STY, &Cpu::ABS  }, // 0x8C
        { &Cpu::STA, &Cpu::ABS  }, // 0x8D
        { &Cpu::STX, &Cpu::ABS  }, // 0x8E
        { &Cpu::SAX, &Cpu::ABS  }, // 0x8F *

        // 0x90 - 0x9F

        { &Cpu::BCC, &Cpu::REL  }, // 0x90
        { &Cpu::STA, &Cpu::INDY }, // 0x91
        { &Cpu::JAM, &Cpu::IMP  }, // 0x92 *
        { &Cpu::SHA, &Cpu::INDY }, // 0x93 *
        { &Cpu::STY, &Cpu::ZPGX }, // 0x94
        { &Cpu::STA, &Cpu::ZPGX }, // 0x95
        { &Cpu::STX, &Cpu::ZPGY }, // 0x96
        { &Cpu::SAX, &Cpu::ZPGY }, // 0x97 *
        { &Cpu::TYA, &Cpu::IMP  }, // 0x98
        { &Cpu::STA, &Cpu::ABSY }, // 0x99
        { &Cpu::TXS, &Cpu::IMP  }, // 0x9A
        { &Cpu::TAS, &Cpu::ABSY }, // 0x9B *
        { &Cpu::SHY, &Cpu::ABSX }, // 0x9C *
        { &Cpu::STA, &Cpu::ABSX }, // 0x9D
        { &Cpu::SHX, &Cpu::ABSY }, // 0x9E *
        { &Cpu::SHA, &Cpu::ABSY }, // 0x9F *

        // 0xA0 - 0xAF

        { &Cpu::LDY, &Cpu::IMM  }, // 0xA0
        { &Cpu::LDA, &Cpu::INDX }, // 0xA1
        { &Cpu::LDX, &Cpu::IMM  }, // 0xA2
        { &Cpu::LAX, &Cpu::INDX }, // 0xA3 *
        { &Cpu::LDY, &Cpu::ZPG  }, // 0xA4
        { &Cpu::LDA, &Cpu::ZPG  }, // 0xA5
        { &Cpu::LDX, &Cpu::ZPG  }, // 0xA6
        { &Cpu::LAX, &Cpu::ZPG  }, // 0xA7 *
        { &Cpu::TAY, &Cpu::IMP  }, // 0xA8
        { &Cpu::LDA, &Cpu::IMM  }, // 0xA9
        { &Cpu::TAX, &Cpu::IMP  }, // 0xAA
        { &Cpu::LXA, &Cpu::IMM  }, // 0xAB *
        { &Cpu::LDY, &Cpu::ABS  }, // 0xAC
        { &Cpu::LDA, &Cpu::ABS  }, // 0xAD
        { &Cpu::LDX, &Cpu::ABS  }, // 0xAE
        { &Cpu::LAX, &Cpu::ABS  }, // 0xAF *

        // 0xB0 - 0xBF

        { &Cpu::BCS, &Cpu::REL  }, // 0xB0
        { &Cpu::LDA, &Cpu::INDY }, // 0xB1
        { &Cpu::JAM, &Cpu::IMP  }, // 0xB2 *
        { &Cpu::LAX, &Cpu::INDY }, // 0xB3 *
        { &Cpu::LDY, &Cpu::ZPGX }, // 0xB4
        { &Cpu::LDA, &Cpu::ZPGX }, // 0xB5
        { &Cpu::LDX, &Cpu::ZPGY }, // 0xB6
        { &Cpu::LAX, &Cpu::ZPGY }, // 0xB7 *
        { &Cpu::CLV, &Cpu::IMP  }, // 0xB8
        { &Cpu::LDA, &Cpu::ABSY }, // 0xB9
        { &Cpu::TSX, &Cpu::IMP  }, // 0xBA
        { &Cpu::LAS, &Cpu::ABSY }, // 0xBB *
        { &Cpu::LDY, &Cpu::ABSX }, // 0xBC
        { &Cpu::LDA, &Cpu::ABSX }, // 0xBD
        { &Cpu::LDX, &Cpu::ABSY }, // 0xBE
        { &Cpu::LAX, &Cpu::ABSY }, // 0xBF *

        // 0xC0 - 0xCF

        { &Cpu::CPY, &Cpu::IMM  }, // 0xC0
        { &Cpu::CMP, &Cpu::INDX }, // 0xC1
        { &Cpu::NOP, &Cpu::IMM  }, // 0xC2 *
        { &Cpu::DCP, &Cpu::INDX }, // 0xC3 *
        { &Cpu::CPY, &Cpu::ZPG  }, // 0xC4
        { &Cpu::CMP, &Cpu::ZPG  }, // 0xC5
        { &Cpu::DEC, &Cpu::ZPG  }, // 0xC6
        { &Cpu::DCP, &Cpu::ZPG  }, // 0xC7 *
        { &Cpu::INY, &Cpu::IMP  }, // 0xC8
        { &Cpu::CMP, &Cpu::IMM  }, // 0xC9
        { &Cpu::DEX, &Cpu::IMP  }, // 0xCA
        { &Cpu::SBX, &Cpu::IMM  }, // 0xCB *
        { &Cpu::CPY, &Cpu::ABS  }, // 0xCC
        { &Cpu::CMP, &Cpu::ABS  }, // 0xCD
        { &Cpu::DEC, &Cpu::ABS  }, // 0xCE
        { &Cpu::DCP, &Cpu::ABS  }, // 0xCF *

        // 0xD0 - 0xDF

        { &Cpu::BNE, &Cpu::REL  }, // 0xD0
        { &Cpu::CMP, &Cpu::INDY }, // 0xD1
        { &Cpu::JAM, &Cpu::IMP  }, // 0xD2 *
        { &Cpu::DCP, &Cpu::INDY }, // 0xD3 *
        { &Cpu::NOP, &Cpu::ZPGX }, // 0xD4 *
        { &Cpu::CMP, &Cpu::ZPGX }, // 0xD5
        { &Cpu::DEC, &Cpu::ZPGX }, // 0xD6
        { &Cpu::DCP, &Cpu::ZPGX }, // 0xD7 *
        { &Cpu::CLD, &Cpu::IMP  }, // 0xD8
        { &Cpu::CMP, &Cpu::ABSY }, // 0xD9
        { &Cpu::NOP, &Cpu::IMP  }, // 0xDA *
        { &Cpu::DCP, &Cpu::ABSY }, // 0xDB *
        { &Cpu::NOP, &Cpu::ABSX }, // 0xDC *
        { &Cpu::CMP, &Cpu::ABSX }, // 0xDD
        { &Cpu::DEC, &Cpu::ABSX }, // 0xDE
        { &Cpu::DCP, &Cpu::ABSX }, // 0xDF *

        // 0xE0 - 0xEF

        { &Cpu::CPX,  &Cpu::IMM  }, // 0xE0
        { &Cpu::SBC,  &Cpu::INDX }, // 0xE1
        { &Cpu::NOP,  &Cpu::IMM  }, // 0xE2 *
        { &Cpu::ISC,  &Cpu::INDX }, // 0xE3 *
        { &Cpu::CPX,  &Cpu::ZPG  }, // 0xE4
        { &Cpu::SBC,  &Cpu::ZPG  }, // 0xE5
        { &Cpu::INC,  &Cpu::ZPG  }, // 0xE6
        { &Cpu::ISC,  &Cpu::ZPG  }, // 0xE7 *
        { &Cpu::INX,  &Cpu::IMP  }, // 0xE8
        { &Cpu::SBC,  &Cpu::IMM  }, // 0xE9
        { &Cpu::NOP,  &Cpu::IMP  }, // 0xEA
        { &Cpu::USBC, &Cpu::IMM  }, // 0xEB *
        { &Cpu::CPX,  &Cpu::ABS  }, // 0xEC
        { &Cpu::SBC,  &Cpu::ABS  }, // 0xED
        { &Cpu::INC,  &Cpu::ABS  }, // 0xEE
        { &Cpu::ISC,  &Cpu::ABS  }, // 0xEF *

        // 0xF0 - 0xFF

        { &Cpu::BEQ, &Cpu::REL  }, // 0xF0
        { &Cpu::SBC, &Cpu::INDY }, // 0xF1
        { &Cpu::JAM, &Cpu::IMP  }, // 0xF2 *
        { &Cpu::ISC, &Cpu::INDY }, // 0xF3 *
        { &Cpu::NOP, &Cpu::ZPGX }, // 0xF4 *
        { &Cpu::SBC, &Cpu::ZPGX }, // 0xF5
        { &Cpu::INC, &Cpu::ZPGX }, // 0xF6
        { &Cpu::ISC, &Cpu::ZPGX }, // 0xF7 *
        { &Cpu::SED, &Cpu::IMP  }, // 0xF8
        { &Cpu::SBC, &Cpu::ABSY }, // 0xF9
        { &Cpu::NOP, &Cpu::IMP  }, // 0xFA *
        { &Cpu::ISC, &Cpu::ABSY }, // 0xFB *
        { &Cpu::NOP, &Cpu::ABSX }, // 0xFC *
        { &Cpu::SBC, &Cpu::ABSX }, // 0xFD
        { &Cpu::INC, &Cpu::ABSX }, // 0xFE
        { &Cpu::ISC, &Cpu::ABSX }  // 0xFF *
    }};

    #pragma endregion instruction
}

int Cpu::clock ()
{
    auto code = bus -> read(pc);  
    auto oper = cmd.at(code); 

    pc++;

    // Execute command and returns programm cycles
    return oper.execute(this);
}

void Cpu::reset ()
{
    x = 0x00;
    y = 0x00;
    p = 0x20;
    s = 0x00;
    a = 0x00;

    pc = 0x0000;
}

uint8_t Cpu::IMM  () { return 0x00; }
uint8_t Cpu::ABS  () { return 0x00; }
uint8_t Cpu::ABSX () { return 0x00; }
uint8_t Cpu::ABSY () { return 0x00; }
uint8_t Cpu::ZPG  () { return 0x00; }
uint8_t Cpu::ZPGX () { return 0x00; }
uint8_t Cpu::ZPGY () { return 0x00; }
uint8_t Cpu::IMP  () { return 0x00; }
uint8_t Cpu::ACC  () { return 0x00; }
uint8_t Cpu::IND  () { return 0x00; }
uint8_t Cpu::INDX () { return 0x00; }
uint8_t Cpu::INDY () { return 0x00; }
uint8_t Cpu::REL  () { return 0x00; }


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