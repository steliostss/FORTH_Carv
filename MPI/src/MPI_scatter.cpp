#include <iostream>
#include <memory>
#include "/usr/include/mpi/mpi.h"

typedef std::unique_ptr<int[]> uPtr_int;

uPtr_int create_partition(const void *sendbuf, int start, int array_size);

void alt_MPI_Scatter(int sender,                //to id the sender process
                     const void *sendbuf,       //address of send buffer
                     int sendcount,             //number of elements in buffer
                     MPI::Datatype sendtype,    //type of data in send buffer
                     void *recvbuf,             //address of receive buffer
                     MPI::Datatype recvtype,    //type of data in receive buffer
                     int root,                  //rank of sender process
                     MPI_Comm comm              //communicator
);

int main(int argc, char **argv) {
    MPI::Init(argc, argv);
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();

    uPtr_int my_array(new int[world_size]);

    if (world_rank == 0) {
        std::string sender;
        std::cout << "Give me the sender: ";
        std::cin >> sender;
        std::cout << "Sender is: " << sender << std::endl;
    } 
    else { 
        // MPI_Wait();
    }

    MPI::Finalize();
    return 0;
}

void alt_MPI_Scatter(int sender,             //to id the sender process
                     const void *sendbuf,    //address of send buffer
                     int sendcount,          //number of elements in buffer
                     MPI::Datatype sendtype, //type of data in send buffer
                     void *recvbuf,          //address of receive buffer
                     MPI::Datatype recvtype, //type of data in receive buffer
                     int root,               //rank of sender process
                     MPI_Comm comm           //communicator
) {
    int array_size = sizeof(sendbuf);
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();

    int scatter_pieces = array_size / world_size;

    //choose what to do depending on who is the sender
    if (world_rank == sender) {
        int i (0);
        int start (0);
        int end (array_size);

        while (i++ < world_size) {          //for all processes
            if (i != world_rank) {          //but not the sender
                std::cout << "Begining data distribution..." << std::endl;
                // Creating partition
                uPtr_int sub_array = create_partition(sendbuf, start, array_size);
                MPI::COMM_WORLD.Isend(sendbuf, scatter_pieces, MPI::INT, i, 0);
            }
        }
    }
    else {
        std::cout << "Receiving..." << std::endl;
        MPI::COMM_WORLD.Irecv(recvbuf, scatter_pieces, MPI::INT, sender, 0);
    }
}

uPtr_int create_partition(const void *sendbuf, int start, int array_size) {
    for (int i = start; i<array_size; ++i) {

    }
}