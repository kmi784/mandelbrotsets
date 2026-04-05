CC := gcc

# compiler flags
D_CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -g -O0 -pthread 
R_CFLAGS := -Wall -Wextra -Wpedantic -Wsign-conversion -O3 -pthread 

# preprocessor flags (also -DMY_MACRO=...)
CPPFLAGS := -Iinclude

# linker flags
LDFLAGS := -pthread -lm

# libraries
LDLIBS := -lmandelbrot_debug


.PHONY: clean release debug cli mandelbrot render

# $@ = Target of rule
# $< = First dependency
# $^ = all dependencies

##### DEBUGGING ########################################################################
debug: render

# debugging cli.c
cli: bin/cli_debug

bin/cli_debug: build/cli_debug.o build/mandelbrot_debug.o build/export_debug.o build/debug.o 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

build/cli_debug.o: src/cli.c include/cli.h include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

# debugging mandelbrot.c
mandelbrot: bin/debug_mandelbrot
	@echo "RUN $<"
	@LD_LIBRARY_PATH=./lib time ./$<

bin/debug_mandelbrot: build/debug.o build/export_debug.o lib/libmandelbrot_debug.so 
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ -Llib $(LDFLAGS) $(LDLIBS) -o $@
	
lib/libmandelbrot_debug.so: build/mandelbrot_debug.o
	@mkdir -p lib 
	@echo "LD $@"
	@$(CC) -shared $< $(LDFLAGS) -o $@

# debugging render.c
render: bin/debug_render
	@echo "RUN $<"
	@ ./$<

bin/debug_render: build/mandelbrot_debug.o build/render_debug.o build/debug.o
	@mkdir -p bin
	@echo "LD $@"
	@$(CC) $^ $(LDFLAGS) -o $@

build/render_debug.o: src/render.c include/render.h include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -c $< -o $@


# rules needed by debugging mandelbrot and cli
build/debug.o: src/debug.c include/cli.h include/export.h include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) $(CPPFLAGS) -c $< -o $@

build/export_debug.o: src/export.c include/export.h include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(D_CFLAGS) -c $< -o $@

build/mandelbrot_debug.o: src/mandelbrot.c include/mandelbrot.h 
	@mkdir -p build
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

build/export.o: 
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -c $< -o $@

build/mandelbrot.o: src/mandelbrot.c include/mandelbrot.h
	@mkdir -p build
	@echo "CC $@"
	@$(CC) $(R_CFLAGS) $(CPPFLAGS) -fPIC -c $< -o $@

clean:
	@rm -rf bin build lib results/grids/debug.txt 

