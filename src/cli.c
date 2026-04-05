#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "../include/cli.h"
#include "../include/export.h"

void set_defaults(Configs* cfg) {
    cfg->real_min = -3.5;
    cfg->real_max = 1.5;
    cfg->imag_min = -1.5;
    cfg->imag_max = 1.5;
    cfg->grid_size = "small";
    cfg->num_threads = 2;
    cfg->num_iter = 1000;
    cfg->output_path = NULL;
    cfg->assume_yes = 0;
}

/**
 * print help text
 * @param program_name
 *      name of executable
 * @param cfg
 *      default configs 
 */
static void print_help(const char* program_name, const Configs* cfg) {
    printf("Usage: %s [options]\n", program_name);
    printf("  --grid_size=<string>   allowed grid sizes: small|medium|large (default: %s)\n", cfg->grid_size);
    printf("  --real_min=<float>     right boundary in complex plain (default: %f)\n", cfg->real_min);
    printf("  --real_max=<float>     left boundary in complex plain (default: %f)\n", cfg->real_max);
    printf("  --imag_min=<float>     lower boundary in complex plain (default: %f)\n", cfg->imag_min);
    printf("  --imag_max=<float>     upper boundary in complex plain (default: %f)\n", cfg->imag_max);
    printf("  --num_threads=<uint>   number of threads (default: %u)\n", cfg->num_threads);
    printf("  --num_iter=<uint>      maximal amount of iterations per pixel (default: %u)\n", cfg->num_iter);  
    printf("  -o, --output_path      output path (default: ./[grid_size].txt)\n");
    printf("  -y, --yes              skip confirmation prompt (for scripts)\n");
    printf("  -h, --help             help\n");
}

/**
 * parses string argument to unsigned integer
 * @param s
 *      pointer to argument which should parsed 
 * @param 
 *      pointer to output unsigned integer
 * @return 
 *      `1` success ,`0` failure
 */   
static int parse_uint(const char* s, unsigned int* out){
    char* end;
    unsigned long value = strtoul(s, &end, 10);
    if(end==s) {return 0;}
    if(*end != '\0') {return 0;}
    *out = (unsigned int)value;
    return 1; 
}

/**
 * parses string argument to double
 * @param s
 *      pointer to argument which should parsed 
 * @param 
 *      pointer to output double
 * @return 
 *      `1` success ,`0` failure
 */
static int parse_double(const char* s, double* out) {
    char* end;
    double value = strtod(s, &end);
    if(end==s) {return 0;}
    if(*end != '\0') {return 0;}
    *out = value;
    return 1;
}

int parse_args(Configs* cfg, int  argc, char **argv) {
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
        {"output_path", required_argument, 0, 'o'},
        {"yes", no_argument, 0, 'y'},
        {0, 0, 0, 0}
    };

    int opt;
    while((opt = getopt_long(argc, argv, "ho:y", long_options, NULL)) != -1) {
        switch(opt) {
        case 'a':
            if(!parse_double(optarg, &cfg->real_min)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --real_min.\n", optarg);
                return -1;
            }
            break;

        case 'b':
            if(!parse_double(optarg, &cfg->real_max)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --real_max.\n", optarg);
                return -1;
            }
            break;

        case 'c':
            if(!parse_double(optarg, &cfg->imag_min)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --imag_min.\n", optarg);
                return -1;
            }
            break;

        case 'd':
            if(!parse_double(optarg, &cfg->imag_max)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --imag_max.\n", optarg);
                return -1;
            }
            break;

        case 'e':
            cfg->grid_size = optarg;
            break;
        
        case 'f':
            if(!parse_uint(optarg, &cfg->num_threads)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --num_threads.\n", optarg);
                return -1;
            }
            break;

        case 'g':
            if(!parse_uint(optarg, &cfg->num_iter)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --num_iter.\n", optarg);
                return -1;
            }
            break;

        case 'h':
            print_help(argv[0], cfg);
            return 1;
        
        case 'o':
            cfg->output_path = optarg;
            break;

        case 'y':
            cfg->assume_yes = 1;
            break;

        default:
            fprintf(
                stderr, "ERROR: Invalid option, see -h, --help for more information.\n"
            );
            return -1;
        }
    }
    return 0;
}

int validate_configs(const Configs* cfg) {
    if(cfg->real_min >= cfg->real_max) {
        fprintf(stderr, "ERROR: real_min must be smaller than real_max.\n");
        return 0;
    }
    if(cfg->imag_min >= cfg->imag_max) {
        fprintf(stderr, "ERROR: imag_min must be smaller than imag_max.\n");
        return 0;
    }
    if(
        strcmp(cfg->grid_size, "small") != 0 &&
        strcmp(cfg->grid_size, "medium") != 0 &&
        strcmp(cfg->grid_size, "large") != 0
    ) {
        fprintf(stderr, "ERROR: invalid grid_size '%s'.\n", cfg->grid_size);
        return 0;
    }
    if(cfg->num_threads == 0) {
        fprintf(stderr, "ERROR: num_threads must be greater than 0.\n");
        return 0;
    }
    if(cfg->num_iter == 0) {
        fprintf(stderr, "ERROR: num_iter must be greater than 0.\n");
        return 0;
    }
    return 1;
}

int finalize_configs(Configs* cfg) {
    if(strcmp(cfg->grid_size, "small") == 0) {
        cfg->size = GRID_SMALL;
    } else if(strcmp(cfg->grid_size, "medium") == 0) {
        cfg->size = GRID_MEDIUM;
    } else {
        cfg->size = GRID_LARGE;
    }

    int rc;
    if(cfg->output_path == NULL) {
        rc = snprintf(cfg->path, sizeof(cfg->path), "./%s.txt", cfg->grid_size);
    } else {
        rc = snprintf(cfg->path, sizeof(cfg->path), "%s", cfg->output_path);
    }

    if(rc < 0 || (size_t)rc >= sizeof(cfg->path)) {
        fprintf(stderr, "ERROR: output path is too long.\n");
        return 0;
    }

    return 1;
}

int confirm_configs(const Configs* cfg) {
    printf("Current configs:\n");
    printf("  - grid size:            %s\n", cfg->grid_size);
    printf("  - real part range:      (%f, %f)\n", cfg->real_min, cfg->real_max);
    printf("  - imaginary part range: (%f, %f)\n", cfg->imag_min, cfg->imag_max);
    printf("  - number of threads:    %u\n", cfg->num_threads);
    printf("  - number of iterations: %u\n", cfg->num_iter);
    printf("  - output path:          %s\n", cfg->path);
    printf("Continue? [y/N]: ");

    int c = getchar(); // reads only first character
    while (getchar() != '\n' && !feof(stdin)); // reads remaining character until '\n' or EOF
    if(c != 'y' && c != 'Y') {
        printf("Process interrupted.\n");
        return 0;
    } else {
        return 1;
    }
}

int run_program(const Configs* cfg) {
    FILE* file = fopen(cfg->path, "w");
    if(file == NULL) {
        fprintf(stderr, "ERROR: Opening file %s failed.\n", cfg->path);
        return 1;
    }

    printf("Starting mandelbrot routine...\n");
    MandelbrotGrid grid;
    if(
        init_grid(
            &grid, cfg->size, cfg->real_min, cfg->imag_min, cfg->real_max, cfg->imag_max
        )
    ) {
        fclose(file);
        fprintf(stderr, "ERROR: Initiating MandelbrotGrid failed.\n");
        return 1;
    }
    if(compute_mandelbrot(&grid, cfg->num_iter, cfg->num_threads)) {
        fclose(file);
        free_grid(&grid);
        fprintf(stderr, "ERROR: Computing MandelbrotGrid failed.\n");
        return 1;
    }
    if(save_grid(&grid, file)) {
        fclose(file);
        free_grid(&grid);
        fprintf(stderr, "ERROR: Saving MandelbrotGrid failed.\n");
        return 1;
    }

    fclose(file);
    free_grid(&grid);
    printf("Finished mandelbrot routine.");
    return 0;
}