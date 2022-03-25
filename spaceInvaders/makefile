CC= g++
CFLAGS= -Wall -g
LDFLAGS= -lGL -lglfw
INC= -I./include
OBJECTS = objects/*.o

.PHONY: clean all

all: main.o emulator.o cpu.o display.o memory.o stack.o keyboard.o
	@mkdir -p bin
	$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) $(INC) -o bin/chip8.out

main.o: src/main.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/main.cpp -o objects/main.o

emulator.o: src/emulator.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/emulator.cpp -o objects/emulator.o

cpu.o: src/cpu.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/cpu.cpp -o objects/cpu.o

display.o: src/display.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/display.cpp -o objects/display.o

memory.o: src/memory.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/memory.cpp -o objects/memory.o

stack.o: src/stack.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/stack.cpp -o objects/stack.o

keyboard.o: src/keyboard.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/keyboard.cpp -o objects/keyboard.o

clean:
	$(RM) bin/chip8.out
	$(RM) -r objects
