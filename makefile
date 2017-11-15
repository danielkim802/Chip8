chip8make: ./src/emulator.c ./src/chip8_cpu.c ./src/chip8_exec.c
	gcc -o chip8 ./src/emulator.c ./src/chip8_cpu.c ./src/chip8_exec.c
