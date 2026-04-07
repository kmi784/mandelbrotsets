#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../src/cli.c"

static void test_set_defaults(void) {
    Configs cfg;
    set_defaults(&cfg);

    assert(cfg.real_min == -3.4);
    assert(cfg.real_max == 1.4);
    assert(cfg.imag_min == -1.5);
    assert(cfg.imag_max == 1.5);

    assert(strcmp(cfg.resolution, "hd") == 0);
    assert(cfg.num_threads == 2);
    assert(cfg.num_iter == 1000);

    assert(cfg.output == NULL);
    assert(cfg.output_path == NULL);
    assert(strcmp(cfg.format, "png") == 0);
    assert(strcmp(cfg.cmap, "red") == 0);

    assert(cfg.assume_yes == 0);
}

static void test_parse_args_valid_image(void) {
    Configs cfg;
    set_defaults(&cfg);

    char *argv[] = {
        "test_cli",
        "--real_min=-2.0",
        "--real_max=0.5",
        "--imag_min=-1.0",
        "--imag_max=1.0",
        "--resolution=fullhd",
        "--num_threads=4",
        "--num_iter=250",
        "--output=image",
        "--format=ppm",
        "--cmap=gray",
        "--output_path=result.ppm",
        "--yes"
    };
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    optind = 1;
    assert(parse_args(&cfg, argc, argv) == 0);

    assert(cfg.real_min == -2.0);
    assert(cfg.real_max == 0.5);
    assert(cfg.imag_min == -1.0);
    assert(cfg.imag_max == 1.0);
    assert(strcmp(cfg.resolution, "fullhd") == 0);
    assert(cfg.num_threads == 4);
    assert(cfg.num_iter == 250);
    assert(strcmp(cfg.output, "image") == 0);
    assert(strcmp(cfg.format, "ppm") == 0);
    assert(strcmp(cfg.cmap, "gray") == 0);
    assert(strcmp(cfg.output_path, "result.ppm") == 0);
    assert(cfg.assume_yes == 1);
}

static void test_parse_args_valid_data(void) {
    Configs cfg;
    set_defaults(&cfg);

    char *argv[] = {
        "test_cli",
        "--output=data",
        "--resolution=hd",
        "--output_path=result.txt"
    };
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    optind = 1;
    assert(parse_args(&cfg, argc, argv) == 0);

    assert(strcmp(cfg.output, "data") == 0);
    assert(strcmp(cfg.resolution, "hd") == 0);
    assert(strcmp(cfg.output_path, "result.txt") == 0);
}

static void test_parse_args_invalid_double(void) {
    Configs cfg;
    set_defaults(&cfg);

    char *argv[] = {
        "test_cli",
        "--real_min=abc"
    };
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    optind = 1;
    assert(parse_args(&cfg, argc, argv) == -1);
}

static void test_parse_args_invalid_uint(void) {
    Configs cfg;
    set_defaults(&cfg);

    char *argv[] = {
        "test_cli",
        "--num_threads=abc"
    };
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    optind = 1;
    assert(parse_args(&cfg, argc, argv) == -1);
}

static void test_validate_configs_valid_image(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    assert(validate_configs(&cfg) == 1);
}

static void test_validate_configs_valid_data_with_txt(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "data";
    cfg.format = "txt";
    assert(validate_configs(&cfg) == 1);
}

static void test_validate_configs_missing_output(void) {
    Configs cfg;
    set_defaults(&cfg);

    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_real_range(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.real_min = 1.0;
    cfg.real_max = 1.0;
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_imag_range(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.imag_min = 2.0;
    cfg.imag_max = 1.0;
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_resolution(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.resolution = "debug";
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_num_threads(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.num_threads = 0;
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_num_iter(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.num_iter = 0;
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_output(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "grid";
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_format_for_image(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.format = "txt";
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_cmap(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.cmap = "debug";
    assert(validate_configs(&cfg) == 0);
}

static void test_finalize_configs_image_hd_default_path(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";

    assert(finalize_configs(&cfg) == 1);
    assert(cfg.size == GRID_SMALL);
    assert(strcmp(cfg.format, "png") == 0);
    assert(strcmp(cfg.path, "./mandelbrot_hd.png") == 0);
    assert(cfg.colormap == COLORMAP_RED);
}

static void test_finalize_configs_image_fullhd_custom_path(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.resolution = "fullhd";
    cfg.format = "ppm";
    cfg.cmap = "gray";
    cfg.output_path = "my_output.ppm";

    assert(finalize_configs(&cfg) == 1);
    assert(cfg.size == GRID_MEDIUM);
    assert(strcmp(cfg.path, "my_output.ppm") == 0);
    assert(cfg.colormap == COLORMAP_GRAY);
}

static void test_finalize_configs_data_forces_txt(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "data";
    cfg.format = "png";

    assert(finalize_configs(&cfg) == 1);
    assert(cfg.size == GRID_SMALL);
    assert(strcmp(cfg.format, "txt") == 0);
    assert(strcmp(cfg.path, "./mandelbrot_hd.txt") == 0);
}

static void test_finalize_configs_4k_sets_large(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.resolution = "4k";

    assert(finalize_configs(&cfg) == 1);
    assert(cfg.size == GRID_LARGE);
}

static void test_run_program_data_txt(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "data";
    cfg.resolution = "hd";
    cfg.num_threads = 1;
    cfg.num_iter = 10;
    cfg.output_path = "./test_output_data.txt";

    assert(finalize_configs(&cfg) == 1);
    assert(run_program(&cfg) == 0);

    FILE *f = fopen(cfg.path, "r");
    assert(f != NULL);
    fclose(f);

    remove(cfg.path);
}

static void test_run_program_image_ppm(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.output = "image";
    cfg.resolution = "hd";
    cfg.format = "ppm";
    cfg.cmap = "red";
    cfg.num_threads = 1;
    cfg.num_iter = 10;
    cfg.output_path = "./test_output_image.ppm";

    assert(finalize_configs(&cfg) == 1);
    assert(run_program(&cfg) == 0);

    FILE *f = fopen(cfg.path, "rb");
    assert(f != NULL);
    fclose(f);

    remove(cfg.path);
}

int main(void) {
    printf(">>>>>> Starting tests in 'test_cli.c'...\n");

    printf("------ default-tests\n");
    test_set_defaults();

    printf("------ parse-tests\n");
    test_parse_args_valid_image();
    test_parse_args_valid_data();
    test_parse_args_invalid_double();
    test_parse_args_invalid_uint();

    printf("------ validation-tests\n");
    test_validate_configs_valid_image();
    test_validate_configs_valid_data_with_txt();
    test_validate_configs_missing_output();
    test_validate_configs_invalid_real_range();
    test_validate_configs_invalid_imag_range();
    test_validate_configs_invalid_resolution();
    test_validate_configs_invalid_num_threads();
    test_validate_configs_invalid_num_iter();
    test_validate_configs_invalid_output();
    test_validate_configs_invalid_format_for_image();
    test_validate_configs_invalid_cmap();

    printf("------ finalize-tests\n");
    test_finalize_configs_image_hd_default_path();
    test_finalize_configs_image_fullhd_custom_path();
    test_finalize_configs_data_forces_txt();
    test_finalize_configs_4k_sets_large();

    printf("------ run-tests\n");
    test_run_program_data_txt();
    test_run_program_image_ppm();

    printf("<<<<<< All tests in 'test_cli.c' passed.\n\n");
    return 0;
}