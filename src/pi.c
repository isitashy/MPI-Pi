#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUMSTEPS 1000000

int main() {
        int i;
        double x, pi, sum = 0.0;
        struct timespec start, end;

        clock_gettime(CLOCK_MONOTONIC, &start);
        double step = 1.0/(double) NUMSTEPS;
        x = 0.5 * step;

        for (i=0; i<= NUMSTEPS; i++) {
                x += step;
                sum += 4.0/(1.0+x*x);
        }
        pi = step * sum;
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
        
        //printf("x:%f\n", x);
        printf("PI is %.20f\n",pi);
        printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

        return 0;
}


