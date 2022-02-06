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

    return mem -> read(op);
}


/* 
    Write data to memory or accumulator 
*/

void Cpu::write (uint8_t data)
{
    if (cmd -> isAcc()) {
        a = data;
    } else {
        mem -> write(op, data);
    }
}


/*
    Read operation code and execute command
    Returns total programm cycles per operation
*/

void Cpu::clock ()
{
    counter++;

    auto temp = pc;

    auto code = mem -> read(pc++);  
    auto oper = map -> getCommand(code);

    cmd = &oper;

    // Execute command and returns programm cycles
    oper.execute(this);

    // Disassembled output
    if (counter > 26764002)
        log -> step(counter, temp, oper, this);
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

    op = mem -> indexed(pc) + y;
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
    Add Arg to Accumulator with Carry
*/
void Cpu::ADC (uint8_t arg)
{
    uint16_t crr = p.getCarry();
    uint16_t sum = (uint16_t) a + (uint16_t) arg + crr;

    p.setNegative (sum);
    p.setZero     (sum);
    p.setCarry    ((bool) (sum > 255));
    p.setOverflow ((bool) (~(a ^ arg) & (a ^ sum) & 0x80));

    a = 0x00FF & sum;
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
    uint8_t data  = read();
    uint8_t shift = data << 1;

    p.setNegative (shift);
    p.setZero     (shift);
    p.setCarry    ((bool) (data & 0x80));

    write(shift);
}


/*
    Branch 

    Branch offsets are signed 8-bit values, -128 ... +127, 
    negative offsets in two's complement.

    Page transitions may occur and add an extra cycle to the exucution.
*/
void Cpu::BRA()
{
    uint16_t rel = read();

    if (rel & 0x80) 
    {
		rel |= 0xFF00;
    }

    pc += rel;
}


/*
    BCC
    Branch on Carry Clear

    branch on C = 0                       N Z C I D V
                                          - - - - - - 
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | relative   | BCC oper  | 90  | 2     | 2**    |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BCC() 
{
    if (!p.isCarry())
    {
        BRA();
    }
}


/*
    BCC
    Branch on Carry Set

    branch on C = 1                       N Z C I D V
                                          - - - - - - 
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | relative   | BCS oper  | B0  | 2     | 2**    |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BCS() 
{
    if (p.isCarry())
    {
        BRA();
    }
}


/*
    BCC
    Branch on Result Zero

    branch on Z = 1                       N Z C I D V
                                          - - - - - - 
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | relative   | BEQ oper  | F0  | 2     | 2**    |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BEQ() 
{ 
    if (p.isZero())
    {
        BRA();
    }
}


/*
    BIT
    Test Bits in Memory with Accumulator

    bits 7 and 6 of operand are transfered to bit 7 and 6 of SR (N,V);
    the zero-flag is set to the result of operand AND accumulator.

    A AND M, M7 -> N, M6 -> V             N Z C I D V
                                         M7 + - - - M6
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | zeropage   | BIT oper  | 24  | 2     | 3      |
    | absolute   | BIT oper  | 2C  | 3     | 4      |
    +------------+-----------+-----+-------+--------+

*/
void Cpu::BIT() 
{
    auto data = read();

    p.setNegative ((bool) (data & 0x80));
    p.setOverflow ((bool) (data & 0x40));
    p.setZero     ( data & a );
}


/*
    BMI
    Branch on Result Minus

    branch on N = 1                       N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | relative   | BMI oper  | 30  | 2     | 2**    |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BMI() 
{
    if (p.isNegative())
    {
        BRA();
    }
}


/*
    BNE
    Branch on Result not Zero

    branch on Z = 0                       N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | relative   | BNE oper  | D0  | 2     | 2**    |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BNE() 
{
    if (!p.isZero())
    {
        BRA();
    }
}


/*
    BPL
    Branch on Result Plus

    branch on N = 0                       N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | relative   | BPL oper  | 10  | 2     | 2**    |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BPL() 
{
    if (!p.isNegative())
    {
        BRA();
    }
}


/*
    BRK
    Force Break

    BRK initiates a software interrupt similar to a hardware
    interrupt (IRQ). The return address pushed to the stack is
    PC+2, providing an extra byte of spacing for a break mark
    (identifying a reason for the break.)
    The status register will be pushed to the stack with the break
    flag set to 1. However, when retrieved during RTI or by a PLP
    instruction, the break flag will be ignored.
    The interrupt disable flag is not set automatically.

    interrupt,
    push PC+2, push SR                    N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | BRK       | 00  | 1     | 7      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BRK() 
{
    pc++;

    uint8_t lo =  pc & 0xFF;
    uint8_t hi = (pc >> 8) & 0xFF;

    // ~
    mem -> push(s, hi);
    mem -> push(s, lo);
    
    // ~
    mem -> push(s, p);

    pc = 0xFFFE;
    pc = mem -> direct(pc);

    p.setInterrupt (true);
    p.setBreak     (false);
}


/*
    BVC
    Branch on Overflow Clear

    branch on V = 0                       N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | relative   | BVC oper  | 50  | 2     | 2**    |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BVC() 
{ 
    if (!p.isOverflow())
    {
        BRA();
    }
}


/*
    BVS
    Branch on Overflow set

    branch on V = 1                       N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | relative   | BVS oper  | 70  | 2     | 2**    |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::BVS() 
{ 
    if (p.isOverflow())
    {
        BRA();
    }
}


/*
    CLC
    Clear Carry Flag

    0 -> C                                N Z C I D V
                                          - - 0 - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | CLC       | 18  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::CLC() 
{ 
    p.setCarry(false);
}


/*
    CLD
    Clear Decimal Mode

    0 -> D                                N Z C I D V
                                          - - - - 0 -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | CLD       | D8  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::CLD() 
{ 
    p.setDecimal(false);
}


/*
    CLI
    Clear Interrupt Disable Bit

    0 -> I                               N Z C I D V
                                         - - - 0 - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | CLI       | 58  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::CLI() 
{ 
    p.setInterrupt(false);
}


/*
    CLV
    Clear Overflow Flag

    0 -> V                               N Z C I D V
                                         - - - - - 0
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | CLV       | B8  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::CLV() 
{ 
    p.setOverflow(false);
}


/*
    Compare memory and argument.

    Arg < Mem  ->  N=1, Z=0, C=0
    Arg = Mem  ->  N=0, Z=1, C=1
    Arg > Mem  ->  N=0, Z=0, C=1
*/
void Cpu::CMP(uint8_t arg) 
{ 
    auto data = read();

    p.setNegative ( (bool) (data >  arg) );
    p.setZero     ( (bool) (data == arg) );
    p.setCarry    ( (bool) (data <= arg) );
}


/*
    CMP
    Compare Memory with Accumulator

    A - M                                      N Z C I D V
                                               + + + - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | immediate    | CMP #oper    | C9  | 2     | 2      |
    | zeropage     | CMP oper     | C5  | 2     | 3      |
    | zeropage,X   | CMP oper,X   | D5  | 2     | 4      |
    | absolute     | CMP oper     | CD  | 3     | 4      |
    | absolute,X   | CMP oper,X   | DD  | 3     | 4*     |
    | absolute,Y   | CMP oper,Y   | D9  | 3     | 4*     |
    | (indirect,X) | CMP (oper,X) | C1  | 2     | 6      |
    | (indirect),Y | CMP (oper),Y | D1  | 2     | 5*     |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::CMP() 
{ 
    CMP(a);
}


/*
    CPX
    Compare Memory and Index X

    X - M                                N Z C I D V
                                         + + + - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | immediate  | CPX #oper | E0  | 2     | 2      |
    | zeropage   | CPX oper  | E4  | 2     | 3      |
    | absolute   | CPX oper  | EC  | 3     | 4      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::CPX() 
{ 
    CMP(x); 
}


/*
    CPY
    Compare Memory and Index Y

    Y - M                                 N Z C I D V
                                          + + + - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | immediate  | CPY #oper | C0  | 2     | 2      |
    | zeropage   | CPY oper  | C4  | 2     | 3      |
    | absolute   | CPY oper  | CC  | 3     | 4      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::CPY() 
{ 
    CMP(y);
}


/*
    DCP (DCM)
    DEC oper + CMP oper

    M - 1 -> M, A - M                          N Z C I D V
                                               + + + - - -
    +--------------+--------------+-----+-------+--------+
    |  addressing  |  assembler   | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | DCP oper     | C7  |     2 |      5 |
    | zeropage,X   | DCP oper,X   | D7  |     2 |      6 |
    | absolute     | DCP oper     | CF  |     3 |      6 |
    | absolut,X    | DCP oper,X   | DF  |     3 |      7 |
    | absolut,Y    | DCP oper,Y   | DB  |     3 |      7 |
    | (indirect,X) | DCP (oper,X) | C3  |     2 |      8 |
    | (indirect),Y | DCP (oper),Y | D3  |     2 |      8 |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::DCP() 
{ 
    NOP();
}


/*
    DEC
    Decrement Memory by One

    M - 1 -> M                             N Z C I D V
                                           + + - - - -
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | zeropage   | DEC oper   | C6  | 2     | 5      |
    | zeropage,X | DEC oper,X | D6  | 2     | 6      |
    | absolute   | DEC oper   | CE  | 3     | 6      |
    | absolute,X | DEC oper,X | DE  | 3     | 7      |
    +------------+------------+-----+-------+--------+
*/
void Cpu::DEC() 
{ 
    uint16_t data = read();

    data--;

    p.setNegative (data);
    p.setZero     (data);

    write(data);
}


/*
    DEX
    Decrement Index X by One

    X - 1 -> X                            N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | DEX       | CA  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::DEX() 
{ 
    x--;

    p.setNegative (x);
    p.setZero     (x);
}


/*
    DEY
    Decrement Index Y by One

    Y - 1 -> Y                            N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | DEY       | 88  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::DEY() 
{ 
    y--;

    p.setNegative (y);
    p.setZero     (y); 
}


/*
    EOR
    Exclusive-OR Memory with Accumulator

    A EOR M -> A                               N Z C I D V
                                               + + - - - -
    +--------------+--------------+-----+-------+--------+
    |  addressing  |  assembler   | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | immediate    | EOR #oper    | 49  |     2 | 2      |
    | zeropage     | EOR oper     | 45  |     2 | 3      |
    | zeropage,X   | EOR oper,X   | 55  |     2 | 4      |
    | absolute     | EOR oper     | 4D  |     3 | 4      |
    | absolute,X   | EOR oper,X   | 5D  |     3 | 4*     |
    | absolute,Y   | EOR oper,Y   | 59  |     3 | 4*     |
    | (indirect,X) | EOR (oper,X) | 41  |     2 | 6      |
    | (indirect),Y | EOR (oper),Y | 51  |     2 | 5*     |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::EOR() 
{ 
    a ^= read();

    p.setNegative (a);
    p.setZero     (a);
}


/*
    INC
    Increment Memory by One

    M + 1 -> M                             N Z C I D V
                                           + + - - - -
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | zeropage   | INC oper   | E6  |     2 |      5 |
    | zeropage,X | INC oper,X | F6  |     2 |      6 |
    | absolute   | INC oper   | EE  |     3 |      6 |
    | absolute,X | INC oper,X | FE  |     3 |      7 |
    +------------+------------+-----+-------+--------+
*/
void Cpu::INC() 
{ 
    uint16_t data = read();

    data++;

    p.setNegative (data);
    p.setZero     (data);

    write(data);
}


/*
    INX
    Increment Index X by One

    X + 1 -> X                            N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | INX       | E8  |     1 |      2 |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::INX() 
{ 
    x++;

    p.setNegative (x);
    p.setZero     (x);
}


/*
    INY
    Increment Index Y by One

    X + 1 -> X                            N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | INY       | C8  |     1 |      2 |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::INY() 
{ 
    y++;

    p.setNegative (y);
    p.setZero     (y);
}


/*
    ISC (ISB, INS)
    INC oper + SBC oper

    M + 1 -> M, A - M - C -> A                 N Z C I D V
                                               + + + - - +
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | ISC oper     | E7  | 2     | 5      |
    | zeropage,X   | ISC oper,X   | F7  | 2     | 6      |
    | absolute     | ISC oper     | EF  | 3     | 6      |
    | absolut,X    | ISC oper,X   | FF  | 3     | 7      |
    | absolut,Y    | ISC oper,Y   | FB  | 3     | 7      |
    | (indirect,X) | ISC (oper,X) | E3  | 2     | 8      |
    | (indirect),Y | ISC (oper),Y | F3  | 2     | 4      |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::ISC() 
{ 
    NOP();
}


/*
    JAM (KIL, HLT)
    These instructions freeze the CPU.

    The processor will be trapped infinitely in T1 phase 
    with $FF on the data bus. — Reset required.

    Instruction codes: 02, 12, 22, 32, 42, 52, 62, 72, 92, B2, D2, F2
*/
void Cpu::JAM() 
{ 
    NOP();
}


/*
    JMP
    Jump to New Location

    (PC+1) -> PCL
    (PC+2) -> PCH                          N Z C I D V
                                           - - - - - -
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | absolute   | JMP oper   | 4C  | 3     | 3      |
    | indirect   | JMP (oper) | 6C  | 3     | 5      |
    +------------+------------+-----+-------+--------+
*/
void Cpu::JMP() 
{ 
    pc = op;
}


/*
    JSR
    Jump to New Location Saving Return Address

    push (PC+2),
    (PC+1) -> PCL
    (PC+2) -> PCH                         N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | absolute   | JSR oper  | 20  | 3     | 6      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::JSR() 
{ 
    pc--;

    uint8_t lo = (0x00FF & pc);
    uint8_t hi = (0xFF00 & pc) >> 8; 

    mem -> push(s, hi);
    mem -> push(s, lo);

    JMP();
}


/*
    LAS (LAR)
    LDA/TSX oper

    M AND SP -> A, X, SP                   N Z C I D V
                                           + + - - - -
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | absolut,Y  | LAS oper,Y | BB  | 3     | 4*     |
    +------------+------------+-----+-------+--------+
*/
void Cpu::LAS() 
{ 
    NOP();
}


/*
    LAX
    LDA oper + LDX oper

    M -> A -> X                                N Z C I D V
                                               + + - - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | LAX oper     | A7  | 2     | 3      |
    | zeropage,Y   | LAX oper,Y   | B7  | 2     | 4      |
    | absolute     | LAX oper     | AF  | 3     | 4      |
    | absolut,Y    | LAX oper,Y   | BF  | 3     | 4*     |
    | (indirect,X) | LAX (oper,X) | A3  | 2     | 6      |
    | (indirect),Y | LAX (oper),Y | B3  | 2     | 5*     |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::LAX() 
{
    NOP();
}


/*
    LDA
    Load Accumulator with Memory

    M -> A                                     N Z C I D V
                                               + + - - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | immediate    | LDA #oper    | A9  | 2     | 2      |
    | zeropage     | LDA oper     | A5  | 2     | 3      |
    | zeropage,X   | LDA oper,X   | B5  | 2     | 4      |
    | absolute     | LDA oper     | AD  | 3     | 4      |
    | absolute,X   | LDA oper,X   | BD  | 3     | 4*     |
    | absolute,Y   | LDA oper,Y   | B9  | 3     | 4*     |
    | (indirect,X) | LDA (oper,X) | A1  | 2     | 6      |
    | (indirect),Y | LDA (oper),Y | B1  | 2     | 5*     |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::LDA() 
{ 
    a = read();

    p.setNegative (a);
    p.setZero     (a);
}


/*
    LDX
    Load Index X with Memory

    M -> X                                 N Z C I D V
                                           + + - - - -
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | immediate  | LDX #oper  | A2  | 2     | 2      |
    | zeropage   | LDX oper   | A6  | 2     | 3      |
    | zeropage,Y | LDX oper,Y | B6  | 2     | 4      |
    | absolute   | LDX oper   | AE  | 3     | 4      |
    | absolute,Y | LDX oper,Y | BE  | 3     | 4*     |
    +------------+------------+-----+-------+--------+
*/
void Cpu::LDX() 
{ 
    x = read();

    p.setNegative (x);
    p.setZero     (x);
}


/*
    LDY
    Load Index Y with Memory

    M -> Y                                 N Z C I D V
                                           + + - - - -
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | immediate  | LDY #oper  | A0  | 2     | 2      |
    | zeropage   | LDY oper   | A4  | 2     | 3      |
    | zeropage,X | LDY oper,X | B4  | 2     | 4      |
    | absolute   | LDY oper   | AC  | 3     | 4      |
    | absolute,X | LDY oper,X | BC  | 3     | 4*     |
    +------------+------------+-----+-------+--------+
*/
void Cpu::LDY() 
{ 
    y = read();

    p.setNegative (y);
    p.setZero     (y);
}


/*
    LSR
    Shift One Bit Right (Memory or Accumulator)

    0 -> [76543210] -> C                    N Z C I D V
                                            0 + + - - -
    +-------------+------------+-----+-------+--------+
    | addressing  | assembler  | opc | bytes | cycles |
    +-------------+------------+-----+-------+--------+
    | accumulator | LSR A      | 4A  | 1     | 2      |
    | zeropage    | LSR oper   | 46  | 2     | 5      |
    | zeropage,X  | LSR oper,X | 56  | 2     | 6      |
    | absolute    | LSR oper   | 4E  | 3     | 6      |
    | absolute,X  | LSR oper,X | 5E  | 3     | 7      |
    +-------------+------------+-----+-------+--------+
*/
void Cpu::LSR() 
{ 
    uint8_t data  = read();
    uint8_t shift = data >> 1; 

    p.setNegative (shift);
    p.setZero     (shift);
    p.setCarry    ((bool) (data & 0x01));

    write (shift);
}


/*
    LXA (LAX immediate)
    Store * AND oper in A and X

    Highly unstable, involves a 'magic' constant, see ANE

    (A OR CONST) AND oper -> A -> X            N Z C I D V
                                               + + - - - -
    +------------+-----------+-----+-------+--------+----+
    | addressing | assembler | opc | bytes | cycles |    |
    +------------+-----------+-----+-------+--------+----+
    | immediate  | LXA #oper | AB  | 2     | 2      | †† |
    +------------+-----------+-----+-------+--------+----+
*/
void Cpu::LXA() 
{
    NOP();
}


/*
    NOP
    No Operation
                                          N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | NOP       | EA  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::NOP() { }


/*
    ORA
    OR Memory with Accumulator

    A OR M -> A                                N Z C I D V
                                               + + - - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | immediate    | ORA #oper    | 09  | 2     | 2      |
    | zeropage     | ORA oper     | 05  | 2     | 3      |
    | zeropage,X   | ORA oper,X   | 15  | 2     | 4      |
    | absolute     | ORA oper     | 0D  | 3     | 4      |
    | absolute,X   | ORA oper,X   | 1D  | 3     | 4*     |
    | absolute,Y   | ORA oper,Y   | 19  | 3     | 4*     |
    | (indirect,X) | ORA (oper,X) | 01  | 2     | 6      |
    | (indirect),Y | ORA (oper),Y | 11  | 2     | 5*     |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::ORA() 
{ 
    a |= read();

    p.setNegative (a);
    p.setZero     (a);
}


/*
    PHA
    Push Accumulator on Stack

    push A                                N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | PHA       | 48  | 1     | 3      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::PHA() 
{ 
    mem -> push(s, a);
}


/*
    PHP
    Push Processor Status on Stack

    The status register will be pushed with the break
    flag and bit 5 set to 1.

    push SR                               N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | PHP       | 08  | 1     | 3      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::PHP() 
{ 
    mem -> push(s, p);

    p.setBreak(false);
}


/*
    PLA
    Pull Accumulator from Stack

    pull A                                N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | PLA       | 68  | 1     | 4      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::PLA() 
{ 
    a = mem -> pop(s);

    p.setNegative (a);
    p.setZero     (a);
}


/*
    PLP
    Pull Processor Status from Stack

    The status register will be pulled with the break
    flag and bit 5 ignored.

    pull SR                               N Z C I D V
                                          + + + + + +
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | PLP       | 28  | 1     | 4      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::PLP() 
{ 
    p = mem -> pop(s);
}


/*
    RLA
    ROL oper + AND oper

    M = C <- [76543210] <- C, A AND M -> A     N Z C I D V
                                               + + + - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | RLA oper     | 27  | 2     | 5      |
    | zeropage,X   | RLA oper,X   | 37  | 2     | 6      |
    | absolute     | RLA oper     | 2F  | 3     | 6      |
    | absolut,X    | RLA oper,X   | 3F  | 3     | 7      |
    | absolut,Y    | RLA oper,Y   | 3B  | 3     | 7      |
    | (indirect,X) | RLA (oper,X) | 23  | 2     | 8      |
    | (indirect),Y | RLA (oper),Y | 33  | 2     | 8      |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::RLA()
{ 
    NOP();
}


/*
    ROL
    Rotate One Bit Left (Memory or Accumulator)

    C <- [76543210] <- C                    N Z C I D V
                                            + + + - - -
    +-------------+------------+-----+-------+--------+
    | addressing  | assembler  | opc | bytes | cycles |
    +-------------+------------+-----+-------+--------+
    | accumulator | ROL A      | 2A  | 1     | 2      |
    | zeropage    | ROL oper   | 26  | 2     | 5      |
    | zeropage,X  | ROL oper,X | 36  | 2     | 6      |
    | absolute    | ROL oper   | 2E  | 3     | 6      |
    | absolute,X  | ROL oper,X | 3E  | 3     | 7      |
    +-------------+------------+-----+-------+--------+
*/
void Cpu::ROL() 
{ 
    uint8_t data  = read();
    uint8_t shift = (data << 1) | p.getCarry();

    p.setNegative (shift);
    p.setZero     (shift);
    p.setCarry    ((bool) (data & 0x80));

    write (shift);
}


/*
    ROR
    Rotate One Bit Right (Memory or Accumulator)

    C -> [76543210] -> C                    N Z C I D V
                                            + + + - - -
    +-------------+------------+-----+-------+--------+
    | addressing  | assembler  | opc | bytes | cycles |
    +-------------+------------+-----+-------+--------+
    | accumulator | ROR A      | 6A  | 1     | 2      |
    | zeropage    | ROR oper   | 66  | 2     | 5      |
    | zeropage,X  | ROR oper,X | 76  | 2     | 6      |
    | absolute    | ROR oper   | 6E  | 3     | 6      |
    | absolute,X  | ROR oper,X | 7E  | 3     | 7      |
    +-------------+------------+-----+-------+--------+
*/
void Cpu::ROR() 
{ 
    uint8_t data  = read();
    uint8_t shift = (data >> 1) | (p.getCarry() << 7);

    p.setNegative (shift);
    p.setZero     (shift);
    p.setCarry    ((bool) (data & 0x01));

    write (shift);
}


/*
    RRA
    ROR oper + ADC oper

    M = C -> [76543210] -> C, A + M + C -> A, C

                                               N Z C I D V
                                               + + + - - +
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | RRA oper     | 67  | 2     | 5      |
    | zeropage,X   | RRA oper,X   | 77  | 2     | 6      |
    | absolute     | RRA oper     | 6F  | 3     | 6      |
    | absolut,X    | RRA oper,X   | 7F  | 3     | 7      |
    | absolut,Y    | RRA oper,Y   | 7B  | 3     | 7      |
    | (indirect,X) | RRA (oper,X) | 63  | 2     | 8      |
    | (indirect),Y | RRA (oper),Y | 73  | 2     | 8      |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::RRA() 
{ 
    NOP();
}


/*
    RTI
    Return from Interrupt

    The status register is pulled with the break flag
    and bit 5 ignored. Then PC is pulled from the stack.

    pull SR, pull PC                      

    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | RTI       | 40  | 1     | 6      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::RTI() 
{ 
    p   = mem -> pop(s); 

    pc  = mem -> pop(s);
    pc |= mem -> pop(s) << 8;

    p.setBreak(false);
}


/*
    RTS
    Return from Subroutine

    pull PC, PC+1 -> PC                   N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | RTS       | 60  | 1     | 6      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::RTS() 
{ 
    pc  = mem -> pop(s);
    pc |= mem -> pop(s) << 8;

    pc++;   
}


/*
    SAX (AXS, AAX)
    A and X are put on the bus at the same time 
    (resulting effectively in an AND operation) and stored in M

    A AND X -> M                               N Z C I D V
                                               - - - - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | SAX oper     | 87  | 2     | 3      |
    | zeropage,Y   | SAX oper,Y   | 97  | 2     | 4      |
    | absolute     | SAX oper     | 8F  | 3     | 4      |
    | (indirect,X) | SAX (oper,X) | 83  | 2     | 6      |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::SAX() 
{ 
    NOP();
}


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


/*
    SBX (AXS, SAX)
    CMP and DEX at once, sets flags like CMP

    (A AND X) - oper -> X                 N Z C I D V
                                          + + + - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | immediate  | SBX #oper | CB  |     2 |      2 |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::SBX() 
{ 
    NOP();
}


/*
    SEC
    Set Carry Flag

    1 -> C                                N Z C I D V
                                          - - 1 - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | SEC       | 38  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::SEC() 
{ 
    p.setCarry(true);
}


/*
    SED
    Set Decimal Flag

    1 -> D                                N Z C I D V
                                          - - - - 1 -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | SED       | F8  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::SED() 
{ 
    p.setDecimal(true);
}


/*
    SEI
    Set Interrupt Disable Status

    1 -> D                                N Z C I D V
                                          - - - 1 - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | SEI       | 78  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::SEI() 
{ 
    p.setInterrupt(true);
}


/*
    SHA (AHX, AXA)
    Stores A AND X AND (high-byte of addr. + 1) at addr.

    unstable: sometimes 'AND (H+1)' is dropped, page boundary 
    crossings may not work (with the high-byte of the value used 
    as the high-byte of the address)

    A AND X AND (H+1) -> M                         N Z C I D V
                                                   - - - - - -
    +--------------+--------------+-----+-------+--------+---+
    | addressing   | assembler    | opc | bytes | cycles |   |
    +--------------+--------------+-----+-------+--------+---+
    | absolut,Y    | SHA oper,Y   | 9F  | 3     | 5      | † |
    | (indirect),Y | SHA (oper),Y | 93  | 2     | 6      | † |
    +--------------+--------------+-----+-------+--------+---+
*/
void Cpu::SHA() 
{
    NOP();
}


/*
    SHY (A11, SYA, SAY)
    Stores Y AND (high-byte of addr. + 1) at addr.

    unstable: sometimes 'AND (H+1)' is dropped, page boundary 
    crossings may not work (with the high-byte of the value 
    used as the high-byte of the address)

    Y AND (H+1) -> M                           N Z C I D V
                                               - - - - - -
    +------------+------------+-----+-------+--------+---+
    | addressing | assembler  | opc | bytes | cycles |   |
    +------------+------------+-----+-------+--------+---+
    | absolut,X  | SHY oper,X | 9C  | 3     | 5      | † |
    +------------+------------+-----+-------+--------+---+
*/
void Cpu::SHY() 
{ 
    NOP();
}


/*
    SHX (A11, SXA, XAS)
    Stores X AND (high-byte of addr. + 1) at addr.

    unstable: sometimes 'AND (H+1)' is dropped, page boundary 
    crossings may not work (with the high-byte of the value 
    used as the high-byte of the address)

    X AND (H+1) -> M                           N Z C I D V
                                               - - - - - -
    +------------+------------+-----+-------+--------+---+
    | addressing | assembler  | opc | bytes | cycles |   |
    +------------+------------+-----+-------+--------+---+
    | absolut,Y  | SHX oper,Y | 9E  | 3     | 5      | † |
    +------------+------------+-----+-------+--------+---+
*/
void Cpu::SHX() 
{ 
    NOP();
}


/*
    SLO (ASO)
    ASL oper + ORA oper

    M = C <- [76543210] <- 0, A OR M -> A      N Z C I D V
                                               + + + - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | SLO oper     | 07  | 2     | 5      |
    | zeropage,X   | SLO oper,X   | 17  | 2     | 6      |
    | absolute     | SLO oper     | 0F  | 3     | 6      |
    | absolut,X    | SLO oper,X   | 1F  | 3     | 7      |
    | absolut,Y    | SLO oper,Y   | 1B  | 3     | 7      |
    | (indirect,X) | SLO (oper,X) | 03  | 2     | 8      |
    | (indirect),Y | SLO (oper),Y | 13  | 2     | 8      |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::SLO() 
{ 
    NOP();
}


/*
    SRE (LSE)
    LSR oper + EOR oper

    M = 0 -> [76543210] -> C, A EOR M -> A     N Z C I D V
                                               + + + - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | SRE oper     | 47  | 2     | 5      |
    | zeropage,X   | SRE oper,X   | 57  | 2     | 6      |
    | absolute     | SRE oper     | 4F  | 3     | 6      |
    | absolut,X    | SRE oper,X   | 5F  | 3     | 7      |
    | absolut,Y    | SRE oper,Y   | 5B  | 3     | 7      |
    | (indirect,X) | SRE (oper,X) | 43  | 2     | 8      |
    | (indirect),Y | SRE (oper),Y | 53  | 2     | 8      |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::SRE() 
{ 
    NOP();
}


/*
    STA
    Store Accumulator in Memory

    A -> M                                     N Z C I D V
                                               - - - - - -
    +--------------+--------------+-----+-------+--------+
    | addressing   | assembler    | opc | bytes | cycles |
    +--------------+--------------+-----+-------+--------+
    | zeropage     | STA oper     | 85  | 2     | 3      |
    | zeropage,X   | STA oper,X   | 95  | 2     | 4      |
    | absolute     | STA oper     | 8D  | 3     | 4      |
    | absolute,X   | STA oper,X   | 9D  | 3     | 5      |
    | absolute,Y   | STA oper,Y   | 99  | 3     | 5      |
    | (indirect,X) | STA (oper,X) | 81  | 2     | 6      |
    | (indirect),Y | STA (oper),Y | 91  | 2     | 6      |
    +--------------+--------------+-----+-------+--------+
*/
void Cpu::STA() 
{ 
    write(a);
}


/*
    STX
    Store Index X in Memory

    X -> M                                 N Z C I D V
                                           - - - - - -
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | zeropage   | STX oper   | 86  | 2     | 3      |
    | zeropage,Y | STX oper,Y | 96  | 2     | 4      |
    | absolute   | STX oper   | 8E  | 3     | 4      |
    +------------+------------+-----+-------+--------+
*/
void Cpu::STX() 
{ 
    write(x);
}


/*
    STY
    Store Index Y in Memory

    Y -> M                                 N Z C I D V
                                           - - - - - -
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | zeropage   | STY oper   | 84  | 2     | 3      |
    | zeropage,X | STY oper,X | 94  | 2     | 4      |
    | absolute   | STY oper   | 8C  | 3     | 4      |
    +------------+------------+-----+-------+--------+
*/
void Cpu::STY() 
{ 
    write(y);
}


/*
    TAS (XAS, SHS)
    Puts A AND X in SP and stores A AND X AND 
    (high-byte of addr. + 1) at addr.

    unstable: sometimes 'AND (H+1)' is dropped, page boundary 
    crossings may not work (with the high-byte of the value 
    used as the high-byte of the address)

    A AND X -> SP, A AND X AND (H+1) -> M      N Z C I D V
                                               - - - - - -
    +------------+------------+-----+-------+--------+---+
    | addressing | assembler  | opc | bytes | cycles |   |
    +------------+------------+-----+-------+--------+---+
    | absolut,Y  | TAS oper,Y | 9B  | 3     | 5      | † |
    +------------+------------+-----+-------+--------+---+
*/
void Cpu::TAS() 
{ 
    NOP();
}


/*
    TAX
    Transfer Accumulator to Index X

    A -> X                                N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | TAX       | AA  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::TAX() 
{ 
    x = a;

    p.setNegative(x);
    p.setZero    (x);
}


/*
    TAY
    Transfer Accumulator to Index Y

    A -> Y                                N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | TAX       | AA  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::TAY() 
{
    y = a;

    p.setNegative(y);
    p.setZero    (y);
}


/*
    TSX
    Transfer Stack Pointer to Index X

    SP -> X                               N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | TSX       | BA  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::TSX() 
{ 
    x = s;

    p.setNegative(x);
    p.setZero    (x);
}


/*
    TXA
    Transfer Index X to Accumulator

    X -> A                                N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | TXA       | 8A  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::TXA() 
{ 
    a = x;

    p.setNegative(a);
    p.setZero    (a);
}


/*
    TXS
    Transfer Index X to Stack Register

    X -> SP                               N Z C I D V
                                          - - - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | TXS       | 9A  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::TXS() 
{ 
    s = x;
}


/*
    TYA
    Transfer Index Y to Accumulator

    Y -> A                                N Z C I D V
                                          + + - - - -
    +------------+-----------+-----+-------+--------+
    | addressing | assembler | opc | bytes | cycles |
    +------------+-----------+-----+-------+--------+
    | implied    | TYA       | 98  | 1     | 2      |
    +------------+-----------+-----+-------+--------+
*/
void Cpu::TYA() 
{ 
    a = y;

    p.setNegative(a);
    p.setZero    (a);
}


/*
    USBC (SBC)
    SBC oper + NOP

    effectively same as normal SBC immediate, instr. E9.

    A - M - C -> A                         N Z C I D V
                                           + + + - - +
    +------------+------------+-----+-------+--------+
    | addressing | assembler  | opc | bytes | cycles |
    +------------+------------+-----+-------+--------+
    | immediate  | USBC #oper | EB  | 2     | 2      |
    +------------+------------+-----+-------+--------+
*/
void Cpu::USB() 
{ 
    NOP();
}