CC := gcc

# compiler flags
CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -g -O0 -pthread 

# preprocessor flags (also -DMY_MACRO=...)
CPPFLAGS := -Iinclude

# linker flags
LDFLAGS := -pthread

# libraries
LDLIBS := -lmandelbrot


.PHONY: run clean

run: bin/debug
	@echo "RUN $<"
	@LD_LIBRARY_PATH=./lib time ./bin/debug

bin/debug: lib/libmandelbrot.so build/debug.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) build/debug.o -Llib $(LDFLAGS) $(LDLIBS) -o bin/debug
	

lib/libmandelbrot.so: build/mandelbrot.o
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared build/mandelbrot.o $(LDFLAGS) -o lib/libmandelbrot.so

	

build/mandelbrot.o: src/mandelbrot.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -fPIC -c src/mandelbrot.c -o build/mandelbrot.o

build/debug.o: src/debug.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c src/debug.c -o build/debug.o

	

clean:
	@rm -rf build lib bin

