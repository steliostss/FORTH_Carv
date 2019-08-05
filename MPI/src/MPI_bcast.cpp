#include <iostream>
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"
#include "/usr/include/mpi/mpi.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>
#include "cmd_line_args.h"

#define MESSAGES_TO_SENT 2

/*************************************************
 ****************** TYPEDEF **********************
 *************************************************/

typedef std::unique_ptr<int[]> uPtr_int;
typedef std::shared_ptr<int[]> sPtr_int;
typedef std::vector<int> intVec;
typedef std::vector<int>::iterator intVecIter;
typedef std::unique_ptr<intVec> uPtr_intVec;
typedef std::shared_ptr<intVec> sPtr_intVec;

/*************************************************
 ********* DECLARATIONS OF FUNCTIONS *************
 *************************************************/

int main(int argc, char **argv)
{

    MPI::Init(argc, argv);
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();

    int data(0);
    int count(0);
    int message_counter(0);

    while (message_counter++ < MESSAGES_TO_SENT)
    {
        if (world_rank == 0)
        {
            // If we are the root process, send our data to everyone
            for (int i = 1; i < world_size; i++)
            {
                if (i != world_rank)
                {
                    MPI::COMM_WORLD.Isend(&data, count, MPI_INT, i, i);
                    std::cout << "Root: " << world_rank << " just sent a message to: ";
                    std::cout << i << ", with tag: " << i << "..." << std::endl;
                }
            }
        }
        else
        {
            for (int i = 1; i < world_size; i++)
            {
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