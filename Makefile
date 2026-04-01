SRC := main.c 
D_TRGT := debug 
BINARIES := ../bin

CXX := gcc
D_FLAGS := -Wall -Wpedantic -Wsign-conversion -O0 -g -pthread
R_FLAGS := -O3 -pthread

SCALES := small medium large
ITERATIONS := 5000
OUTPUT := data

THREADS := 1 2 4 6 8

all: $(D_TRGT)

$(D_TRGT): $(SRC)
	$(CXX) $(D_FLAGS) $(SRC) -o $(D_TRGT)
	@echo "Running debug build..."
	time ./$(D_TRGT)

threads:
	@for T in $(THREADS); do \
		$(CXX) $(R_FLAGS) -DSCALE=\"large\" -DNUMBER_ITERATIONS=$(ITERATIONS) -DNUMBER_THREADS=$$T main.c -o $(BINARIES)/test_threads_cores$$T; \
	done

release:
	@for S in $(SCALES); do\
		DEFS="-DSCALE=\"$$S\" -DNUMBER_ITERATIONS=$(ITERATIONS) -DOUTPUT_DIRECTORY=\"$(OUTPUT)\""; \
		$(CXX) $(R_FLAGS) $$DEFS $(SRC) -o $(BINARIES)/$${S}; \
	done

clean: 
	@rm -f debug ../data/array_debug.txt .main.c.swp