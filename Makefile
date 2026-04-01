CC := gcc

# compiler flags
D_CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -g -O0 -pthread 
R_CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -O3 -pthread 

# preprocessor flags (also -DMY_MACRO=...)
CPPFLAGS := -Iinclude

# linker flags
LDFLAGS := -pthread

# libraries
D_LDLIBS := -lmandelbrot_debug
R_LDLIBS := -lmandelbrot


.PHONY: run clean

run: bin/debug
	@echo "RUN $<"
	@LD_LIBRARY_PATH=./lib time ./bin/debug

bin/debug: lib/libmandelbrot_debug.so build/debug.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) build/debug.o -Llib $(LDFLAGS) $(D_LDLIBS) -o bin/debug
	

lib/libmandelbrot_debug.so: build/mandelbrot_debug.o
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared build/mandelbrot_debug.o $(LDFLAGS) -o lib/libmandelbrot_debug.so

build/mandelbrot_debug.o: src/mandelbrot.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -fPIC -c src/mandelbrot.c -o build/mandelbrot_debug.o

build/debug.o: src/debug.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -c src/debug.c -o build/debug.o


release: bin/grid 

bin/grid: lib/libmandelbrot.so build/grid.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) build/grid.o -Llib $(LDFLAGS) $(R_LDLIBS) -o bin/grid

lib/libmandelbrot.so: build/mandelbrot.o 
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared build/mandelbrot.o $(LDFLAGS) -o lib/libmandelbrot.so

build/mandelbrot.o: src/mandelbrot.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -fPIC -c src/mandelbrot.c -o build/mandelbrot.o

build/grid.o: src/main.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -c src/main.c -o build/grid.o

clean:
	@rm -rf bin/debug build lib/*_debug.so results/grids/debug.txt 

