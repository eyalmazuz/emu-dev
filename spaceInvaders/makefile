CC= g++
CFLAGS= -Wall -g
LDFLAGS= -lGL -lglfw
INC= -I./include
OBJECTS = objects/*.o

.PHONY: clean all

all: main.o emulator.o video.o memory.o shiftRegister.o
	@mkdir -p bin
	$(CC) $(OBJECTS) $(CFLAGS) $(LDFLAGS) $(INC) -o bin/invaders.out

main.o: src/main.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/main.cpp -o objects/main.o

emulator.o: src/emulator.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/emulator.cpp -o objects/emulator.o

# cpu.o: src/cpu.cpp
# 	@mkdir -p objects	
# 	$(CC) $(CFLAGS) $(INC) -c src/cpu.cpp -o objects/cpu.o

video.o: src/video.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/video.cpp -o objects/video.o

memory.o: src/memory.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/memory.cpp -o objects/memory.o

shiftRegister.o: src/shiftRegister.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/shiftRegister.cpp -o objects/shiftRegister.o

# keyboard.o: src/keyboard.cpp
# 	@mkdir -p objects	
# 	$(CC) $(CFLAGS) $(INC) -c src/keyboard.cpp -o objects/keyboard.o

clean:
	$(RM) bin/invaders.out
	$(RM) -r objects
