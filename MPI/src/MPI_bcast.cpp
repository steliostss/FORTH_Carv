#include <iostream> 
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"

#define MESSAGES_TO_SENT 2

int main (int argc, char** argv) {

    MPI::Init(argc, argv);
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();

    int data(0);
    int count(0);
    int message_counter(0);

    while (message_counter++ < MESSAGES_TO_SENT) {
        if (world_rank == 0) {
            // If we are the root process, send our data to everyone
            for (int i = 1; i < world_size; i++) {
                if (i != world_rank) {
                    MPI::COMM_WORLD.Isend(&data, count, MPI_INT, i, i);
                    std::cout << "Root: " << world_rank << " just sent a message to: ";
                    std::cout << i << ", with tag: " << i << "..." << std::endl;
                }
            }
        }
        else {
            for (int i=1; i< world_size; i++) {
                // If we are a receiver process, receive the data from the root
                MPI::COMM_WORLD.Irecv(&data, count, MPI_INT, 0, i);
                std::cout << "Process: " << world_rank << " just received a message from root...";
                std::cout << ", with tag: " << i << "..." << std::endl;
            }
        }
    }
    MPI::Finalize();
    return 0;
}