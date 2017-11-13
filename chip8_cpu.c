#include "chip8_structs.h"
#include "chip8_cpu.h"
#include "chip8_exec.h"
#include "constants.h"

// font set
unsigned char chip8_fontset[80] =
{ 
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// initialize cpu
void chip8_initialize(struct chip8_cpu* cpu) {
	// set memory to zero
	for (int i = 0; i < 4096; i ++) {
		cpu->mem[i] = 0;
	}

	// load font set
	for (int i = 0; i < 80; i ++) {
		cpu->mem[i] = chip8_fontset[i];
	}

	// set registers to 0
	for (int i = 0; i < 16; i ++) {
		cpu->V[i] = 0;
	}
	cpu->I = 0;
	cpu->PC = 0x200;
	cpu->SP = 0;

	// initialize io
	for (int i = 0; i < 16; i ++) {
		cpu->keyboard[i] = 0;
	}
	for (int i = 0; i < 32 * 64; i ++) {
		cpu->display[i] = 0;
	}

	// initialize timers
	cpu->delay_timer = 0;
	cpu->sound_timer = 0;
}

// load rom into memory
void chip8_load_rom(struct chip8_cpu* cpu, unsigned char* rom, unsigned int rom_size) {
	for (int i = 0; i < rom_size; i ++) {
		cpu->mem[i + 0x200] = rom[i];
	}
}

// fetch instruction
unsigned short chip8_fetch_opcode(struct chip8_cpu* cpu) {
	unsigned short pc = cpu->PC;
	unsigned char msb = cpu->mem[pc];
	unsigned char lsb = cpu->mem[pc + 1];
	return (msb << 8) | lsb;
}

// decode opcode; return key to function table
unsigned char chip8_decode_opcode(struct chip8_cpu* cpu, unsigned short opcode) {
	switch (opcode >> 12) {
		case 0x0:
			if (opcode == 0x00E0) {
				return FUNC_DISP_CLEAR;
			} else if (opcode == 0x00EE) {
				return FUNC_FLOW_RETURN;
			} else {
				return FUNC_CALL;
			}
		case 0x1:
			return FUNC_FLOW_GOTO;
		case 0x2:
			return FUNC_FLOW_CALL;
		case 0x3:
			return FUNC_COND_IMM_EQ;
		case 0x4:
			return FUNC_COND_IMM_NE;
		case 0x5:
			return FUNC_COND_EQ;
		case 0x6:
			return FUNC_CONST_SET;
		case 0x7:
			return FUNC_CONST_ADD;
		case 0x8:
			switch (opcode & 0xF) {
				case 0x0:
					return FUNC_ASSIGN_SET;
				case 0x1:
					return FUNC_BITOP_OR;
				case 0x2:
					return FUNC_BITOP_AND;
				case 0x3:
					return FUNC_BITOP_XOR;
				case 0x4:
					return FUNC_MATH_ADDEQ;
				case 0x5:
					return FUNC_MATH_SUBEQ;
				case 0x6:
					return FUNC_BITOP_SRL;
				case 0x7:
					return FUNC_MATH_SUB;
				case 0xE:
					return FUNC_BITOP_SLL;
				default:
					return FUNC_INVALID;
			}
		case 0x9:
			return FUNC_COND_NE;
		case 0xA:
			return FUNC_MEM_SETI;
		case 0xB:
			return FUNC_FLOW_PC;
		case 0xC:
			return FUNC_RAND_AND;
		case 0xD:
			return FUNC_DISP_DRAW;
		case 0xE:
			switch (opcode & 0xFF) {
				case 0x9E:
					return FUNC_KEYOP_EQ;
				case 0xA1:
					return FUNC_KEYOP_NE;
				default:
					return FUNC_INVALID;
			}
		case 0xF:
			switch (opcode & 0xFF) {
				case 0x07:
					return FUNC_TIMER_DELAY;
				case 0x0A:
					return FUNC_KEYOP_WAIT;
				case 0x15:
					return FUNC_TIMER_SET;
				case 0x18:
					return FUNC_SOUND_SET;
				case 0x1E:
					return FUNC_MEM_ADDEQ;
				case 0x29:
					return FUNC_MEM_SPR;
				case 0x33:
					return FUNC_BCD;
				case 0x55:
					return FUNC_MEM_DUMP;
				case 0x65:
					return FUNC_MEM_LOAD;
				default:
					return FUNC_INVALID;
			}
		default:
			return FUNC_INVALID;
	}
}

// execute opcode given a key
void chip8_exec_opcode(struct chip8_cpu* cpu, unsigned short opcode, unsigned char opcode_key) {
	// make jump table
	static void (*chip8_opcode_functions[35]) (struct chip8_cpu*, unsigned short) = {
		chip8_func_call,
		chip8_func_disp_clear,
		chip8_func_flow_return,
		chip8_func_flow_goto,
		chip8_func_flow_call,
		chip8_func_cond_imm_eq,
		chip8_func_cond_imm_ne,
		chip8_func_cond_eq,
		chip8_func_const_set,
		chip8_func_const_add,
		chip8_func_assign_set,
		chip8_func_bitop_or,
		chip8_func_bitop_and,
		chip8_func_bitop_xor,
		chip8_func_math_addeq,
		chip8_func_math_subeq,
		chip8_func_bitop_srl,
		chip8_func_math_sub,
		chip8_func_bitop_sll,
		chip8_func_cond_ne,
		chip8_func_mem_seti,
		chip8_func_flow_pc,
		chip8_func_rand_and,
		chip8_func_disp_draw,
		chip8_func_keyop_eq,
		chip8_func_keyop_ne,
		chip8_func_timer_delay,
		chip8_func_keyop_wait,
		chip8_func_timer_set,
		chip8_func_sound_set,
		chip8_func_mem_addeq,
		chip8_func_mem_spr,
		chip8_func_bcd,
		chip8_func_mem_dump,
		chip8_func_mem_load
	};

	// execute function
	chip8_opcode_functions[opcode_key](cpu, opcode);
}





