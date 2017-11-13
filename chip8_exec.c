#include <stdio.h>
#include "chip8_structs.h"
#include "chip8_exec.h"

// assumptions: 
// 		- PC always increments by 2 after each instruction is executed

// 0x0NNN
void chip8_func_call(struct chip8_cpu* cpu, unsigned short opcode) {
	printf("called");
}

// 0x00E0
void chip8_func_disp_clear(struct chip8_cpu* cpu, unsigned short opcode) {
	for (int i = 0; i < 32 * 64; i ++) {
		cpu->mem[i] = 0;
	}
}

// 0x00EE
void chip8_func_flow_return(struct chip8_cpu* cpu, unsigned short opcode) {
	// error checking

	// pop address from the stack and assign to pc
	cpu->PC = cpu->stack[--cpu->SP];
}

// 0x1NNN
void chip8_func_flow_goto(struct chip8_cpu* cpu, unsigned short opcode) {
	unsigned short jump_addr = opcode & 0x0FFF;
	cpu->PC = jump_addr;
	cpu->PC -= 2;
}

// 0x2NNN
void chip8_func_flow_call(struct chip8_cpu* cpu, unsigned short opcode) {
	// get func address
	unsigned short func_addr = opcode & 0x0FFF;

	// push current address to the stack
	cpu->stack[cpu->SP++] = cpu->PC;

	// set pc to func address
	cpu->PC = func_addr;
	cpu->PC -= 2;
}

// 0x3XNN
void chip8_func_cond_imm_eq(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned short imm = opcode & 0x00FF;

	// skip if equal
	if (cpu->V[x] == imm) {
		cpu->PC += 2;
	}
}

// 0x4XNN
void chip8_func_cond_imm_ne(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned short imm = opcode & 0x00FF;

	// skip if not equal
	if (cpu->V[x] != imm) {
		cpu->PC += 2;
	}
}

// 0x5XY0
void chip8_func_cond_eq(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	// skip if equal
	if (cpu->V[x] == cpu->V[y]) {
		cpu->PC += 2;
	}
}

// 0x6XNN
void chip8_func_const_set(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned short imm = opcode & 0x00FF;

	cpu->V[x] = imm;
}

// 0x7XNN
void chip8_func_const_add(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned short imm = opcode & 0x00FF;

	cpu->V[x] += imm;
}

// 0x8XY0
void chip8_func_assign_set(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	cpu->V[x] = cpu->V[y];
}

// 0x8XY1
void chip8_func_bitop_or(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	cpu->V[x] |= cpu->V[y];
}

// 0x8XY2
void chip8_func_bitop_and(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	cpu->V[x] &= cpu->V[y];
}

// 0x8XY3
void chip8_func_bitop_xor(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	cpu->V[x] ^= cpu->V[y];	
}

// 0x8XY4
void chip8_func_math_addeq(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	// get carry bit
	long result = cpu->V[x] + cpu->V[y];
	if ((result >> 16) & 1)
		cpu->V[0xF] = 1;

	cpu->V[x] += cpu->V[y];
}

// 0x8XY5
void chip8_func_math_subeq(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	// TODO: get borrow bit

	cpu->V[x] -= cpu->V[y];	
}

// 0x8XY6
void chip8_func_bitop_srl(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	// get lsb
	cpu->V[0xF] = cpu->V[y] & 1;

	cpu->V[y] >>= 1;
	cpu->V[x] = cpu->V[y];
}

// 0x8XY7
void chip8_func_math_sub(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	// TODO: get borrow bit

	cpu->V[x] = cpu->V[y] - cpu->V[x];
}

// 0x8XYE
void chip8_func_bitop_sll(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	// get msb
	cpu->V[0xF] = (cpu->V[y] >> 15) & 1;

	cpu->V[y] <<= 1;
	cpu->V[x] = cpu->V[y];
}

// 0x9XY0
void chip8_func_cond_ne(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;

	if (cpu->V[x] != cpu->V[y])
		cpu->PC += 2;
}

// 0xANNN
void chip8_func_mem_seti(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned short imm = opcode & 0x0FFF;

	cpu->I = imm;
}

// 0xBNNN
void chip8_func_flow_pc(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned short imm = opcode & 0x0FFF;

	// exec
	cpu->PC = cpu->V[0] + imm;
	cpu->PC -= 2;
}

// 0xCXNN
void chip8_func_rand_and(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned short imm = opcode & 0x00FF;

	// TODO: rand char
	unsigned char rand = 0;

	cpu->V[x] = rand & imm;
}

// 0xDXYN
void chip8_func_disp_draw(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	unsigned char y = (opcode & 0x00F0) >> 4;
	unsigned char imm = opcode & 0x000F;

	// TODO: draw stuff
}

// 0xEX9E
void chip8_func_keyop_eq(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;

	// TODO: get key press
}

// 0xEXA1
void chip8_func_keyop_ne(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;

	// TODO: get key press
}

// 0xFX07
void chip8_func_timer_delay(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	
	cpu->V[x] = cpu->delay_timer;
}

// 0xFX0A
void chip8_func_keyop_wait(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	
	// TODO: blocking operation
}

// 0xFX15
void chip8_func_timer_set(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	
	cpu->delay_timer = cpu->V[x];
}

// 0xFX18
void chip8_func_sound_set(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;
	
	cpu->sound_timer = cpu->V[x];
}

// 0xFX1E
void chip8_func_mem_addeq(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;

	cpu->I += cpu->V[x];	
}

// 0xFX29
void chip8_func_mem_spr(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;

	cpu->I = cpu->V[x] * 5;
}

// 0xFX33
void chip8_func_bcd(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;

	// TODO: bcd
}

// 0xFX55
void chip8_func_mem_dump(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;

	// TODO: check bounds

	unsigned char offset = cpu->I;
	cpu->I += x + 1;
	for (int i = 0; i < x + 1; i ++) {
		cpu->mem[offset + i * 2] = cpu->V[i] >> 8;
		cpu->mem[offset + i * 2 + 1] = cpu->V[i] & 0xFF;
	}
}

// 0xF65
void chip8_func_mem_load(struct chip8_cpu* cpu, unsigned short opcode) {
	// get values
	unsigned char x = (opcode & 0x0F00) >> 8;

	// TODO: check bounds

	unsigned char offset = cpu->I;
	cpu->I += x + 1;
	for (int i = 0; i < x + 1; i ++) {
		cpu->V[i] = 0;
		cpu->V[i] |= cpu->mem[offset + i] << 8;
		cpu->V[i] |= cpu->mem[offset + i * 2];
	}
}
