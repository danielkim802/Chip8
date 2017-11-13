// chip8 functions
void chip8_initialize(struct chip8_cpu* cpu);
void chip8_load_rom(struct chip8_cpu* cpu, unsigned char* rom, unsigned int rom_size);
unsigned short chip8_fetch_opcode(struct chip8_cpu* cpu);
unsigned char chip8_decode_opcode(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_exec_opcode(struct chip8_cpu* cpu, unsigned short opcode, unsigned char opcode_key);