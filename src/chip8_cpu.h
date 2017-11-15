// chip8 init functions
void chip8_initialize(struct chip8_cpu* cpu);
void chip8_load_rom(struct chip8_cpu* cpu, unsigned char* rom, unsigned int rom_size);

// chip8 instruction functions
unsigned short chip8_fetch_opcode(struct chip8_cpu* cpu);
unsigned char chip8_decode_opcode(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_exec_opcode(struct chip8_cpu* cpu, unsigned short opcode, unsigned char opcode_key);
void chip8_cycle(struct chip8_cpu* cpu);

// chip8 debugging
void chip8_print_display(struct chip8_cpu* cpu);
void chip8_print_state(struct chip8_cpu* cpu);