#include "chip8_cpu.h"
#include "chip8_exec.h"
#include "constants.h"

// initialize cpu
void chip8_initialize(struct chip8_cpu* cpu) {
	// set memory to zero
	for (int i = 0; i < 4096; i ++) {
		cpu->mem[i] = 0;
	}

	// copy over font set

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
	return 0;
}

// execute opcode given a key
void chip8_exec_opcode(struct chip8_cpu* cpu, unsigned short opcode, unsigned char opcode_key) {
	// make jump table
	static void (*chip8_opcode_functions[35]) (struct chip8_cpu*, unsigned short) = {
		chip8_func_call, 		chip8_func_disp_clear, 	chip8_func_flow_return, chip8_func_flow_goto,
		chip8_func_flow_call, 	chip8_func_cond_imm_eq, chip8_func_cond_imm_ne, chip8_func_cond_eq, 
		chip8_func_const_set, 	chip8_func_const_add, 	chip8_func_assign_set, 	chip8_func_bitop_or,
		chip8_func_bitop_and, 	chip8_func_bitop_xor, 	chip8_func_math_addeq, 	chip8_func_math_subeq,
		chip8_func_bitop_srl, 	chip8_func_math_sub, 	chip8_func_bitop_sll, 	chip8_func_cond_ne,
		chip8_func_mem_seti, 	chip8_func_flow_pc, 	chip8_func_rand_and, 	chip8_func_disp_draw,
		chip8_func_keyop_eq, 	chip8_func_keyop_ne, 	chip8_func_timer_delay, chip8_func_keyop_wait,
		chip8_func_timer_set, 	chip8_func_sound_set, 	chip8_func_mem_addeq, 	chip8_func_mem_spr,
		chip8_func_bcd, 		chip8_func_mem_dump, 	chip8_func_mem_load
	};

	// execute function
	chip8_opcode_functions[opcode_key](cpu, opcode);
}





