#include <iostream>
#include <memory>
#include "/usr/include/mpi/mpi.h"

void alt_MPI_Scatter(const void *sendbuf, 
                    int sendcount,              //address of send buffer 
                    MPI::Datatype sendtype,     //type of data in send buffer
                    void *recvbuf,              //address of receive buffer
                    MPI::Datatype recvtype,     //type of data in receive buffer
                    int root,                   //rank of sender process
                    MPI_Comm comm               //communicator
);

int main(int argc, char **argv) {
    MPI::Init(argc, argv);
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();

    std::unique_ptr<int[]> my_array (new int[world_size]);


    for (size_t i=0; i<(size_t)world_size; ++i) {
        my_array[i] = i;
        std::cout << my_array[i] << " ";
    }

    std::cout << std::endl;
    std::cout << world_size << ", " << world_rank << std::endl;

    MPI::Finalize();
    return 0;
}

void alt_MPI_Scatter(const void *sendbuf,
                     int sendcount, 
                     MPI::Datatype sendtype,
                     void *recvbuf,
                     MPI::Datatype recvtype,
                     int root, 
                     MPI_Comm comm
) {
    
}
