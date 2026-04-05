#ifndef CLI_H
#define CLI_H

#include "mandelbrot.h"

/**
 * Configuration object storing parameters for mandelbrot grid computation
 */
typedef struct {
    // ranges
    double real_min;
    double real_max;
    double imag_min;
    double imag_max;
    
    // grid
    const char* grid_size;
    GridSize size;

    // computation
    unsigned int num_threads;
    unsigned int num_iter;
    
    // output
    const char* output_path;
    char path[256];

    // misc
    int assume_yes;
} Configs;

/**
 * set default parameters in `Configs` object
 * @param cfg 
 *      pointer to `Configs` object
 */
void set_defaults(Configs* cfg);

/**
 * reads passed arguments (main-entry) and writes them into `Config` object
 * @param cfg 
 *      pointer to `Configs` object
 * @param argc   
 *      argument count
 * @param argv 
 *      argument vector
 * @return `0` on success, `-1` on failure, `1` end program    
 */
int parse_args(Configs* cfg, int  argc, char **argv);

/**
 * validates configuration in passed `Config` object
 * @param cfg 
 *      pointer to `Configs` object
 * @return `1` valid configs, `0` invalid configs
 */
int validate_configs(const Configs* cfg);

/**
 * finalize configuration for computation. In particular `Configs->size` and 
 * `Configs->path` will be set.
 * @param cfg
 *      pointer to `Configs` object
 * @return `1` success, `0` failure
 */
int finalize_configs(Configs* cfg);

/**
 * requests configuration confirmation from user
 * @param cfg 
 *      pointer to `Configs` object
 * @param argc   
 *      argument count
 * @return `1` continue, `0` interrupt
 */
int confirm_configs(const Configs* cfg);

/**
 * runs the mandelbrot routine
 * @param cfg 
 *      pointer to `Configs` object
 * @return `0` success, `1` failure
 */
int run_program(const Configs* cfg);

#endif