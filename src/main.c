#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#ifndef SCALE
    #define SCALE "debug"
#endif

#ifndef NUMBER_ITERATIONS 
    #define NUMBER_ITERATIONS 100
#endif

#ifndef OUTPUT_DIRECTORY
    #define OUTPUT_DIRECTORY "../data"
#endif

#ifndef NUMBER_THREADS 
    #define NUMBER_THREADS 6
#endif


double imag_min = -1.5;
double imag_max = 1.5;
double real_min = -3.0;
double real_max = 1.5;


// complex numbers
typedef struct {
    double real;
    double imag;
} Complex;

Complex add(Complex a, Complex b){
    Complex tmp = {
        a.real + b.real,
        a.imag + b.imag
    };
    return tmp;
}

Complex square(Complex a){
    Complex tmp = {
        a.real*a.real - a.imag*a.imag,
        2*a.real*a.imag
    };
    return tmp;
}


Complex _get_complex_number(
    unsigned  int row, unsigned int col, unsigned int height, unsigned int width
){
    Complex tmp = {
        real_min + (real_max - real_min) * col / (width - 1),
        imag_min + (imag_max - imag_min) * row / (height - 1)
    };
    return tmp;
}


// threading
typedef struct {
    unsigned int row_start;
    unsigned int row_end;
    unsigned int heigth;
    unsigned int width;
    unsigned int* array;
} ThreadData;

void* worker(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    
    for(unsigned int i = data->row_start; i < data->row_end; ++i) {
        for(unsigned int j = 0; j < data->width; ++j) {
            bool bounded = true;
            Complex z = {0.0, 0.0};
            Complex c = _get_complex_number(i, j, data->heigth, data->width);

            for(unsigned int k = 0; k < NUMBER_ITERATIONS; ++k) {
                z = add(square(z), c);

                if(z.real * z.real + z.imag * z.imag > 4) {
                    data->array[i * data->width + j] = k;
                    bounded = false;
                    break;
                }
            }

            if(bounded) {
                data->array[i * data->width + j] = NUMBER_ITERATIONS;
            }
        }
    }

    return NULL;
}



int main(){

    unsigned int height;
    if(strcmp(SCALE, "debug") == 0) {
        height = 350; 
    } else if(strcmp(SCALE, "small") == 0) {
        height = 4000;
    } else if(strcmp(SCALE, "medium") == 0) {
        height = 8000;
    } else if(strcmp(SCALE, "large") == 0) {
        height = 12000;
    } else {
        perror("Unkown scale");
        return 1;
    }

    unsigned int width = (unsigned int)((double)height * 3/2);

    // generate data
    unsigned int *image;
    image = malloc(height * width * sizeof(unsigned int));
    if (image == NULL) {
        perror("malloc failed\n");
        return 1;
    }

    pthread_t threads[NUMBER_THREADS];
    ThreadData data[NUMBER_THREADS];

    unsigned int chunk = (height + NUMBER_THREADS - 1) / NUMBER_THREADS;
    for(unsigned int t = 0; t < NUMBER_THREADS; ++t) {
        data[t].row_start = t*chunk;
        data[t].row_end = (t < NUMBER_THREADS-1) ? (t+1)*chunk : height;
        data[t].heigth = height;
        data[t].width = width;
        data[t].array = image;

        pthread_create(&threads[t], NULL, worker, &data[t]);
    }

    for(unsigned int t = 0; t < NUMBER_THREADS; ++t) {
        pthread_join(threads[t], NULL);
    }


    // save data
    char filename[100];
    snprintf(
        filename, 
        sizeof(filename), 
        "%s/array_%s.txt", 
        OUTPUT_DIRECTORY, 
        SCALE
    );
    FILE *file = fopen(filename, "w");
    if (file == NULL){
        perror("directory not found\n");
        return 1;
    }

    fprintf(file, "%u\t%u\t%u\n", height, width, NUMBER_ITERATIONS);
    for(unsigned int i = 0; i < height; i++){
        for(unsigned int j = 0; j < width; j++){
            fprintf(file, "%u\t", image[i*width +  j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    free(image);

    return 0;
}