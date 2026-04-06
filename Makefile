CC := gcc

# compiler flags
D_CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -g -O0 -pthread 
R_CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -O3 -pthread 

# preprocessor flags (also -DMY_MACRO=...)
CPPFLAGS := -Iinclude

# linker flags
LDFLAGS := -pthread -lm

# libraries
LDLIBS := -lmandelbrot


.PHONY: clean release debug cli mandelbrot render tests

# $@ = Target of rule
# $< = First dependency
# $^ = all dependencies

##### DEBUGGING ########################################################################
debug: cli

# debugging cli.c
cli: bin/debug_cli

bin/debug_cli: build/debug/cli.o build/debug/mandelbrot.o build/debug/export.o build/debug/debug.o 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

build/debug/cli.o: src/cli.c include/cli.h include/mandelbrot.h
	@mkdir -p build/debug
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

# debugging mandelbrot.c
mandelbrot: bin/debug_mandelbrot
	@echo "RUN $<"
	@LD_LIBRARY_PATH=./lib/debug time ./$<

bin/debug_mandelbrot: build/debug/export.o lib/debug/libmandelbrot.so build/debug/debug.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ -Llib/debug $(LDFLAGS) $(LDLIBS) -o $@
	
lib/debug/libmandelbrot.so: build/debug/mandelbrot.o
	@mkdir -p lib/debug
	@echo "LD $@"
	@$(CC) -shared $< $(LDFLAGS) -o $@

# debugging render.c
render: bin/debug_render
	@echo "RUN $<"
	@ ./$<

bin/debug_render: build/debug/mandelbrot.o build/debug/export.o build/debug/render.o build/debug/debug.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

build/debug/render.o: src/render.c include/render.h include/mandelbrot.h
	@mkdir -p build/debug
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -c $< -o $@


# rules needed by debugging mandelbrot and cli
build/debug/debug.o: src/debug.c include/cli.h include/export.h include/mandelbrot.h
	@mkdir -p build/debug
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -c $< -o $@

build/debug/export.o: src/export.c include/export.h include/mandelbrot.h
	@mkdir -p build/debug
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

build/debug/mandelbrot.o: src/mandelbrot.c include/mandelbrot.h 
	@mkdir -p build/debug
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -fPIC -c $< -o $@


##### RELEASE ##########################################################################
release: bin/grid lib/libmandelbrot.so

bin/grid: build/cli.o build/mandelbrot.o build/export.o build/grid.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

lib/libmandelbrot.so: build/mandelbrot.o 
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared $< $(LDFLAGS) -o $@

build/cli.o: src/cli.c include/cli.h include/mandelbrot.h
	@mkdir -p build 
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -c $< -o $@

build/grid.o: src/main.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -c $< -o $@

build/export.o: src/export.c
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -c $< -o $@

build/mandelbrot.o: src/mandelbrot.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -fPIC -c $< -o $@


##### TESTS ############################################################################


TESTS := $(patsubst tests/test_%.c, bin/test_%, $(wildcard tests/test*.c))

tests: $(TESTS)
	@for t in $(TESTS); do $$t; done

bin/test_cli: build/tests/test_cli.o build/tests/mandelbrot.o build/tests/export.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

bin/test_export: build/tests/test_export.o build/tests/mandelbrot.o build/tests/render.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

bin/test_mandelbrot: build/tests/test_mandelbrot.o 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

bin/test_render: build/tests/test_render.o build/tests/mandelbrot.o 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

build/tests/test_%.o: tests/test_%.c src/%.c 
	@mkdir -p build/tests
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

build/tests/mandelbrot.o: src/mandelbrot.c 
	@mkdir -p build/tests
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

build/tests/export.o: src/export.c 
	@mkdir -p build/tests
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

build/tests/render.o: src/render.c 
	@mkdir -p build/tests
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

clean:
	@rm -rf bin/test* bin/debug* build/tests build/debug lib/debug

