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

bin/debug: bin/ lib/libmandelbrot.so build/debug.o
	@echo "LD $@"
	@$(CC) build/debug.o -Llib $(LDFLAGS) $(LDLIBS) -o bin/debug

bin/:
	@mkdir -p bin

lib/libmandelbrot.so: lib/ build/mandelbrot.o
	@echo "LD $@"
	@$(CC) -shared build/mandelbrot.o $(LDFLAGS) -o lib/libmandelbrot.so

lib/:
	@mkdir -p lib 

build/mandelbrot.o: build/ src/mandelbrot.c include/mandelbrot.h
	@echo "CC $@"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -fPIC -c src/mandelbrot.c -o build/mandelbrot.o

build/debug.o: build/ src/debug.c include/mandelbrot.h
	@echo "CC $@"
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c src/debug.c -o build/debug.o

build/:
	@mkdir -p build

clean:
	@rm -rf build lib bin

