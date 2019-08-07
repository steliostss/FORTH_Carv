#include <iostream>
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"
#include "helper_functions.h"

#define BCAST_SUCCESS 10

/*************************************************
 ********* DECLARATIONS OF FUNCTIONS *************
 *************************************************/

int alt_Bcast(  int *sendbuf,           // start address of send buffer
                int count,              // number of elements
                MPI::Datatype sendtype, // datatype of elements
                int sender,             // sender 
                MPI_Comm comm           // communicator
);
//(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)

/*************************************************
 ******************** MAIN ***********************
 *************************************************/

int main(int argc, char **argv)
{

    MPI::Init(argc, argv);

    int sender(-1);
    int array_size(-1);
    int messages(0);

    sPtr_intVec recv = handle_command_line_args(argc, argv, &array_size, &sender, &messages);

    int *Ssend_array(new int[array_size]);
    fill_array(Ssend_array, array_size);

    int result = alt_Bcast(Ssend_array, array_size, MPI::INT, sender, MPI::COMM_WORLD);


    int world_rank = MPI::COMM_WORLD.Get_rank();
    if (result == 10 && world_rank != sender)
    {
        std::cout << world_rank << ": Successful" << std::endl;
    }
    MPI::Finalize();
    return 0;
}

/*************************************************
 ********* IMPLEMENTATIONS OF FUNCTIONS **********
 *************************************************/

int alt_Bcast(int *sendbuf,           // start address of send buffer
              int count,              // number of elements
              MPI::Datatype sendtype, // datatype of elements
              int sender,             // sender
              MPI_Comm comm           // communicator
)
{
    int world_rank = MPI::COMM_WORLD.Get_rank();
    int world_size = MPI::COMM_WORLD.Get_size();

    if (world_rank == sender) 
    {
        for (int i=0; i<world_size; ++i) 
        {
            if (i == world_rank) continue;
            MPI::COMM_WORLD.Send(sendbuf, count, sendtype, i, 0);
            std::cout << "Sender: " << sender << " just sent the array to: " << i << std::endl;
        }
    }
    else 
    {
        int *recv = new int[count];
        MPI_Request request = MPI::COMM_WORLD.Irecv(recv, count, sendtype, sender, 0);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        std::cout << "Process with world_rank: " << world_rank << " just received the array." << std::endl;

        delete recv;
    }
    
    return BCAST_SUCCESS;
}