#include <iostream>
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"
#include "helper_functions.h"
#include <bits/stdc++.h>

#define BCAST_SUCCESS 10

/*************************************************
 ********* DECLARATIONS OF FUNCTIONS *************
 *************************************************/

int optimized_bcast();
int simple_bcast(int *sendbuf, int count, MPI::Datatype sendtype, int *sender);
void define_recipients(int position, sPtr_intVec recipients);
int alt_Bcast(int *sendbuf,           // start address of send buffer
              int count,              // number of elements
              MPI::Datatype sendtype, // datatype of elements
              int *sender,            // sender
              MPI_Comm comm,          // communicator
              int method              // send method
);

/*************************************************
 ******************** MAIN ***********************
 *************************************************/

int main(int argc, char **argv)
{

    MPI::Init(argc, argv);

    int sender(-1);
    int array_size(-1);
    int messages(-1);
    int method(-1);

    int world_rank = MPI::COMM_WORLD.Get_rank();

    sPtr_intVec recv = handle_command_line_args(argc, argv, &array_size, &sender, &messages, &method);

    uPtr_intVec recipients = reorder_vec(recv, &sender);
    auto iterator = std::find(recipients->begin(), recipients->end(), world_rank);
    int position = std::distance(recipients->begin(), iterator);

    // std::cout << "Proc with id: " << world_rank << " has position: " << position << std::endl;

    define_recipients(position, std::move(recipients));

    /*
     * int *Ssend_array(new int[array_size]);
     * fill_array(Ssend_array, array_size);
     * 
     * // int result = alt_Bcast(Ssend_array, array_size, MPI::INT, &sender, MPI::COMM_WORLD, method);
     * 
     * int world_rank = MPI::COMM_WORLD.Get_rank();
     * if (result == 10 && world_rank != sender)
     * {
     *     std::cout << world_rank << ": Successful" << std::endl;
     * }
     * 
     */
    
    MPI::Finalize();
    return 0;
}

/*************************************************
 ********* IMPLEMENTATIONS OF FUNCTIONS **********
 *************************************************/

int alt_Bcast(int *sendbuf,           // start address of send buffer
              int count,              // number of elements
              MPI::Datatype sendtype, // datatype of elements
              int *sender,            // sender
              MPI_Comm comm,          // communicator
              int method              // send method
)
{
    int result(-1);

    if (method == 1)
        result = simple_bcast(sendbuf, count, sendtype, sender);
    else if (method == 2)
        result = optimized_bcast();

    return result;
}

/*
 * This function implements the collective communication of MPI: broadcast
 * with simple sends from the sender to everyone else
 */
int simple_bcast(int *sendbuf, int count, MPI::Datatype sendtype, int *sender)
{
    int world_rank = MPI::COMM_WORLD.Get_rank();
    int world_size = MPI::COMM_WORLD.Get_size();

    if (world_rank == *sender)
    {
        for (int i = 0; i < world_size; ++i)
        {
            if (i == world_rank)
                continue;
            MPI::COMM_WORLD.Send(sendbuf, count, sendtype, i, 0);
            std::cout << "Sender: " << *sender << " just sent the array to: " << i << std::endl;
        }
    }
    else
    {
        int *recv = new int[count];
        MPI_Request request = MPI::COMM_WORLD.Irecv(recv, count, sendtype, *sender, 0);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        std::cout << "Process with world_rank: " << world_rank << " just received the array." << std::endl;

        delete recv;
    }
    return BCAST_SUCCESS;
}

/*
 * This function implements the collective communication of MPI: broadcast
 * The collective comminication is optimized using all procs to broadcast the message
 */
int optimized_bcast()
{
    sPtr_intVec recipients(new intVec);

    return BCAST_SUCCESS;
}

void define_recipients(int position, sPtr_intVec recipients)
{
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();

    

}
