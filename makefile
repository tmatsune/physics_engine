# Compiler flags
CFLAGS = -std=c17 -Wall -Wextra -Werror `sdl2-config --cflags`

# Linker flags
LDFLAGS = `sdl2-config --libs` -lSDL2_image

# Targets
all: main

main: main.o utils.o game.o assert.o rigid_body.o render.o 
	gcc main.o utils.o game.o assert.o rigid_body.o render.o -o main $(LDFLAGS)

main.o: main.c
	gcc $(CFLAGS) -c main.c -o main.o

game.o: game/game.c game/game.h assert/assert.h utils/utils.h utils/render.h physics/rigid_body.o
	gcc $(CFLAGS) -c game/game.c -o game.o 

rigid_body.o: physics/rigid_body.c assert/assert.h utils/utils.h utils/render.h game/game.h
	gcc $(CFLAGS) -c physics/rigid_body.c -o rigid_body.o

utils.o: utils/utils.c utils/utils.h assert/assert.h 
	gcc $(CFLAGS) -c utils/utils.c -o utils.o
render.o: utils/render.c utils/render.h assert/assert.h
	gcc $(CFLAGS) -c utils/render.c -o render.o 

assert.o: assert/assert.c assert/assert.h
	gcc $(CFLAGS) -c assert/assert.c -o assert.o

clean:
	rm -f main main.o utils.o
