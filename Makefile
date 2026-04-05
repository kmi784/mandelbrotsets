CC := gcc

# compiler flags
D_CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -g -O0 -pthread 
R_CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -O3 -pthread 

# preprocessor flags (also -DMY_MACRO=...)
CPPFLAGS := -Iinclude

# linker flags
LDFLAGS := -pthread

# libraries
LDLIBS := -lmandelbrot_debug


.PHONY: clean release debug cli mandelbrot

# $@ = Target of rule
# $< = First dependency
# $^ = all dependencies

##### DEBUGGING ########################################################################
debug: cli

# debugging cli
cli: bin/cli_debug

bin/cli_debug: build/cli_debug.o build/mandelbrot_debug.o build/debug.o 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

build/cli_debug.o: src/cli.c include/cli.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

# debugging mandelbrot
mandelbrot: bin/debug_mandelbrot
	@echo "RUN $<"
	@LD_LIBRARY_PATH=./lib time ./$<

bin/debug_mandelbrot: build/debug.o lib/libmandelbrot_debug.so 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $< -Llib $(LDFLAGS) $(LDLIBS) -o $@
	
lib/libmandelbrot_debug.so: build/mandelbrot_debug.o
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared $< $(LDFLAGS) -o $@

# rules needed by debugging mandelbrot and cli
build/mandelbrot_debug.o: src/mandelbrot.c include/mandelbrot.h 
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -fPIC -c $< -o $@

build/debug.o: src/debug.c include/mandelbrot.h include/cli.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -c $< -o $@


##### RELEASE ##########################################################################
release: bin/grid lib/libmandelbrot.so

bin/grid: build/mandelbrot.o build/cli.o build/grid.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

lib/libmandelbrot.so: build/mandelbrot.o 
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared $< $(LDFLAGS) -o $@

build/mandelbrot.o: src/mandelbrot.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -fPIC -c $< -o $@

build/cli.o: src/cli.c include/mandelbrot.h include/cli.h
	@mkdir -p build 
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -c $< -o $@

build/grid.o: src/main.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	@rm -rf bin build lib results/grids/debug.txt 

