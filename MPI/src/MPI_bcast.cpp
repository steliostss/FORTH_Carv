#include <iostream> 
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"

#define MESSAGES_TO_SENT 20

int main (int argc, char** argv) {

    // Initialize the MPI environment
    // MPI_Init(&argc, &argv);
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    // Each process gets its own rank
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    // Count time for each process
    double start = MPI_Wtime();
    // int number_to_send;

    int message_counter = 0;
    while (message_counter < MESSAGES_TO_SENT) {
        if (world_rank == 0) {        // only root will broadcast
        // ------------------------------------------------------------------
        // TODO: make it so every process is able to send
        // ------------------------------------------------------------------
            for (int i=1; i<world_size; ++i) {
                MPI_Send(
                    &message_counter, /* Buffer from which to get data  */
                    1,                /* Count of data                  */
                    MPI_INT,          /* MPI_type of data               */
                    i,                /* Destination process            */
                    i,                /* Tag                            */
                    MPI_COMM_WORLD    /* Communicator                   */
                );
            }
        }
        else if ( get_world_rank == world_rank) {

        }
    }

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