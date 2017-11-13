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