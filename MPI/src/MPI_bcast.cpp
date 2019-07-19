#include <iostream> 
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"

int main (int argc, char** argv) {

    // Initialize the MPI environment
    // MPI_Init(&argc, &argv);
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    double start = MPI_Wtime();
    int number;

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Get_processor_name(processor_name, &name_len);

    double end = MPI_Wtime();
   
    usleep(60);
    // Print off a hello world message
    std::cout << "Hello world from processor " << processor_name;
    std::cout << ", rank " << world_rank;
    std::cout << " out of " << world_size << std::endl;
    std::cout << "The process took " << end - start << " seconds to run." << std::endl;
    fflush(stdout);

   // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}