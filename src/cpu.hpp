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
#include <string>
#include <array>

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
    // 6502 general instruction list row
    //

    struct Command 
    {
        // Operation instruction implementation
        uint8_t (Cpu::*inst) (void);

        // Addressing mode implementation
        void (Cpu::*mode) (void);

        // Execute command
        void execute(Cpu * cpu);
    };

    // Instruction list (include all illegal codes)
    std::array<Command, 256> commands; 

    //
    // Address Modes
    //

    void IMM  (); // immediate              
    void ABS  (); // absolute               
    void ABSX (); // absolute, X-indexed    
    void ABSY (); // absolute, Y-indexed 

    void ZPG  (); // zeropage
    void ZPGX (); // zeropage, X-indexed
    void ZPGY (); // zeropage, Y-indexed

    void IMP  (); // implied
    void ACC  (); // accumulator
    void IND  (); // indirect
    void INDX (); // X-indexed, indirect
    void INDY (); // indirect, Y-indexed
    void REL  (); // relative

    //
    // Instruction set
    //

    uint8_t ADC(); // Add Memory to Accumulator with Carry
    uint8_t ALR(); //
    uint8_t ANC(); //
    uint8_t AND(); // "AND" Memory with Accumulator
    uint8_t ANE(); //
    uint8_t ARR(); //
    uint8_t ASL(); // Shift Left One Bit (Memory or Accumulator)

    uint8_t BCC(); // Branch on Carry Clear
    uint8_t BCS(); // Branch on Carry Set
    uint8_t BEQ(); // Branch on Result Zero
    uint8_t BIT(); // Test Bits in Memory with Accumulator
    uint8_t BMI(); // Branch on Result Minus
    uint8_t BNE(); // Branch on Result not Zero
    uint8_t BPL(); // Branch on Result Plus
    uint8_t BRK(); // Force Break
    uint8_t BVC(); // Branch on Overflow Clear
    uint8_t BVS(); // Branch on Overflow Set

    uint8_t CLC(); // Clear Carry Flag
    uint8_t CLD(); // Clear Decimal Mode
    uint8_t CLI(); // Clear interrupt Disable Bit
    uint8_t CLV(); // Clear Overflow Flag
    uint8_t CMP(); // Compare Memory and Accumulator
    uint8_t CPX(); // Compare Memory and Index X
    uint8_t CPY(); // Compare Memory and Index Y

    uint8_t DCP(); //
    uint8_t DEC(); // Decrement Memory by One
    uint8_t DEX(); // Decrement Index X by One
    uint8_t DEY(); // Decrement Index Y by One

    uint8_t EOR(); // "Exclusive-Or" Memory with Accumulator

    uint8_t INC(); // Increment Memory by One
    uint8_t INX(); // Increment Index X by One
    uint8_t INY(); // Increment Index Y by One
    uint8_t ISC(); //

    uint8_t JAM(); // These instructions freeze the CPU.
    uint8_t JMP(); // Jump to New Location
    uint8_t JSR(); // Jump to New Location Saving Return Address

    uint8_t LAS(); //
    uint8_t LAX(); //
    uint8_t LDA(); // Load Accumulator with Memory
    uint8_t LDX(); // Load Index X with Memory
    uint8_t LDY(); // Load Index Y with Memory
    uint8_t LSR(); // Shift Right One Bit (Memory or Accumulator)
    uint8_t LXA(); //

    uint8_t NOP(); // No Operation

    uint8_t ORA(); // "OR" Memory with Accumulator

    uint8_t PHA(); // Push Accumulator on Stack
    uint8_t PHP(); // Push Processor Status on Stack
    uint8_t PLA(); // Pull Accumulator from Stack
    uint8_t PLP(); // Pull Processor Status from Stack

    uint8_t RLA(); // 
    uint8_t ROL(); // Rotate One Bit Left (Memory or Accumulator)
    uint8_t ROR(); // Rotate One Bit Right (Memory or Accumulator)
    uint8_t RRA(); // 
    uint8_t RTI(); // Return from Interrupt
    uint8_t RTS(); // Return from Subroutine

    uint8_t SAX(); //
    uint8_t SBC(); // Subtract Memory from Accumulator with Borrow
    uint8_t SBX(); // 
    uint8_t SEC(); // Set Carry Flag
    uint8_t SED(); // Set Decimal Mode
    uint8_t SEI(); // Set Interrupt Disable Status
    uint8_t SHA(); //
    uint8_t SHY(); //
    uint8_t SHX(); //
    uint8_t SLO(); //
    uint8_t SRE(); //
    uint8_t STA(); // Store Accumulator in Memory
    uint8_t STX(); // Store Index X in Memory
    uint8_t STY(); // Store Index Y in Memory

    uint8_t TAS(); //
    uint8_t TAX(); // Transfer Accumulator to Index X
    uint8_t TAY(); // Transfer Accumulator to Index Y
    uint8_t TSX(); // Transfer Stack Pointer to Index X
    uint8_t TXA(); // Transfer Index X to Accumulator
    uint8_t TXS(); // Transfer Index X to Stack Pointer
    uint8_t TYA(); // Transfer Index Y to Accumulator

    uint8_t USBC(); // 

public:

    Cpu();

    void clock ();
    void reset ();
};

#endif