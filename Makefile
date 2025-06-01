SHELL=/bin/bash

CFLAGS := $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lm

# run the code and clean up the junk
run: boid_sim clean_obj
	@./boid_sim

# Link the src code
boid_sim: main.o
	@cc main.c -o boid_sim $(LDFLAGS)

# Compile the src code
main.o: main.c
	@cc -c main.c -o main.o $(CFLAGS)


# CLEAN ALL EXTRAS
# Remove object files.
clean_obj:
	@find . -name '*.o' -type f -delete




# Remove anything compiled
clean: clean_obj
	@rm -f boid_sim
