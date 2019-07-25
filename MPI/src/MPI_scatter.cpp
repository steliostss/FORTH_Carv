#include <iostream>
#include <memory>
#include "/usr/include/mpi/mpi.h"

typedef std::unique_ptr<int[]> uPtr_int;
typedef std::shared_ptr<int[]> sPtr_int;

uPtr_int create_partition(uPtr_int sendbuf, int start, int scatter_pieces, int array_size);
void fill_array(sPtr_int array);

void alt_MPI_Scatter(uPtr_int sendbuf,          //address of send buffer
                     int sendcount,             //number of elements in buffer
                     MPI::Datatype sendtype,    //type of data in send buffer
                     uPtr_int recvbuf,          //address of receive buffer
                     MPI::Datatype recvtype,    //type of data in receive buffer
                     int sender,                //rank of sender process
                     MPI_Comm comm              //communicator
);

int main(int argc, char **argv) {
    MPI::Init(argc, argv);
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();

    uPtr_int Usend_array(new int[world_size]);
    uPtr_int Urecv_array(new int[world_size]);

    int sender = std::stoi(argv[1]);
    std::cout << "Sender is: " << sender << std::endl;

    sPtr_int Ssend_array = std::move(Usend_array);
    fill_array(Ssend_array);

    // alt_MPI_Scatter(std::move(Usend_array),     //send buffer
    //                 5,                          //number of elements
    //                 MPI_INT,                    //MPI Send Datatype
    //                 std::move(Urecv_array),     //receive buffer
    //                 MPI_INT,                    //MPI Receive Datatype
    //                 sender,                     //sender id
    //                 MPI::COMM_WORLD);           //MPI Communicator

    MPI::Finalize();
    return 0;
}

void alt_MPI_Scatter(uPtr_int sendbuf,          //address of send buffer
                     int sendcount,             //number of elements in buffer
                     MPI::Datatype sendtype,    //type of data in send buffer
                     uPtr_int recvbuf,          //address of receive buffer
                     MPI::Datatype recvtype,    //type of data in receive buffer
                     int sender,                //rank of sender process
                     MPI_Comm comm              //communicator
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

        while (i < world_size) {          //for all processes
            std::cout << "i is: "<< i << std::endl;
            if (i != world_rank) {          //but not the sender
                std::cout << "Begining data distribution..." << std::endl;
                // Creating partition
                uPtr_int sub_array = create_partition(std::move(sendbuf), start, scatter_pieces, array_size);
                start += scatter_pieces;
                MPI::COMM_WORLD.Isend(std::move(&sub_array), scatter_pieces, MPI::INT, i, 0);
                std::cout << "Sender: " << world_rank << " just sent to: " << i << std::endl;
            }
            ++i;
        }
    }
    else {
        MPI::Request request = MPI::COMM_WORLD.Irecv(std::move(&recvbuf), scatter_pieces, MPI::INT, sender, 0);
        if (request.Get_status() != 0) {
            std::cout << "Status for proc: " << world_rank << " is: " << request.Get_status() << std::endl;
            std::cout << "Receiving..." << std::endl;
            std::cout << "Proc: " << world_rank << " just received" << std::endl;
        }
    }
    return;
}

uPtr_int create_partition(uPtr_int sendbuf, int start, int scatter_pieces, int array_size)
{
    for (int i=start; i<array_size; ++i) {
        return NULL;
    }
}

void fill_array(sPtr_int array) {
    int size = sizeof(array)/sizeof(int);
    std::cout << "size: " << size << std::endl;
    for (int i=0; i<size; ++i) {
        array[i] = i;
        std::cout << i;
    }
    std::cout << std::endl;
}
