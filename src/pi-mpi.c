#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMSTEPS 1000000

// Returns Area of rectangle 
// stepsPerProcessor: amount of NUMSTEPS assigned to each processor
// base: base length of rectangle
// proc_id: id of processor
double rect(double stepsPerProcessor, double base, int proc_id) {
  double x, y, area;
  
  if(proc_id == 0){  // proc_id:0 will start calculating from 0
    x = 0;
  } else {  // other proc_id's will start at respective ranges
    x = proc_id * stepsPerProcessor * base; 
  }
  
  for(int i = 1; i <= stepsPerProcessor; i++) {  // iterate through steps assigned and find local area
    x = x + base;
    y += (4/(1+(x*x)));
  }
  area = base * y;
  return area;
}

int main(int argc, char** argv) {
    
  int num_proc, proc_id, root_proc = 0, n = NUMSTEPS;
  double pi, a = 0.0, b = 1.0, local_sum = 0.0, base, x;
  struct timespec start, end;

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  
  // Get start time of calculation
  clock_gettime(CLOCK_MONOTONIC, &start);

  // Get the number of processes and assign id
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
  
  base = (b-a)/n; // base of rectangle
  double epg = n/num_proc; // EveryProcessorGets(epg) amount of steps every processor must calculate
  local_sum = rect(epg, base, proc_id); //area calculated by each processor
  
  // Send local_sum to proc_id:0 
  if(proc_id != 0) {
      MPI_Send(&local_sum, 1, MPI_DOUBLE, root_proc, 0, MPI_COMM_WORLD);
  } else { // Iterate through number of processes and add all areas
      pi = local_sum;
      for(int i = 1; i < num_proc; i++) {
        MPI_Recv(&local_sum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        pi += local_sum;
      }
  }
  
  // Print Results
  if(proc_id == 0){
    // Get end time and calculate elapsed time in nanoseconds
    clock_gettime(CLOCK_MONOTONIC, &end);
    u_int64_t diff = 1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    
    // Print results of Integral PI and elapsed time
    printf("PI is %.20f\n",pi);                                                         
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
  }  
  
  // Finalize the MPI environment
  MPI_Finalize();
}
