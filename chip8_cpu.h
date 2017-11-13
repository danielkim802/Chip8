// chip8 functions
struct chip8_cpu {
	// memory
	unsigned char mem[4096];
	unsigned short stack[16];

	// registers
	unsigned short V[16];
	unsigned short I;
	unsigned short PC;
	unsigned short SP;

	// IO
	unsigned char keyboard[16];
	unsigned char display[32 * 64];

	// timers
	unsigned char delay_timer;
	unsigned char sound_timer;
};

void chip8_initialize(struct chip8_cpu* cpu);
void chip8_load_rom(struct chip8_cpu* cpu, unsigned char* rom, unsigned int rom_size);
unsigned short chip8_fetch_opcode(struct chip8_cpu* cpu);
unsigned char chip8_decode_opcode(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_exec_opcode(struct chip8_cpu* cpu, unsigned short opcode, unsigned char opcode_key);