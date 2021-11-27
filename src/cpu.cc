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

Cpu::Cpu()
{
    commands = 
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
    }}; 
}