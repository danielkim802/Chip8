#include <stdio.h>
#include <stdlib.h>
#include "chip8_structs.h"
#include "chip8_cpu.h"

int main(int argc, char* argv[]) {
	// load rom
	struct chip8_cpu cpu;
	chip8_initialize(&cpu);
	FILE* rom_file = fopen(argv[1], "rb");

	fseek (rom_file, 0, SEEK_END);
	const unsigned int rom_size = ftell(rom_file);
	rewind(rom_file);
	unsigned char* rom = (unsigned char*) malloc(sizeof(unsigned char) * rom_size);
	fread(rom, 1, rom_size, rom_file);

	chip8_load_rom(&cpu, &rom[0], rom_size);

	// main loop
	while (1) {
		puts("\033[44A");					// overwrite previous print
		chip8_cycle(&cpu);					// step cpu cycle
		chip8_print_display(&cpu);			// print display
		chip8_print_state(&cpu);			// print cpu state
		for (int i = 0; i < 1000000; i ++);	// delay
	}
	
	return 0;
}