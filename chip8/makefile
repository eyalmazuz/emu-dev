CC= g++
CFLAGS= -Wall -g
LDFLAGS= -L/usr/local/lib -lSDL2
INC= -I./include -I/usr/local/include

.PHONY: clean all

all: main.o chip8.o
	$(CC) objects/main.o objects/chip8.o $(CFLAGS) $(LDFLAGS) $(INC) -o bin/chip8.out

main.o: src/main.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/main.cpp -o objects/main.o

chip8.o: src/chip8.cpp
	@mkdir -p objects	
	$(CC) $(CFLAGS) $(INC) -c src/chip8.cpp -o objects/chip8.o

clean:
	$(RM) bin/chip8.out
	$(RM) -r objects
