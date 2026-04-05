#include <stdio.h>
#include "../include/cli.h"

int main(int argc, char* argv[]){

    Configs configs;
    set_defaults(&configs);

    int rc = parse_args(&configs, argc, argv);
    if(rc == 1) {return 0;}
    if(rc == -1) {return 1;}

    if(!validate_configs(&configs)) {return 1;}

    if(!finalize_configs(&configs)) {return 1;}

    if (!configs.assume_yes && !confirm_configs(&configs)){return 0;}

    return run_program(&configs);
}