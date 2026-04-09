#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#include "../include/cli.h"
#include "../include/export.h"

/********** cli helper **********/

/**
 * print help text
 * @param program_name
 *      name of executable
 * @param cfg
 *      default configs 
 */
static void print_help(const char* program_name, const Configs* cfg) {

    printf("Usage:\n");
    printf("  %s -x {image|data} [options]\n\n", program_name);
    printf("  %s --output {image|data} [options]\n\n", program_name);
    printf("Options:\n");
    printf("  %-35s %s\n", "--real_min <float>", "Right boundary in complex plane");
    printf("  %-35s default: %f\n\n", "", cfg->real_min);
    printf("  %-35s %s\n", "--real_max <float>", "Left boundary in complex plane");
    printf("  %-35s default: %f\n\n", "", cfg->real_max);
    printf("  %-35s %s\n", "--imag_min <float>", "Lower boundary in complex plane");
    printf("  %-35s default: %f\n\n", "", cfg->imag_min);
    printf("  %-35s %s\n", "--imag_max <float>", "Upper boundary in complex plane");
    printf("  %-35s default: %f\n\n", "", cfg->imag_max);
    printf("  %-35s %s\n", "-r, --resolution {hd|fullhd|4k}", "Vertical resolution (width adapts to aspect ratio)");
    printf("  %-35s default: %s\n\n", "", cfg->resolution);
    printf("  %-35s %s\n", "-t, --num_threads <uint>", "Number of threads");
    printf("  %-35s default: %u\n\n", "", cfg->num_threads);
    printf("  %-35s %s\n", "-i, --num_iter <uint>", "Iterations per pixel");
    printf("  %-35s default: %u\n\n", "", cfg->num_iter);
    printf("  %-35s %s\n", "-f, --format {png|ppm}", "Output image format");
    printf("  %-35s default: %s\n\n", "", cfg->format);
    printf("  %-35s %s\n", "-c, --cmap {blue|darksouls|gray|green|inferno|purple|red|sasuke|yellow}", "Color theme");
    printf("  %-35s default: %s\n\n", "", cfg->cmap);
    printf("  %-35s %s\n", "-o, --output_path <string>","Output path (./mandelbrot_[resolution].[ppm|png|txt])");
    printf("  %-35s %s\n", "-y, --yes", "Skip confirmation prompt");
    printf("  %-35s %s\n", "-h, --help", "Show this help message");

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


/********** cli api **********/

void set_defaults(Configs* cfg) {
    cfg->real_min = -3.4;
    cfg->real_max = 1.4;
    cfg->imag_min = -1.5;
    cfg->imag_max = 1.5;

    cfg->resolution = "hd";

    cfg->num_threads = 2;
    cfg->num_iter = 1000;

    cfg->output = NULL;
    cfg->output_path = NULL;
    cfg->format = "png";
    cfg->cmap = "red";

    cfg->assume_yes = 0;
}

int parse_args(Configs* cfg, int  argc, char **argv) {
    // option handling
    struct option long_options[] = {
        {"real_min", required_argument, 0, 'a'},
        {"real_max", required_argument, 0, 'b'},
        {"imag_min", required_argument, 0, 'd'},
        {"imag_max", required_argument, 0, 'e'},
        {"resolution", required_argument, 0, 'r'},
        {"num_threads", required_argument, 0, 't'},
        {"num_iter", required_argument, 0, 'i'},
        {"help", no_argument, 0, 'h'},
        {"output", required_argument, 0, 'x'},
        {"format", required_argument, 0, 'f'},
        {"cmap", required_argument, 0, 'c'},
        {"output_path", required_argument, 0, 'o'},
        {"yes", no_argument, 0, 'y'},
        {0, 0, 0, 0}
    };

    int opt;
    while((opt = getopt_long(argc, argv, "r:t:i:x:f:c:ho:y", long_options, NULL)) != -1) {
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

        case 'd':
            if(!parse_double(optarg, &cfg->imag_min)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --imag_min.\n", optarg);
                return -1;
            }
            break;

        case 'e':
            if(!parse_double(optarg, &cfg->imag_max)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --imag_max.\n", optarg);
                return -1;
            }
            break;

        case 'r':
            cfg->resolution = optarg;
            break;
        
        case 't':
            if(!parse_uint(optarg, &cfg->num_threads)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --num_threads.\n", optarg);
                return -1;
            }
            break;

        case 'i':
            if(!parse_uint(optarg, &cfg->num_iter)) {
                fprintf(stderr, "ERROR: Invalid value '%s' for --num_iter.\n", optarg);
                return -1;
            }
            break;

        case 'h':
            print_help(argv[0], cfg);
            return 1;

        case 'x':
            cfg->output = optarg;
            break;
        
        case 'f':
            cfg->format = optarg;
            break;

        case 'c':
            cfg->cmap = optarg;
            break;

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
    if(cfg->output == NULL) {
        fprintf(stderr, "ERROR: '--output' is required (not optional).\n");
        return 0;
    }

    if(cfg->real_min >= cfg->real_max) {
        fprintf(stderr, "ERROR: '--real_min' must be smaller than real_max.\n");
        return 0;
    }
    if(cfg->imag_min >= cfg->imag_max) {
        fprintf(stderr, "ERROR: '--imag_min' must be smaller than imag_max.\n");
        return 0;
    }
    if(
        strcmp(cfg->resolution, "hd") != 0 &&
        strcmp(cfg->resolution, "fullhd") != 0 &&
        strcmp(cfg->resolution, "4k") != 0
    ) {
        fprintf(stderr, "ERROR: invalid '--resolution': '%s'.\n", cfg->resolution);
        return 0;
    }
    if(cfg->num_threads == 0) {
        fprintf(stderr, "ERROR: '--num_threads' must be greater than 0.\n");
        return 0;
    }
    if(cfg->num_iter == 0) {
        fprintf(stderr, "ERROR: '--num_iter' must be greater than 0.\n");
        return 0;
    }
    if(
        strcmp(cfg->output, "image") != 0 &&
        strcmp(cfg->output, "data") != 0
    ) {
        fprintf(stderr, "ERROR: invalid '--output': '%s'.\n", cfg->output);
        return 0;
    }
    if(
        strcmp(cfg->format, "png") != 0 &&
        strcmp(cfg->format, "ppm") != 0
    ) {
        if(strcmp(cfg->format, "txt") != 0 || strcmp(cfg->output, "data") != 0) {
            fprintf(stderr, "ERROR: invalid '--format': '%s'.\n", cfg->format);
            return 0;
        }
    }
    if(
        strcmp(cfg->cmap, "blue") != 0 &&
        strcmp(cfg->cmap, "darksouls") != 0 &&
        strcmp(cfg->cmap, "gray") != 0 &&
        strcmp(cfg->cmap, "green") != 0 &&
        strcmp(cfg->cmap, "inferno") != 0 &&
        strcmp(cfg->cmap, "purple") != 0 &&
        strcmp(cfg->cmap, "red") != 0 &&
        strcmp(cfg->cmap, "sasuke") != 0 &&
        strcmp(cfg->cmap, "yellow") != 0 

    ) {
        fprintf(stderr, "ERROR: invalid '--cmap': '%s'.\n", cfg->cmap);
        return 0;
    }

    return 1;
}

int finalize_configs(Configs* cfg) {
    // resolution
    if(strcmp(cfg->resolution, "4k") == 0) {cfg->size = GRID_LARGE;} 
    else if(strcmp(cfg->resolution, "fullhd") == 0) {cfg->size = GRID_MEDIUM;} 
    else {cfg->size = GRID_SMALL;}



    // format
    if(strcmp(cfg->output, "data") == 0) {
        cfg->format = "txt";   
    }

    // output path
    int rc;
    if(cfg->output_path == NULL) {
        rc = snprintf(cfg->path, sizeof(cfg->path), "./mandelbrot_%s.%s", cfg->resolution, cfg->format);        
    } else {
        rc = snprintf(cfg->path, sizeof(cfg->path), "%s", cfg->output_path);
    }
    if(rc < 0 || (size_t)rc >= sizeof(cfg->path)) {
        fprintf(stderr, "ERROR: output path is too long.\n");
        return 0;
    }

    // cmap
    if(strcmp(cfg->cmap, "blue") == 0) {cfg->colormap = COLORMAP_BLUE;}
    else if(strcmp(cfg->cmap, "darksouls") == 0) {cfg->colormap = COLORMAP_DARKSOULS;}
    else if(strcmp(cfg->cmap, "gray") == 0) {cfg->colormap = COLORMAP_GRAY;}
    else if(strcmp(cfg->cmap, "green") == 0) {cfg->colormap = COLORMAP_GREEN;}
    else if(strcmp(cfg->cmap, "inferno") == 0) {cfg->colormap = COLORMAP_INFERNO;}
    else if(strcmp(cfg->cmap, "purple") == 0) {cfg->colormap = COLORMAP_PURPLE;}
    else if(strcmp(cfg->cmap, "sasuke") == 0) {cfg->colormap = COLORMAP_SASUKE;}
    else if(strcmp(cfg->cmap, "yellow") == 0) {cfg->colormap = COLORMAP_YELLOW;}
    else {cfg->colormap = COLORMAP_RED;}

    return 1;
}

int confirm_configs(const Configs* cfg) {
    printf("Configuration (%s):\n\n", cfg->output);
    printf("  %-28s %s\n", "Resolution:", cfg->resolution);
    printf("  %-28s (%f, %f)\n", "Real range:", cfg->real_min, cfg->real_max);
    printf("  %-28s (%f, %f)\n", "Imaginary range:", cfg->imag_min, cfg->imag_max);
    printf("  %-28s %u\n", "Threads:", cfg->num_threads);
    printf("  %-28s %u\n", "Iterations:", cfg->num_iter);
    if (strcmp(cfg->output, "image") == 0) {
        printf("  %-28s %s\n", "Colormap:", cfg->cmap);
        printf("  %-28s %s\n", "Format:", cfg->format);
    }
    printf("  %-28s %s\n", "Output path:", cfg->path);
    printf("\nContinue? [y/N]: ");

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
    int rc = 1;

    int is_image = strcmp(cfg->output, "image") == 0;
    int is_png   = strcmp(cfg->format, "png") == 0;

    FILE* file = NULL;
    MandelbrotGrid grid = {0};
    MandelbrotImage image = {0};

    printf("Starting calculations...\n");

    /* open file if needed */
    if(!is_png) {
        file = fopen(cfg->path, "w");
        if(file == NULL) {
            fprintf(stderr, "ERROR: Opening file %s failed.\n", cfg->path);
            goto cleanup;
        }
    }

    /* calculate grid */
    if(init_grid(&grid, cfg->size, cfg->real_min, cfg->imag_min, cfg->real_max, cfg->imag_max)) {
        fprintf(stderr, "ERROR: Initiating MandelbrotGrid failed.\n");
        goto cleanup;
    }

    if(compute_mandelbrot(&grid, cfg->num_iter, cfg->num_threads)) {
        fprintf(stderr, "ERROR: Computing MandelbrotGrid failed.\n");
        goto cleanup;
    }

    printf("Finished calculation.\n");

    /* render image if requested */
    if(is_image) {
        if(init_image(&image, grid.height, grid.width)) {
            fprintf(stderr, "ERROR: Initiating MandelbrotImage failed.\n");
            goto cleanup;
        }

        if(render_mandelbrot(&image, &grid, cfg->colormap)) {
            fprintf(stderr, "ERROR: Rendering MandelbrotImage failed.\n");
            goto cleanup;
        }

        /* grid no longer needed after rendering */
        free_grid(&grid);
    }

    /* export */
    if(is_image) {
        if(is_png) {
            if(save_image_png(&image, cfg->path)) {
                fprintf(stderr, "ERROR: Exporting MandelbrotImage to 'png' failed.\n");
                goto cleanup;
            }
        } else {
            if(save_image_ppm(&image, file)) {
                fprintf(stderr, "ERROR: Exporting MandelbrotImage to 'ppm' failed.\n");
                goto cleanup;
            }
        }
    } else {
        if(save_grid_txt(&grid, file)) {
            fprintf(stderr, "ERROR: Exporting MandelbrotGrid to 'txt' failed.\n");
            goto cleanup;
        }
    }

    rc = 0;

    cleanup:
        free_grid(&grid);
        if(is_image) {
            free_image(&image);
        }
        if(file != NULL) {
            fclose(file);
        }

    return rc;
}