#include "ysim.h"

#include "errors.h"

/************************** Utility Routines ****************************/

/** Return nybble from op (pos 0: least-significant; pos 1:
 *  most-significant)
 */
//int count = 0;
static Byte
get_nybble(Byte op, int pos) {
  return (op >> (pos * 4)) & 0xF;
}

/************************** Condition Codes ****************************/

/** Conditions used in instructions */
typedef enum {
  ALWAYS_COND, LE_COND, LT_COND, EQ_COND, NE_COND, GE_COND, GT_COND
} Condition;

/** accessing condition code flags */
static inline bool get_cc_flag(Byte cc, unsigned flagBitIndex) {
  return !!(cc & (1 << flagBitIndex));
}
static inline bool get_zf(Byte cc) { return get_cc_flag(cc, ZF_CC); }
static inline bool get_sf(Byte cc) { return get_cc_flag(cc, SF_CC); }
static inline bool get_of(Byte cc) { return get_cc_flag(cc, OF_CC); }

/** Return true iff the condition specified in the least-significant
 *  nybble of op holds in y86.  Encoding of Figure 3.15 of Bryant's
 *  CompSys3e.
 */
bool
check_cc(const Y86 *y86, Byte op)
{
  bool ret = false;
  Condition condition = get_nybble(op, 0);
  Byte cc = read_cc_y86(y86);
  switch (condition) {
  case ALWAYS_COND:
    ret = true;
    break;
  case LE_COND:
    ret = (get_sf(cc) ^ get_of(cc)) | get_zf(cc);
    break;
  //@TODO add other cases
  case LT_COND:
    ret = (get_sf(cc) ^ get_of(cc));
    break;
  case EQ_COND:
    ret = get_zf(cc);
    break;
  case NE_COND:
    ret = !get_zf(cc);
    break;
  case GE_COND:
    ret = ~(get_sf(cc) ^ get_of(cc));
    break;
  case GT_COND:
    ret = ~(get_sf(cc) ^ get_of(cc)) & !get_zf(cc);
    break;
  default: {
    Address pc = read_pc_y86(y86);
    fatal("%08lx: bad condition code %d\n", pc, condition);
    break;
    }
  }
  return ret;
}

/** return true iff word has its sign bit set */
static inline bool
isLt0(Word word) {
  return (word & (1UL << (sizeof(Word)*CHAR_BIT - 1))) != 0;
}

/** Set condition codes for addition operation with operands opA, opB
 *  and result with result == opA + opB.
 */
static void
set_add_arith_cc(Y86 *y86, Word opA, Word opB, Word result)
{
  if(read_status_y86(y86) != STATUS_AOK) return;
	
  Byte res = 0x0;
  if(result == 0){ res |= 0x4;}
  if(isLt0(result)){ res |= 0x2;} 
  if(((isLt0(opA))&& isLt0(opB)  && !isLt0(result)) || 
      (!isLt0(opA)&& !isLt0(opB)  && isLt0(result))){

	res |= 0x1;
  }  
  write_cc_y86(y86, res);
}

/** Set condition codes for subtraction operation with operands opA, opB
 *  and result with result == opA - opB. 
 */
static void
set_sub_arith_cc(Y86 *y86, Word opA, Word opB, Word result)
{
  //@TODO
 Byte res = 0x0;
 if(result == 0){ res |= 0x4;}
  if(isLt0(result)){ res |= 0x2;} 
  if((isLt0(opA)&& !isLt0(opB)  && isLt0(result)) || 
      (!isLt0(opA)&& isLt0(opB)  && !isLt0(result))){

	res |= 0x1;
  }   
  write_cc_y86(y86, res);

}

static void
set_logic_op_cc(Y86 *y86, Word result)
{
  //@TODO 
  Byte res = 0x0;
  if((long long)(result) == 0){ res |= 0x4;}
  if(isLt0(result)){ res |= 0x2;} 
  res &= ~(0x1); 
  write_cc_y86(y86, res);

}

/**************************** Operations *******************************/

static void
op1(Y86 *y86, Byte op, Register regA, Register regB)
{
  enum {ADDL_FN, SUBL_FN, ANDL_FN, XORL_FN };
  //@TODO
  Word rA = read_register_y86(y86, regA);
  Word rB = read_register_y86(y86, regB);
  switch(get_nybble(op, 0)){
	
	case ADDL_FN :
		set_add_arith_cc(y86, rA, rB, (long long)(rA) + (long long)(rB));
		write_register_y86(y86, regA, (long long)(rA) + (long long)(rB));
	        break;
        case SUBL_FN :
		set_sub_arith_cc(y86, rA, rB, (long long)(rA) - (long long)(rB));
	       	write_register_y86(y86, regA, (long long)(rA) - (long long)(rB));

		break;
 	case ANDL_FN :
		set_logic_op_cc(y86, rA & rB);
		write_register_y86(y86, regA, (long long)(rA) & (long long)(rB));

		break;
	case XORL_FN :
		set_logic_op_cc(y86, rA ^ rB);
		write_register_y86(y86, regA, (long long)(rA) ^ (long long)(rB));

		break;
	default :
		write_status_y86(y86, STATUS_INS);
		break;		
  }
}

/*********************** Single Instruction Step ***********************/

typedef enum {
  HALT_CODE, NOP_CODE, CMOVxx_CODE, IRMOVQ_CODE, RMMOVQ_CODE, MRMOVQ_CODE,
  OP1_CODE, Jxx_CODE, CALL_CODE, RET_CODE,
  PUSHQ_CODE, POPQ_CODE } BaseOpCode;

/** Execute the next instruction of y86. Must change status of
 *  y86 to STATUS_HLT on halt, STATUS_ADR or STATUS_INS on
 *  bad address or instruction.
 */
void
step_ysim(Y86 *y86)
{
  if(read_status_y86(y86) != STATUS_AOK) return;

  Address pc = read_pc_y86(y86);
  Byte ad = read_memory_byte_y86(y86, pc);
  
  switch(get_nybble(ad, 1)){
	
	  case HALT_CODE : ;
		  write_status_y86(y86, STATUS_HLT);
		  break;
	  case NOP_CODE : ;
		  write_pc_y86(y86, pc + sizeof(Address)/4);
		  break;
	  case IRMOVQ_CODE : ;
		Word temp = read_memory_word_y86(y86, pc + 1 + sizeof(Byte));
		write_register_y86(y86, get_nybble(read_memory_byte_y86(y86, pc + 1), 0), temp);
		write_pc_y86(y86, pc + 1 + sizeof(Byte) + sizeof(Word));
		break;
	  case CALL_CODE: ;
		Word code = read_memory_word_y86(y86, read_pc_y86(y86) + 1);
		write_register_y86(y86, REG_RSP , read_register_y86(y86, REG_RSP) - sizeof(Address));
		write_memory_word_y86(y86, read_register_y86(y86, REG_RSP), pc + 1 + sizeof(Address));

		write_pc_y86(y86, code);
		
		break;
	  case RET_CODE : ;
		write_pc_y86(y86, read_memory_word_y86(y86, read_register_y86(y86, REG_RSP)));
		write_register_y86(y86, REG_RSP, read_register_y86(y86, (REG_RSP))+sizeof(Address));
		break;
	  case MRMOVQ_CODE: ;
		Byte reg = read_memory_byte_y86(y86, pc + sizeof(Byte));
		Word offs = read_memory_word_y86(y86, pc + 2*sizeof(Byte));
		Register srcReg = get_nybble(reg, 0);
		Register destReg = get_nybble(reg, 1);
		write_register_y86(y86, destReg, read_memory_word_y86(y86, offs + read_register_y86(y86, srcReg)));
		write_pc_y86(y86,  pc + 1 + sizeof(Byte) + sizeof(Word));
		break;
	  case RMMOVQ_CODE: ;
		Byte regs = read_memory_byte_y86(y86, pc + sizeof(Byte));
		Word offset = read_memory_word_y86(y86, pc + 2*sizeof(Byte));
		Register src = get_nybble(regs, 1);
		Register dest = get_nybble(regs, 0);
		Address destination = read_register_y86(y86, dest) + offset;
		Word source = read_register_y86(y86, src);
		write_memory_word_y86(y86, destination, source);
		write_pc_y86(y86,  pc + 1 + sizeof(Byte) + sizeof(Word));
	        break;	
	  case CMOVxx_CODE: ;
		if(check_cc(y86, read_memory_byte_y86(y86, pc))){
			Byte regc = read_memory_byte_y86(y86, pc + 1);
			write_register_y86(y86, get_nybble(regc, 0), read_register_y86(y86, get_nybble(regc, 1)));
		}	
		write_pc_y86(y86, pc + 1 + sizeof(Byte));
		break;
	  case 	OP1_CODE: ;
		Byte registers =  read_memory_byte_y86(y86, pc + 1);
		op1(y86, read_memory_byte_y86(y86, pc), get_nybble(registers, 0), get_nybble(registers, 1));
		write_pc_y86(y86, pc+2);
		break;
	  case PUSHQ_CODE : ;
		Address rsp = read_register_y86(y86, REG_RSP) - sizeof(Word);
		write_memory_word_y86(y86, rsp, read_register_y86(y86, get_nybble(read_memory_byte_y86(y86, pc + 1), 1)));
		write_register_y86(y86, REG_RSP, rsp);
		write_pc_y86(y86, pc +2);

		break;
	  case POPQ_CODE : ;
	 	Word valu = read_memory_word_y86(y86, read_register_y86(y86, REG_RSP));
		write_register_y86(y86, REG_RSP, read_register_y86(y86, REG_RSP) + sizeof(Word));

		write_register_y86(y86, get_nybble(read_memory_byte_y86(y86, pc + 1), 1), valu);	

		write_pc_y86(y86, pc +2);

		break;
	  case Jxx_CODE : ;
		if(check_cc(y86, ad)){
			Word code = read_memory_word_y86(y86, read_pc_y86(y86) + 1);

			write_pc_y86(y86, code);
		}else{
			write_pc_y86(y86, pc + sizeof(Byte) + sizeof(Word));
		}
		break;
	  default : ;
		printf("\n %u \n", ad);
		write_status_y86(y86, STATUS_INS);
		break;
  }
 	  /* 
 	  printf("REG_RSP: %lX \n", read_register_y86(y86, REG_RSP));	  
	  printf("REG_RAX: %lX \n", read_register_y86(y86, REG_RAX));
	  printf("REG_RDX: %lX \n", read_register_y86(y86, REG_RDX));	
	  printf("REG_RBX: %lX \n", read_register_y86(y86, REG_RBX));
	  printf("REG_RDI: %lX \n", read_register_y86(y86, REG_RDI));	
	  printf("REG_RSI: %lX \n", read_register_y86(y86, REG_RSI)); 
	  //printf("STACK: %lX \n", read_memory_word_y86(y86, 0x1f0));
	  printf("%d \n------------- \n", count);
	  */
 //printf("%d \n", count);
 //count++;

}

