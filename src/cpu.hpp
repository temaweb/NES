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

#ifndef CPU_HPP
#define CPU_HPP

#include <cstdint>

class Cpu
{
private:

    //
    // A    Accumulator
    //
    //      The accumulator is the main register for arithmetic and logic operations.
    //      Unlike the index registers X and Y, it has a direct connection to the Arithmetic
    //      and Logic Unit (ALU). This is why many operations are only available for the
    //      accumulator, not the index registers.
    //

    uint8_t a = 0x00;


    //
    // X    Index register X
    //
    //      This is the main register for addressing data with indices.
    //      It has a special addressing mode, indexed indirect, which lets you to have
    //      a vector table on the zero page.
    //

    uint8_t x = 0x00;


    //
    // Y    Index register Y
    //
    //      The Y register has the least operations available. On the other hand,
    //      only it has the indirect indexed addressing mode that enables access
    //      to any memory place without having to use self-modifying code.
    //

    uint8_t y = 0x00;


    //
    // S    Stack pointer
    //
    //      The NMOS 65xx processors have 256 bytes of stack memory, ranging
    //      from $0100 to $01FF. The S register is a 8-bit offset to the stack page.
    //      In other words, whenever anything is being pushed on the stack,
    //      it will be stored to the address $0100 + S.
    //
    //      The Stack pointer can be read and written by transfering its value to or from
    //      the index register X (see below) with the TSX and TXS instructions.
    //

    uint8_t s = 0x00;


    //
    // P    Processor status
    //
    //      This 8-bit register stores the state of the processor.
    //      The bits in this register are called flags. Most of the flags have something
    //      to do with arithmetic operations.
    //
    //      The P register can be read by pushing it on the stack (with PHP or by causing
    //      an interrupt). If you only need to read one flag, you can use the branch
    //      instructions. Setting the flags is possible by pulling the P register from stack
    //      or by using the flag set or clear instructions.
    //

    uint8_t p = 0x00;


    //
    // PC   Program Counter
    //
    //      This register points the address from which the next instruction byte
    //      (opcode or parameter) will be fetched. Unlike other registers, this one
    //      is 16 bits in length. The low and high 8-bit halves of the register are called PCL
    //      and PCH, respectively.
    //
    //      The Program Counter may be read by pushing its value on the stack.
    //      This can be done either by jumping to a subroutine or by causing an interrupt.
    //

    uint16_t pc = 0x0000;

    //
    // Addressing modes
    //

    void IMM  ();
    void ABS  ();
    void ZP   ();
    void IMP  ();
    void ACC  ();
    void ABSX ();
    void ABSY ();
    void ZPX  ();
    void ZPY  ();
    void IND  ();
    void INDX ();
    void INDY ();
    void REL  ();

    //
    // Instruction set
    //

    void ADC(); // Add Memory to Accumulator with Carry
    void AND(); // "AND" Memory with Accumulator
    void ASL(); // Shift Left One Bit (Memory or Accumulator)

    void BCC(); // Branch on Carry Clear
    void BCS(); // Branch on Carry Set
    void BEQ(); // Branch on Result Zero
    void BIT(); // Test Bits in Memory with Accumulator
    void BMI(); // Branch on Result Minus
    void BNE(); // Branch on Result not Zero
    void BPL(); // Branch on Result Plus
    void BRK(); // Force Break
    void BVC(); // Branch on Overflow Clear
    void BVS(); // Branch on Overflow Set

    void CLC(); // Clear Carry Flag
    void CLD(); // Clear Decimal Mode
    void CLI(); // Clear interrupt Disable Bit
    void CLV(); // Clear Overflow Flag
    void CMP(); // Compare Memory and Accumulator
    void CPX(); // Compare Memory and Index X
    void CPY(); // Compare Memory and Index Y

    void DEC(); // Decrement Memory by One
    void DEX(); // Decrement Index X by One
    void DEY(); // Decrement Index Y by One

    void EOR(); // "Exclusive-Or" Memory with Accumulator

    void INC(); // Increment Memory by One
    void INX(); // Increment Index X by One
    void INY(); // Increment Index Y by One

    void JMP(); // Jump to New Location
    void JSR(); // Jump to New Location Saving Return Address

    void LDA(); // Load Accumulator with Memory
    void LDX(); // Load Index X with Memory
    void LDY(); // Load Index Y with Memory
    void LSR(); // Shift Right One Bit (Memory or Accumulator)

    void NOP(); // No Operation

    void ORA(); // "OR" Memory with Accumulator

    void PHA(); // Push Accumulator on Stack
    void PHP(); // Push Processor Status on Stack
    void PLA(); // Pull Accumulator from Stack
    void PLP(); // Pull Processor Status from Stack

    void ROL(); // Rotate One Bit Left (Memory or Accumulator)
    void ROR(); // Rotate One Bit Right (Memory or Accumulator)
    void RTI(); // Return from Interrupt
    void RTS(); // Return from Subroutine

    void SBC(); // Subtract Memory from Accumulator with Borrow
    void SEC(); // Set Carry Flag
    void SED(); // Set Decimal Mode
    void SEI(); // Set Interrupt Disable Status
    void STA(); // Store Accumulator in Memory
    void STX(); // Store Index X in Memory
    void STY(); // Store Index Y in Memory

    void TAX(); // Transfer Accumulator to Index X
    void TAY(); // Transfer Accumulator to Index Y
    void TSX(); // Transfer Stack Pointer to Index X
    void TXA(); // Transfer Index X to Accumulator
    void TXS(); // Transfer Index X to Stack Pointer
    void TYA(); // Transfer Index Y to Accumulator

public:

    void clock ();
    void reset ();
};

#endif