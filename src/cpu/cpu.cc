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

#include "log.h"
#include "cmd.h"

#include "cpu/cpu.h"
#include "cpu/map.h"
#include "cpu/mem.h"
#include "bus/bus.h"


/*
    Default constructor
*/

Cpu::Cpu(std::shared_ptr<Bus> bus)
{
    map = std::make_unique<Map>();
    log = std::make_unique<Log>(bus);
    mem = std::make_unique<Mem>(bus);
}


/*
    Default destructor
*/
Cpu::~Cpu() = default;


/*
    Read data from memory/accumulator
*/
uint8_t Cpu::read() const
{
    if (cmd -> isAcc())
        return a;

    return mem -> read(pc);
}


/* 
    Write data to memory or accumulator 
*/
void Cpu::write (uint8_t data)
{
    if (cmd -> isAcc()) {
        a = data;
    } else {
        mem -> write(pc, data);
    }
}


/*
    Read operation code and execute command
    Returns total programm cycles per operation
*/

void Cpu::clock ()
{
    auto temp = pc;

    auto code = mem -> read(pc++);  
    auto oper = map -> getCommand(code);

    cmd = &oper;

    // Execute command and returns programm cycles
    oper.execute(this);

    // Disassembled output
    log -> step(temp, oper, this);
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
    // OPC #$BB
    // Operand is byte BB

    op = pc++;
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
    // OPC $LLHH	
    // Operand is address $HHLL

    op = mem -> abs(pc);
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
    // OPC $LLHH,X	
    // Operand is address; 
    // Effective address is address incremented by X with carry

    op = mem -> abs(pc, x);
}


void Cpu::ABSY () 
{ 
    // OPC $LLHH,Y	
    // Operand is address; 
    // Effective address is address incremented by Y with carry

    op = mem -> abs(pc, y);
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
    // OPC $LL
    // Operand is zeropage address (hi-byte is zero, address = $00LL)

    op = mem -> zpg(pc);
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

void Cpu::ZPGX () 
{ 
    // OPC $LL,X	
    // Operand is zeropage address; 
    // Effective address is address incremented by X without carry

    op = mem -> zpg(pc, x);
}


void Cpu::ZPGY () 
{ 
    // OPC $LL,Y	
    // Operand is zeropage address; 
    // Effective address is address incremented by Y without carry

    op = mem -> zpg(pc, y);
}


/* 
    Implied Addressing (Implied)

    In the implied addressing mode, the address containing the
    operand is implicitly stated in the operation code of the
    instruction. 
*/

void Cpu::IMP () 
{
    
}


/* 
    Accumulator Addressing (Accum)

    This form of addressing is represented with a one byte
    instruction and implies an operation on the accumulator. 
*/

void Cpu::ACC () 
{ 
    // OPC A	
    // Operand is AC (implied single byte instruction)

    op = a;
}


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

void Cpu::IND () 
{
    // OPC ($LLHH)	
    // Operand is address; 
    // Effective address is contents of word at address: C.w($HHLL)

    op = mem -> indirect(pc);
}


/* 
    Absolute Indexed Indirect Addressing (Jump Instruction Only)

    With absolute indexed indirect addressing the contents of
    the second and third instruction bytes are added to the X
    register. The result of this addition, points to a memory loca-
    tion containing the lower-order eight bits of the effective
    address. The next memory location contains the higher-
    order eight bits of the effective address. 
*/

void Cpu::INDX () 
{ 
    // OPC ($LL,X)	
    // Operand is zeropage address; 
    // Effective address is word in (LL + X, LL + X + 1), inc. without carry: C.w($00LL + X)

    op = mem -> indexed(pc, x);
}


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

void Cpu::INDY () 
{
    // OPC ($LL),Y	
    // Operand is zeropage address; 
    // Effective address is word in (LL, LL + 1) incremented by Y with carry: C.w($00LL) + Y

    auto index = mem -> indexed(pc);
    op = 0x00FF & (index + y);
}


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

void Cpu::REL () 
{ 
    IMM();
}


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

void Cpu::ADC (uint8_t arg)
{
    uint16_t sum = a + arg + p.getCarry();
    a = (uint8_t) sum;

    if (p.isDecimal())
    {
        // BCD mode
    }

    p.setNegative (sum);
    p.setZero     (sum);
    p.setCarry    (sum);
}


void Cpu::ADC() 
{ 
    auto data = read();
    ADC(data); 
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

void Cpu::ALR() 
{
    NOP(); 
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

void Cpu::ANC() 
{ 
    NOP(); 
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

void Cpu::AND() 
{ 
    a &= read();

    p.setZero(a);
    p.setNegative(a);
}


/*
    ANE (XAA)
    * AND X + AND oper

    Highly unstable, do not use.

    A base value in A is determined based on the contets of A 
    and a constant, which may be typically $00, $ff, $ee, etc. 
    The value of this constant depends on temerature, the chip 
    series, and maybe other factors, as well.
    In order to eliminate these uncertaincies from the 
    equation, use either 0 as the operand or a value of $FF in 
    the accumulator.

    (A OR CONST) AND X AND oper -> A      N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | immediate  | ANE #oper | 8B  | 2     | 2      |
    +------------+-----------+-----+-------+--------+    
*/

void Cpu::ANE() 
{
    NOP();
}


/*
    ARR
    AND oper + ROR

    This operation involves the adder:
    V-flag is set according to (A AND oper) + oper
    The carry is not set, but bit 7 (sign) is exchanged with the carry

    A AND oper, C -> [76543210] -> C      N Z C I D V
                                          + + + - - +
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | immediate  | ARR #oper | 6B  | 2     | 2      |
    +------------+-----------+-----+-------+--------+
*/

void Cpu::ARR() 
{  
    NOP();
} 


/*
    ASL
    Shift Left One Bit (Memory or Accumulator)

    C <- [76543210] <- 0                    N Z C I D V
                                            + + + - - -
    +-------------+------------+-----+-------+--------+
    | addressing  | assembler  | opc | bytes | cycles |
    +-------------+------------+-----+-------+--------+
    | accumulator | ASL A      | 0A  | 1     | 2      |
    | zeropage    | ASL oper   | 06  | 2     | 5      |
    | zeropage,X  | ASL oper,X | 16  | 2     | 6      |
    | absolute    | ASL oper   | 0E  | 3     | 6      |
    | absolute,X  | ASL oper,X | 1E  | 3     | 7      |
    +-------------+------------+-----+-------+--------+
*/

void Cpu::ASL() 
{
    uint16_t data  = read();
    uint16_t shift = data << 1;

    p.setNegative (shift);
    p.setZero     (shift);
    p.setCarry    (shift);

    write(shift);
}


void Cpu::BCC() { }
void Cpu::BCS() { }
void Cpu::BEQ() { }
void Cpu::BIT() { }
void Cpu::BMI() { }
void Cpu::BNE() { }
void Cpu::BPL() { }
void Cpu::BRK() { }
void Cpu::BVC() { }
void Cpu::BVS() { }
void Cpu::CLC() { }
void Cpu::CLD() { }
void Cpu::CLI() { }
void Cpu::CLV() { }
void Cpu::CMP() { }
void Cpu::CPX() { }
void Cpu::CPY() { }
void Cpu::DCP() { }
void Cpu::DEC() { }
void Cpu::DEX() { }
void Cpu::DEY() { }
void Cpu::EOR() { }
void Cpu::INC() { }
void Cpu::INX() { }
void Cpu::INY() { }
void Cpu::ISC() { }
void Cpu::JAM() { }
void Cpu::JMP() { }
void Cpu::JSR() { }
void Cpu::LAS() { }
void Cpu::LAX() { }
void Cpu::LDA() { }
void Cpu::LDX() { }
void Cpu::LDY() { }
void Cpu::LSR() { }
void Cpu::LXA() { }
void Cpu::NOP() { }
void Cpu::ORA() { }
void Cpu::PHA() { }
void Cpu::PHP() { }
void Cpu::PLA() { }
void Cpu::PLP() { }
void Cpu::RLA() { }
void Cpu::ROL() { }
void Cpu::ROR() { }
void Cpu::RRA() { }
void Cpu::RTI() { }
void Cpu::RTS() { }
void Cpu::SAX() { }


/*
    SBC
    Subtract Memory from Accumulator with Borrow

    A - M - ~C -> A                            N Z C I D V
                                               + + + - - +
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | immediate    | SBC #oper    | E9  | 2     | 2      |
    | zeropage     | SBC oper     | E5  | 2     | 3      |
    | zeropage,X   | SBC oper,X   | F5  | 2     | 4      |
    | absolute     | SBC oper     | ED  | 3     | 4      |
    | absolute,X   | SBC oper,X   | FD  | 3     | 4*     |
    | absolute,Y   | SBC oper,Y   | F9  | 3     | 4*     |
    | (indirect,X) | SBC (oper,X) | E1  | 2     | 6      |
    | (indirect),Y | SBC (oper),Y | F1  | 2     | 5*     |
    +--------------+--------------+-----+-------+--------+
*/

void Cpu::SBC() 
{ 
    auto data = read();
    ADC(~data); 
}

void Cpu::SBX() { }
void Cpu::SEC() { }
void Cpu::SED() { }
void Cpu::SEI() { }
void Cpu::SHA() { }
void Cpu::SHY() { }
void Cpu::SHX() { }
void Cpu::SLO() { }
void Cpu::SRE() { }
void Cpu::STA() { }
void Cpu::STX() { }
void Cpu::STY() { }
void Cpu::TAS() { }
void Cpu::TAX() { }
void Cpu::TAY() { }
void Cpu::TSX() { }
void Cpu::TXA() { }
void Cpu::TXS() { }
void Cpu::TYA() { }
void Cpu::USB() { }