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


.PHONY: clean release run

# $@ = Target of rule
# $< = First dependency
# $^ = all dependencies

run: bin/debug
	@echo "RUN $<"
	@LD_LIBRARY_PATH=./lib time ./$<

bin/debug: build/debug.o lib/libmandelbrot_debug.so 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $< -Llib $(LDFLAGS) $(LDLIBS) -o $@
	
lib/libmandelbrot_debug.so: build/mandelbrot_debug.o
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared $< $(LDFLAGS) -o $@

build/mandelbrot_debug.o: src/mandelbrot.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -fPIC -c $< -o $@

build/debug.o: src/debug.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -c $< -o $@


release: bin/grid lib/libmandelbrot.so

bin/grid: build/grid.o lib/libmandelbrot.a 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ -Llib $(LDFLAGS) -o $@

lib/libmandelbrot.so: build/mandelbrot.o 
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared $< $(LDFLAGS) -o $@

lib/libmandelbrot.a: build/mandelbrot.o
	@mkdir -p lib
	@echo "AR $@"
	@ar rcs $@ $<

build/mandelbrot.o: src/mandelbrot.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -fPIC -c $< -o $@

build/grid.o: src/main.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	@rm -rf bin build lib results/grids/debug.txt 

