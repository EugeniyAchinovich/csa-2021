#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"

class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        /* YOUR CODE HERE */
	instr->_data = CalculatingData(instr, ip);
	instr->_addr = CalculatingAddr(instr, ip);
	instr->_nextIp = CalculatingNextIp(instr, ip);
    }

private:
    /* YOUR CODE HERE */
	Word CalculatingData(const InstructionPtr& instr, Word ip)
	{
	switch (instr->_type)
          {
            case IType::Csrr:
            {
                return instr->_csrVal;
            }
            case IType::Csrw:
            {
                return instr->_src1Val;
            }
            case IType::St:
            {
                return instr->_src2Val;
            }
            case IType::J:
            case IType::Jr:
            {
                return ip + 4u;
            }
            case IType::Auipc:
            {
            	return ip + instr->_imm.value();
            }
            default:
            {
            	return CalculatingAlu(instr);
            }
          }
	}

	Word CalculatingAddr(const InstructionPtr& instr, Word ip)
        {
	  switch (instr->_type)
          {
            case IType::St:
            {
                return CalculatingAlu(instr);
            }
            case IType::Ld:
            {
                return instr->_data; 
            }
            default:
            {
                return 0xdeadbeaf;
            }
          }
        }

	Word CalculatingNextIp(const InstructionPtr& instr, Word ip)
        {
	  bool isJump = false;
          switch(instr->_brFunc)
          {
            case BrFunc::Eq:
            {
                isJump = instr->_src1Val == instr->_src2Val;
                break;
            }
            case BrFunc::Neq:
            {
                isJump = instr->_src1Val != instr->_src2Val;
                break;
            }
            case BrFunc::Lt:
            {
                isJump = static_cast<SignedWord>(instr->_src1Val) <
                         static_cast<SignedWord>(instr->_src2Val);
                break;
            }
            case BrFunc::Ltu:
            {
                isJump = instr->_src1Val < instr->_src2Val;
                break;
            }
            case BrFunc::Ge:
            {
                isJump = static_cast<SignedWord>(instr->_src1Val) >=
                         static_cast<SignedWord>(instr->_src2Val);
                break;
            }
            case BrFunc::Geu:
            {
                isJump = instr->_src1Val >= instr->_src2Val;
                break;
            }
            case BrFunc::AT:
            {
                isJump = true;
                break;
            }
            case BrFunc::NT:
            {
                isJump = false;
                break;
            }
          }
	Word jumpTo = ip + 4u;
        if (isJump)
        {
            switch(instr->_type)
            {
                case IType::Br:
                case IType::J:
                {
                    jumpTo = ip + instr->_imm.value();
                    break;
                }
                case IType::Jr:
                {
                    jumpTo = instr->_src1Val + instr->_imm.value();
                    break;
                }
            }
        }
        return jumpTo;
	}

	Word CalculatingAlu(const InstructionPtr& instr)
	{
	  Word op1 = instr->_src1Val;
          Word op2 = instr->_imm.value_or(instr->_src2Val);
          switch (instr->_aluFunc)
          {
            case AluFunc::Add:
            {
                return op1 + op2;
            }
            case AluFunc::Sub:
            {
                return op1 - op2;
            }
            case AluFunc::And:
            {
                return op1 & op2;
            }
            case AluFunc::Or:
            {
                return op1 | op2;
            }
            case AluFunc::Xor:
            {
                return op1 ^ op2;
            }
            case AluFunc::Slt:
            {
                return static_cast<SignedWord>(op1) <
                       static_cast<SignedWord>(op2);
            }
            case AluFunc::Sltu:
            {
                return op1 < op2;
            }
            case AluFunc::Sll:
            {
                return op1 << (op2 % 32);
            }
            case AluFunc::Srl:
            {
                return op1 >> (op2 % 32);
            }
            case AluFunc::Sra:
            {
                return static_cast<SignedWord>(op1) >> (op2 % 32);
            }
            default:
            {
                return 0xdeadbeaf;
            }
	  }
	}
};

#endif // RISCV_SIM_EXECUTOR_H
