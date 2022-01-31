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

#include "map.h"
#include "cpu.h"

//
// Command mapping
// Asterics means illegal operation code
//

Map::Map()
{
    cmd =
    {{
        // 0x00 - 0x0F

        { "BRK", 7, &Cpu::BRK, &Cpu::IMP  }, // 0x00
        { "ORA", 6, &Cpu::ORA, &Cpu::INDX }, // 0x01
        { "JAM", 1, &Cpu::JAM, &Cpu::IMP  }, // 0x02 *
        { "SLO", 8, &Cpu::SLO, &Cpu::INDX }, // 0x03 *
        { "NOP", 3, &Cpu::NOP, &Cpu::ZPG  }, // 0x04 *
        { "ORA", 3, &Cpu::ORA, &Cpu::ZPG  }, // 0x05
        { "ASL", 5, &Cpu::ASL, &Cpu::ZPG  }, // 0x06
        { "SLO", 5, &Cpu::SLO, &Cpu::ZPG  }, // 0x07 *
        { "PHP", 3, &Cpu::PHP, &Cpu::IMP  }, // 0x08
        { "ORA", 2, &Cpu::ORA, &Cpu::IMM  }, // 0x09
        { "ASL", 2, &Cpu::ASL, &Cpu::ACC  }, // 0x0A
        { "ANC", 2, &Cpu::ANC, &Cpu::IMM  }, // 0x0B *
        { "NOP", 4, &Cpu::NOP, &Cpu::ABS  }, // 0x0C *
        { "ORA", 4, &Cpu::ORA, &Cpu::ABS  }, // 0x0D
        { "ASL", 6, &Cpu::ASL, &Cpu::ABS  }, // 0x0E
        { "SLO", 6, &Cpu::SLO, &Cpu::ABS  }, // 0x0F *


        // 0x10 - 0x1F

        { "BPL", 2, &Cpu::BPL, &Cpu::REL  }, // 0x10
        { "ORA", 5, &Cpu::ORA, &Cpu::INDY }, // 0x11
        { "JAM", 1, &Cpu::JAM, &Cpu::IMP  }, // 0x12 *
        { "SLO", 8, &Cpu::SLO, &Cpu::INDY }, // 0x13 *
        { "NOP", 4, &Cpu::NOP, &Cpu::ZPGX }, // 0x14 *
        { "ORA", 5, &Cpu::ORA, &Cpu::ZPGX }, // 0x15
        { "ASL", 6, &Cpu::ASL, &Cpu::ZPGX }, // 0x16
        { "SLO", 6, &Cpu::SLO, &Cpu::ZPGX }, // 0x17 *
        { "CLC", 2, &Cpu::CLC, &Cpu::IMP  }, // 0x18
        { "ORA", 4, &Cpu::ORA, &Cpu::ABSY }, // 0x19
        { "NOP", 2, &Cpu::NOP, &Cpu::IMP  }, // 0x1A *
        { "SLO", 7, &Cpu::SLO, &Cpu::ABSY }, // 0x1B *
        { "NOP", 4, &Cpu::NOP, &Cpu::ABSX }, // 0x1C *
        { "ORA", 4, &Cpu::ORA, &Cpu::ABSX }, // 0x1D
        { "ASL", 7, &Cpu::ASL, &Cpu::ABSX }, // 0x1E
        { "SLO", 7, &Cpu::SLO, &Cpu::ABSX }, // 0x1F *


        // 0x20 - 0x2F

        { "JSR", 6, &Cpu::JSR, &Cpu::ABS  }, // 0X20
        { "AND", 6, &Cpu::AND, &Cpu::INDX }, // 0X21
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0X22 *
        { "RLA", 8, &Cpu::RLA, &Cpu::INDX }, // 0X23 *
        { "BIT", 3, &Cpu::BIT, &Cpu::ZPG  }, // 0X24
        { "AND", 3, &Cpu::AND, &Cpu::ZPG  }, // 0X25
        { "ROL", 5, &Cpu::ROL, &Cpu::ZPG  }, // 0X26
        { "RLA", 5, &Cpu::RLA, &Cpu::ZPG  }, // 0X27 *
        { "PLP", 4, &Cpu::PLP, &Cpu::IMP  }, // 0X28
        { "AND", 2, &Cpu::AND, &Cpu::IMM  }, // 0X29
        { "ROL", 2, &Cpu::ROL, &Cpu::ACC  }, // 0X2A
        { "ANC", 2, &Cpu::ANC, &Cpu::IMM  }, // 0X2B *
        { "BIT", 4, &Cpu::BIT, &Cpu::ABS  }, // 0X2C
        { "AND", 4, &Cpu::AND, &Cpu::ABS  }, // 0X2D
        { "ROL", 6, &Cpu::ROL, &Cpu::ABS  }, // 0X2E
        { "RLA", 6, &Cpu::RLA, &Cpu::ABS  }, // 0X2F *


        // 0x30 - 0x3F

        { "BMI", 2, &Cpu::BMI, &Cpu::REL  }, // 0x30
        { "AND", 5, &Cpu::AND, &Cpu::INDY }, // 0x31
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0x32 *
        { "RLA", 8, &Cpu::RLA, &Cpu::INDY }, // 0x33 *
        { "NOP", 4, &Cpu::NOP, &Cpu::ZPGX }, // 0x34 *
        { "AND", 4, &Cpu::AND, &Cpu::ZPGX }, // 0x35
        { "ROL", 6, &Cpu::ROL, &Cpu::ZPGX }, // 0x36
        { "RLA", 6, &Cpu::RLA, &Cpu::ZPGX }, // 0x37 *
        { "SEC", 2, &Cpu::SEC, &Cpu::IMP  }, // 0x38
        { "AND", 4, &Cpu::AND, &Cpu::ABSY }, // 0x39
        { "NOP", 2, &Cpu::NOP, &Cpu::IMP  }, // 0x3A *
        { "RLA", 7, &Cpu::RLA, &Cpu::ABSY }, // 0x3B *
        { "NOP", 4, &Cpu::NOP, &Cpu::ABSX }, // 0x3C *
        { "AND", 4, &Cpu::AND, &Cpu::ABSX }, // 0x3D
        { "ROL", 7, &Cpu::ROL, &Cpu::ABSX }, // 0x3E
        { "RLA", 7, &Cpu::RLA, &Cpu::ABSX }, // 0x3F *


        // 0x40 - 0x4F

        { "RTI", 6, &Cpu::RTI, &Cpu::IMP  }, // 0x40
        { "EOR", 6, &Cpu::EOR, &Cpu::INDX }, // 0x41
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0x42 *
        { "SRE", 8, &Cpu::SRE, &Cpu::INDX }, // 0x43 *
        { "NOP", 3, &Cpu::NOP, &Cpu::ZPG  }, // 0x44 *
        { "EOR", 3, &Cpu::EOR, &Cpu::ZPG  }, // 0x45
        { "LSR", 5, &Cpu::LSR, &Cpu::ZPG  }, // 0x46
        { "SRE", 5, &Cpu::SRE, &Cpu::ZPG  }, // 0x47 *
        { "PHA", 3, &Cpu::PHA, &Cpu::IMP  }, // 0x48
        { "EOR", 2, &Cpu::EOR, &Cpu::IMM  }, // 0x49
        { "LSR", 2, &Cpu::LSR, &Cpu::ACC  }, // 0x4A
        { "ALR", 2, &Cpu::ALR, &Cpu::IMM  }, // 0x4B *
        { "JMP", 3, &Cpu::JMP, &Cpu::ABS  }, // 0x4C
        { "EOR", 4, &Cpu::EOR, &Cpu::ABS  }, // 0x4D
        { "LSR", 6, &Cpu::LSR, &Cpu::ABS  }, // 0x4E
        { "SRE", 6, &Cpu::SRE, &Cpu::ABS  }, // 0x4F *


        // 0x50 - 0x5F

        { "BVC", 2, &Cpu::BVC, &Cpu::REL  }, // 0x50
        { "EOR", 5, &Cpu::EOR, &Cpu::INDY }, // 0x51
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0x52 *
        { "SRE", 8, &Cpu::SRE, &Cpu::INDY }, // 0x53 *
        { "NOP", 4, &Cpu::NOP, &Cpu::ZPGX }, // 0x54 *
        { "EOR", 4, &Cpu::EOR, &Cpu::ZPGX }, // 0x55
        { "LSR", 6, &Cpu::LSR, &Cpu::ZPGX }, // 0x56
        { "SRE", 6, &Cpu::SRE, &Cpu::ZPGX }, // 0x57 *
        { "CLI", 2, &Cpu::CLI, &Cpu::IMP  }, // 0x58
        { "EOR", 4, &Cpu::EOR, &Cpu::ABSY }, // 0x59
        { "NOP", 2, &Cpu::NOP, &Cpu::IMP  }, // 0x5A *
        { "SRE", 7, &Cpu::SRE, &Cpu::ABSY }, // 0x5B *
        { "NOP", 4, &Cpu::NOP, &Cpu::ABSX }, // 0x5C *
        { "EOR", 4, &Cpu::EOR, &Cpu::ABSX }, // 0x5D
        { "LSR", 7, &Cpu::LSR, &Cpu::ABSX }, // 0x5E
        { "SRE", 7, &Cpu::SRE, &Cpu::ABSX }, // 0x5F *


        // 0x60 - 0x6F

        { "RTS", 6, &Cpu::RTS, &Cpu::IMP  }, // 0x60
        { "ADC", 6, &Cpu::ADC, &Cpu::INDX }, // 0x61
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0x62 *
        { "RRA", 8, &Cpu::RRA, &Cpu::INDX }, // 0x63 *
        { "NOP", 3, &Cpu::NOP, &Cpu::ZPG  }, // 0x64 *
        { "ADC", 3, &Cpu::ADC, &Cpu::ZPG  }, // 0x65
        { "ROR", 5, &Cpu::ROR, &Cpu::ZPG  }, // 0x66
        { "RRA", 5, &Cpu::RRA, &Cpu::ZPG  }, // 0x67 *
        { "PLA", 4, &Cpu::PLA, &Cpu::IMP  }, // 0x68
        { "ADC", 2, &Cpu::ADC, &Cpu::IMM  }, // 0x69
        { "ROR", 2, &Cpu::ROR, &Cpu::ACC  }, // 0x6A
        { "ARR", 2, &Cpu::ARR, &Cpu::IMM  }, // 0x6B *
        { "JMP", 5, &Cpu::JMP, &Cpu::IND  }, // 0x6C
        { "ADC", 4, &Cpu::ADC, &Cpu::ABS  }, // 0x6D
        { "ROR", 6, &Cpu::ROR, &Cpu::ABS  }, // 0x6E
        { "RRA", 6, &Cpu::RRA, &Cpu::ABS  }, // 0x6F *


        // 0x70 - 0x7F

        { "BVS", 2, &Cpu::BVS, &Cpu::REL  }, // 0x70
        { "ADC", 5, &Cpu::ADC, &Cpu::INDY }, // 0x71
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0x72 *
        { "RRA", 8, &Cpu::RRA, &Cpu::INDY }, // 0x73 *
        { "NOP", 4, &Cpu::NOP, &Cpu::ZPGX }, // 0x74 *
        { "ADC", 4, &Cpu::ADC, &Cpu::ZPGX }, // 0x75
        { "ROR", 6, &Cpu::ROR, &Cpu::ZPGX }, // 0x76
        { "RRA", 6, &Cpu::RRA, &Cpu::ZPGX }, // 0x77 *
        { "SEI", 2, &Cpu::SEI, &Cpu::IMP  }, // 0x78
        { "ADC", 4, &Cpu::ADC, &Cpu::ABSY }, // 0x79
        { "NOP", 2, &Cpu::NOP, &Cpu::IMP  }, // 0x7A *
        { "RRA", 7, &Cpu::RRA, &Cpu::ABSY }, // 0x7B *
        { "NOP", 4, &Cpu::NOP, &Cpu::ABSX }, // 0x7C *
        { "ADC", 4, &Cpu::ADC, &Cpu::ABSX }, // 0x7D
        { "ROR", 7, &Cpu::ROR, &Cpu::ABSX }, // 0x7E
        { "RRA", 7, &Cpu::RRA, &Cpu::ABSX }, // 0x7F *


        // 0x80 - 0x8F

        { "NOP", 2, &Cpu::NOP, &Cpu::IMM  }, // 0x80 *
        { "STA", 6, &Cpu::STA, &Cpu::INDX }, // 0x81
        { "NOP", 2, &Cpu::NOP, &Cpu::IMM  }, // 0x82 *
        { "SAX", 6, &Cpu::SAX, &Cpu::INDX }, // 0x83 *
        { "STY", 3, &Cpu::STY, &Cpu::ZPG  }, // 0x84
        { "STA", 3, &Cpu::STA, &Cpu::ZPG  }, // 0x85
        { "STX", 3, &Cpu::STX, &Cpu::ZPG  }, // 0x86
        { "SAX", 3, &Cpu::SAX, &Cpu::ZPG  }, // 0x87 *
        { "DEY", 2, &Cpu::DEY, &Cpu::IMP  }, // 0x88
        { "NOP", 2, &Cpu::NOP, &Cpu::IMM  }, // 0x89 *
        { "TXA", 2, &Cpu::TXA, &Cpu::IMP  }, // 0x8A
        { "ANE", 2, &Cpu::ANE, &Cpu::IMM  }, // 0x8B *
        { "STY", 4, &Cpu::STY, &Cpu::ABS  }, // 0x8C
        { "STA", 4, &Cpu::STA, &Cpu::ABS  }, // 0x8D
        { "STX", 4, &Cpu::STX, &Cpu::ABS  }, // 0x8E
        { "SAX", 4, &Cpu::SAX, &Cpu::ABS  }, // 0x8F *


        // 0x90 - 0x9F

        { "BCC", 2, &Cpu::BCC, &Cpu::REL  }, // 0x90
        { "STA", 6, &Cpu::STA, &Cpu::INDY }, // 0x91
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0x92 *
        { "SHA", 6, &Cpu::SHA, &Cpu::INDY }, // 0x93 *
        { "STY", 4, &Cpu::STY, &Cpu::ZPGX }, // 0x94
        { "STA", 4, &Cpu::STA, &Cpu::ZPGX }, // 0x95
        { "STX", 4, &Cpu::STX, &Cpu::ZPGY }, // 0x96
        { "SAX", 4, &Cpu::SAX, &Cpu::ZPGY }, // 0x97 *
        { "TYA", 2, &Cpu::TYA, &Cpu::IMP  }, // 0x98
        { "STA", 5, &Cpu::STA, &Cpu::ABSY }, // 0x99
        { "TXS", 2, &Cpu::TXS, &Cpu::IMP  }, // 0x9A
        { "TAS", 5, &Cpu::TAS, &Cpu::ABSY }, // 0x9B *
        { "SHY", 5, &Cpu::SHY, &Cpu::ABSX }, // 0x9C *
        { "STA", 5, &Cpu::STA, &Cpu::ABSX }, // 0x9D
        { "SHX", 5, &Cpu::SHX, &Cpu::ABSY }, // 0x9E *
        { "SHA", 5, &Cpu::SHA, &Cpu::ABSY }, // 0x9F *


        // 0xA0 - 0xAF

        { "LDY", 2, &Cpu::LDY, &Cpu::IMM  }, // 0xA0
        { "LDA", 6, &Cpu::LDA, &Cpu::INDX }, // 0xA1
        { "LDX", 2, &Cpu::LDX, &Cpu::IMM  }, // 0xA2
        { "LAX", 6, &Cpu::LAX, &Cpu::INDX }, // 0xA3 *
        { "LDY", 3, &Cpu::LDY, &Cpu::ZPG  }, // 0xA4
        { "LDA", 3, &Cpu::LDA, &Cpu::ZPG  }, // 0xA5
        { "LDX", 3, &Cpu::LDX, &Cpu::ZPG  }, // 0xA6
        { "LAX", 3, &Cpu::LAX, &Cpu::ZPG  }, // 0xA7 *
        { "TAY", 2, &Cpu::TAY, &Cpu::IMP  }, // 0xA8
        { "LDA", 2, &Cpu::LDA, &Cpu::IMM  }, // 0xA9
        { "TAX", 2, &Cpu::TAX, &Cpu::IMP  }, // 0xAA
        { "LXA", 2, &Cpu::LXA, &Cpu::IMM  }, // 0xAB *
        { "LDY", 4, &Cpu::LDY, &Cpu::ABS  }, // 0xAC
        { "LDA", 4, &Cpu::LDA, &Cpu::ABS  }, // 0xAD
        { "LDX", 4, &Cpu::LDX, &Cpu::ABS  }, // 0xAE
        { "LAX", 4, &Cpu::LAX, &Cpu::ABS  }, // 0xAF *


        // 0xB0 - 0xBF

        { "BCS", 2, &Cpu::BCS, &Cpu::REL  }, // 0xB0
        { "LDA", 5, &Cpu::LDA, &Cpu::INDY }, // 0xB1
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0xB2 *
        { "LAX", 5, &Cpu::LAX, &Cpu::INDY }, // 0xB3 *
        { "LDY", 4, &Cpu::LDY, &Cpu::ZPGX }, // 0xB4
        { "LDA", 4, &Cpu::LDA, &Cpu::ZPGX }, // 0xB5
        { "LDX", 4, &Cpu::LDX, &Cpu::ZPGY }, // 0xB6
        { "LAX", 4, &Cpu::LAX, &Cpu::ZPGY }, // 0xB7 *
        { "CLV", 2, &Cpu::CLV, &Cpu::IMP  }, // 0xB8
        { "LDA", 4, &Cpu::LDA, &Cpu::ABSY }, // 0xB9
        { "TSX", 2, &Cpu::TSX, &Cpu::IMP  }, // 0xBA
        { "LAS", 4, &Cpu::LAS, &Cpu::ABSY }, // 0xBB *
        { "LDY", 4, &Cpu::LDY, &Cpu::ABSX }, // 0xBC
        { "LDA", 4, &Cpu::LDA, &Cpu::ABSX }, // 0xBD
        { "LDX", 4, &Cpu::LDX, &Cpu::ABSY }, // 0xBE
        { "LAX", 4, &Cpu::LAX, &Cpu::ABSY }, // 0xBF *


        // 0xC0 - 0xCF

        { "CPY", 2, &Cpu::CPY, &Cpu::IMM  }, // 0xC0
        { "CMP", 6, &Cpu::CMP, &Cpu::INDX }, // 0xC1
        { "NOP", 2, &Cpu::NOP, &Cpu::IMM  }, // 0xC2 *
        { "DCP", 8, &Cpu::DCP, &Cpu::INDX }, // 0xC3 *
        { "CPY", 3, &Cpu::CPY, &Cpu::ZPG  }, // 0xC4
        { "CMP", 3, &Cpu::CMP, &Cpu::ZPG  }, // 0xC5
        { "DEC", 5, &Cpu::DEC, &Cpu::ZPG  }, // 0xC6
        { "DCP", 5, &Cpu::DCP, &Cpu::ZPG  }, // 0xC7 *
        { "INY", 2, &Cpu::INY, &Cpu::IMP  }, // 0xC8
        { "CMP", 2, &Cpu::CMP, &Cpu::IMM  }, // 0xC9
        { "DEX", 2, &Cpu::DEX, &Cpu::IMP  }, // 0xCA
        { "SBX", 2, &Cpu::SBX, &Cpu::IMM  }, // 0xCB *
        { "CPY", 4, &Cpu::CPY, &Cpu::ABS  }, // 0xCC
        { "CMP", 4, &Cpu::CMP, &Cpu::ABS  }, // 0xCD
        { "DEC", 6, &Cpu::DEC, &Cpu::ABS  }, // 0xCE
        { "DCP", 6, &Cpu::DCP, &Cpu::ABS  }, // 0xCF *


        // 0xD0 - 0xDF

        { "BNE", 2, &Cpu::BNE, &Cpu::REL  }, // 0xD0
        { "CMP", 5, &Cpu::CMP, &Cpu::INDY }, // 0xD1
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0xD2 *
        { "DCP", 8, &Cpu::DCP, &Cpu::INDY }, // 0xD3 *
        { "NOP", 4, &Cpu::NOP, &Cpu::ZPGX }, // 0xD4 *
        { "CMP", 4, &Cpu::CMP, &Cpu::ZPGX }, // 0xD5
        { "DEC", 6, &Cpu::DEC, &Cpu::ZPGX }, // 0xD6
        { "DCP", 6, &Cpu::DCP, &Cpu::ZPGX }, // 0xD7 *
        { "CLD", 2, &Cpu::CLD, &Cpu::IMP  }, // 0xD8
        { "CMP", 4, &Cpu::CMP, &Cpu::ABSY }, // 0xD9
        { "NOP", 2, &Cpu::NOP, &Cpu::IMP  }, // 0xDA *
        { "DCP", 7, &Cpu::DCP, &Cpu::ABSY }, // 0xDB *
        { "NOP", 4, &Cpu::NOP, &Cpu::ABSX }, // 0xDC *
        { "CMP", 4, &Cpu::CMP, &Cpu::ABSX }, // 0xDD
        { "DEC", 7, &Cpu::DEC, &Cpu::ABSX }, // 0xDE
        { "DCP", 7, &Cpu::DCP, &Cpu::ABSX }, // 0xDF *


        // 0xE0 - 0xEF

        { "CPX", 2, &Cpu::CPX, &Cpu::IMM  }, // 0xE0
        { "SBC", 6, &Cpu::SBC, &Cpu::INDX }, // 0xE1
        { "NOP", 2, &Cpu::NOP, &Cpu::IMM  }, // 0xE2 *
        { "ISC", 8, &Cpu::ISC, &Cpu::INDX }, // 0xE3 *
        { "CPX", 3, &Cpu::CPX, &Cpu::ZPG  }, // 0xE4
        { "SBC", 3, &Cpu::SBC, &Cpu::ZPG  }, // 0xE5
        { "INC", 5, &Cpu::INC, &Cpu::ZPG  }, // 0xE6
        { "ISC", 5, &Cpu::ISC, &Cpu::ZPG  }, // 0xE7 *
        { "INX", 2, &Cpu::INX, &Cpu::IMP  }, // 0xE8
        { "SBC", 2, &Cpu::SBC, &Cpu::IMM  }, // 0xE9
        { "NOP", 2, &Cpu::NOP, &Cpu::IMP  }, // 0xEA
        { "USB", 2, &Cpu::USB, &Cpu::IMM  }, // 0xEB *
        { "CPX", 4, &Cpu::CPX, &Cpu::ABS  }, // 0xEC
        { "SBC", 4, &Cpu::SBC, &Cpu::ABS  }, // 0xED
        { "INC", 6, &Cpu::INC, &Cpu::ABS  }, // 0xEE
        { "ISC", 6, &Cpu::ISC, &Cpu::ABS  }, // 0xEF *


        // 0xF0 - 0xFF

        { "BEQ", 2, &Cpu::BEQ, &Cpu::REL  }, // 0xF0
        { "SBC", 5, &Cpu::SBC, &Cpu::INDY }, // 0xF1
        { "JAM", 2, &Cpu::JAM, &Cpu::IMP  }, // 0xF2 *
        { "ISC", 8, &Cpu::ISC, &Cpu::INDY }, // 0xF3 *
        { "NOP", 4, &Cpu::NOP, &Cpu::ZPGX }, // 0xF4 *
        { "SBC", 4, &Cpu::SBC, &Cpu::ZPGX }, // 0xF5
        { "INC", 6, &Cpu::INC, &Cpu::ZPGX }, // 0xF6
        { "ISC", 6, &Cpu::ISC, &Cpu::ZPGX }, // 0xF7 *
        { "SED", 2, &Cpu::SED, &Cpu::IMP  }, // 0xF8
        { "SBC", 4, &Cpu::SBC, &Cpu::ABSY }, // 0xF9
        { "NOP", 2, &Cpu::NOP, &Cpu::IMP  }, // 0xFA *
        { "ISC", 7, &Cpu::ISC, &Cpu::ABSY }, // 0xFB *
        { "NOP", 4, &Cpu::NOP, &Cpu::ABSX }, // 0xFC *
        { "SBC", 4, &Cpu::SBC, &Cpu::ABSX }, // 0xFD
        { "INC", 7, &Cpu::INC, &Cpu::ABSX }, // 0xFE
        { "ISC", 7, &Cpu::ISC, &Cpu::ABSX }  // 0xFF *
    }};
}

const Cmd & Map::getCommand(uint8_t opcode) const{
    return cmd.at(opcode);
}