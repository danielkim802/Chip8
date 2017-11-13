#include "chip8_structs.h"
#include "chip8_cpu.h"

int main(void) {
	struct chip8_cpu cpu;
	chip8_initialize(&cpu);
	unsigned short opcode = chip8_fetch_opcode(&cpu);
	unsigned char opcode_key = chip8_decode_opcode(&cpu, opcode);
	chip8_exec_opcode(&cpu, opcode, opcode_key);
	return 0;
}