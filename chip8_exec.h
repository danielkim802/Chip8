struct chip8_cpu;

// opcode execute functions
void chip8_func_call(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_disp_clear(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_flow_return(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_flow_goto(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_flow_call(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_cond_imm_eq(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_cond_imm_ne(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_cond_eq(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_const_set(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_const_add(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_assign_set(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_bitop_or(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_bitop_and(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_bitop_xor(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_math_addeq(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_math_subeq(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_bitop_srl(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_math_sub(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_bitop_sll(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_cond_ne(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_mem_seti(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_flow_pc(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_rand_and(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_disp_draw(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_keyop_eq(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_keyop_ne(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_timer_delay(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_keyop_wait(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_timer_set(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_sound_set(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_mem_addeq(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_mem_spr(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_bcd(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_mem_dump(struct chip8_cpu* cpu, unsigned short opcode);
void chip8_func_mem_load(struct chip8_cpu* cpu, unsigned short opcode);