#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/cli.c"

static void test_set_defaults(void) {
    Configs cfg;
    set_defaults(&cfg);

    assert(cfg.real_min == -3.5);
    assert(cfg.real_max == 1.5);
    assert(cfg.imag_min == -1.5);
    assert(cfg.imag_max == 1.5);
    assert(strcmp(cfg.grid_size, "small") == 0);
    assert(cfg.num_threads == 2);
    assert(cfg.num_iter == 1000);
    assert(cfg.output_path == NULL);
    assert(cfg.assume_yes == 0);
}

static void test_parse_args_valid(void) {
    Configs cfg;
    set_defaults(&cfg);

    char *argv[] = {
        "test_cli",
        "--real_min=-2.0",
        "--real_max=0.5",
        "--imag_min=-1.0",
        "--imag_max=1.0",
        "--grid_size=medium",
        "--num_threads=4",
        "--num_iter=250",
        "--output_path=result.txt",
        "--yes"
    };
    int argc = (int)(sizeof(argv) / sizeof(argv[0]));

    optind = 1;
    assert(parse_args(&cfg, argc, argv) == 0);

    assert(cfg.real_min == -2.0);
    assert(cfg.real_max == 0.5);
    assert(cfg.imag_min == -1.0);
    assert(cfg.imag_max == 1.0);
    assert(strcmp(cfg.grid_size, "medium") == 0);
    assert(cfg.num_threads == 4);
    assert(cfg.num_iter == 250);
    assert(strcmp(cfg.output_path, "result.txt") == 0);
    assert(cfg.assume_yes == 1);
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

static void test_validate_configs_valid(void) {
    Configs cfg;
    set_defaults(&cfg);

    assert(validate_configs(&cfg) == 1);
}

static void test_validate_configs_invalid_real_range(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.real_min = 1.0;
    cfg.real_max = 1.0;
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_imag_range(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.imag_min = 2.0;
    cfg.imag_max = 1.0;
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_grid_size(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.grid_size = "debug";
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_num_threads(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.num_threads = 0;
    assert(validate_configs(&cfg) == 0);
}

static void test_validate_configs_invalid_num_iter(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.num_iter = 0;
    assert(validate_configs(&cfg) == 0);
}

static void test_finalize_configs_small_default_path(void) {
    Configs cfg;
    set_defaults(&cfg);

    assert(finalize_configs(&cfg) == 1);
    assert(cfg.size == GRID_SMALL);
    assert(strcmp(cfg.path, "./small.txt") == 0);
}

static void test_finalize_configs_medium_custom_path(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.grid_size = "medium";
    cfg.output_path = "my_output.txt";

    assert(finalize_configs(&cfg) == 1);
    assert(cfg.size == GRID_MEDIUM);
    assert(strcmp(cfg.path, "my_output.txt") == 0);
}

static void test_finalize_configs_large(void) {
    Configs cfg;
    set_defaults(&cfg);

    cfg.grid_size = "large";

    assert(finalize_configs(&cfg) == 1);
    assert(cfg.size == GRID_LARGE);
    assert(strcmp(cfg.path, "./large.txt") == 0);
}

int main(void) {
    printf(">>>>>> Starting tests in 'test_cli.c'...\n");
    printf("------ default-tests\n");
    test_set_defaults();

    printf("------ parse-tests\n");
    test_parse_args_valid();
    test_parse_args_invalid_double();
    test_parse_args_invalid_uint();

    printf("----- validation-tests\n");
    test_validate_configs_valid();
    test_validate_configs_invalid_real_range();
    test_validate_configs_invalid_imag_range();
    test_validate_configs_invalid_grid_size();
    test_validate_configs_invalid_num_threads();
    test_validate_configs_invalid_num_iter();

    printf("----- finalize-tests\n");
    test_finalize_configs_small_default_path();
    test_finalize_configs_medium_custom_path();
    test_finalize_configs_large();

    printf("<<<<< All tests in 'test_cli.c' passed.\n");
    return 0;
}