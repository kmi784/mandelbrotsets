#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "../include/mandelbrot.h"


int main(int argc, char* argv[]){

    // default options
    double real_min = -3.5;
    double real_max = 1.5;
    double imag_min = -1.5;
    double imag_max = 1.5;
    char* grid_size = "medium";
    unsigned int num_threads = 2;
    unsigned int num_iter = 1000;
    char* output_path = NULL;
    int output_set = 0;
    int assume_yes = 0;

    // option handling
    struct option long_options[] = {
        {"real_min", required_argument, 0, 'a'},
        {"real_max", required_argument, 0, 'b'},
        {"imag_min", required_argument, 0, 'c'},
        {"imag_max", required_argument, 0, 'd'},
        {"grid_size", required_argument, 0, 'e'},
        {"num_threads", required_argument, 0, 'f'},
        {"num_iter", required_argument, 0, 'g'},
        {"help", no_argument, 0, 'h'},
        {"output_dir", required_argument, 0, 'o'},
        {"yes", no_argument, 0, 'y'},
        {0, 0, 0, 0}
    };

    int opt;
    while((opt = getopt_long(argc, argv, "ho:y", long_options, NULL)) != -1) {
        switch(opt) {
        case 'a':
            real_min = strtod(optarg, NULL);
            break;

        case 'b':
            real_max = strtod(optarg, NULL);
            break;

        case 'c':
            imag_min = strtod(optarg, NULL);
            break;

        case 'd':
            imag_max = strtod(optarg, NULL);
            break;

        case 'e':
            grid_size = optarg;
            break;
        
        case 'f':
            num_threads = (unsigned int)strtoul(optarg, NULL, 10);
            break;

        case 'g':
            num_iter = (unsigned int)strtoul(optarg, NULL, 10);
            break;
        
        case 'h':
            printf(
                "Usage: %s [options]\n"
                "  --grid_size=<string>   size of grid: small, medium or large are allowed (default: %s)\n"
                "  --real_min=<float>     right boundary in complex plain (default: %f)\n"
                "  --real_max=<float>     left boundary in complex plain (default: %f)\n"
                "  --imag_min=<float>     lower boundary in complex plain (default: %f)\n"
                "  --imag_max=<float>     upper boundary in complex plain (default: %f)\n"
                "  --num_threads=<uint>   number of threads (default: %u)\n"
                "  --num_iter=<uint>      maximal amount of iterations per pixel (default: %u)\n"  
                "  --output_dir, -o       output directory (default: results/grids/[grid_size].txt)\n"
                "  --yes, -y              skip confirmation prompt (for scripts)\n"
                "  --help, -h             help\n",
                argv[0], grid_size, real_min, real_max, imag_min, imag_max, num_threads, num_iter
            );
            return 0;
        
        case 'o':
            output_path = optarg;
            output_set = 1;
            break;

        case 'y':
            assume_yes = 1;
            break;

        default:
                fprintf(
                    stderr, 
                    "ERROR: Invalid argument use --help for more information.\n"
                );
                return 1;
        }
    }

    GridSize size;
    if(strcmp(grid_size, "small") == 0) {
        size = GRID_SMALL;
    } else if(strcmp(grid_size, "medium") == 0) {
        size = GRID_MEDIUM;
    } else if(strcmp(grid_size, "large") == 0) {
        size = GRID_LARGE;
    } else {
        fprintf(stderr, "Invalid grid size use small, medium or large.");
        return 1;
    }

    char output_buffer[256];
    if(!output_set) {
        snprintf(
            output_buffer, sizeof(output_buffer), "results/grids/%s.txt", grid_size
        );
        output_path = output_buffer;
    }

    if(!assume_yes) {
        printf(
            "Current configs:\n"
            "  - grid size:            %s\n"
            "  - real part range:      (%f, %f)\n"
            "  - imaginary part range: (%f, %f)\n"
            "  - number of threads:    %u\n"
            "  - number of iterations: %u\n"
            "  - output directory:     %s\n",
            grid_size, real_min, real_max, imag_min, imag_max, num_threads, num_iter, output_path
        );

        printf("Continue? [y/N]: ");
        int c = getchar();
        if(c != 'y' && c != 'Y') {
            printf("Process interrupted.\n");
            return 0;
        }
    }

    FILE* file = fopen(output_path, "w");
    if(file == NULL) {
        fprintf(stderr, "ERROR: Opening file %s failed. No such directory.\n", output_path);
        return 1;
    }

    MandelbrotGrid grid;
    if(init_grid(&grid, size, real_min, imag_min, real_max, imag_max)) {
        fprintf(stderr, "ERROR: Initiating MandelbrotGrid failed.\n");
        return 1;
    }

    if(compute_mandelbrot(&grid, num_iter, num_threads)) {
        fprintf(stderr, "ERROR: Computing MandelbrotGrid failed.\n");
        return 1;
    }

    if(save_grid(&grid, file)) {
        fprintf(stderr, "ERROR: Saving MandelbrotGrid failed.\n");
        return 1;
    }
    fclose(file);

    free_grid(&grid);    

    printf("Finished computation.");

    return 0;
}