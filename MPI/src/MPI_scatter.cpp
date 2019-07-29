#include <iostream>
#include <memory>
#include "/usr/include/mpi/mpi.h"

typedef std::unique_ptr<int[]> uPtr_int;
typedef std::shared_ptr<int[]> sPtr_int;

uPtr_int create_partition(uPtr_int sendbuf, int start, int scatter_pieces, int array_size);
void fill_array(sPtr_int array, int array_size);

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

    std::cout << "My rank is: " << world_rank << std::endl;
    int array_size = std::stoi(argv[2]);
    uPtr_int Usend_array(new int[array_size]);
    uPtr_int Urecv_array(new int[array_size]);

    int sender = std::stoi(argv[1]);
    // std::cout << "Sender is: " << sender << std::endl;

    sPtr_int Ssend_array = std::move(Usend_array);
    fill_array(Ssend_array, array_size);
    // int length_of_array = (int) sizeof(Ssend_array)/sizeof(int);
    // for (auto i = 0; i < length_of_array; ++i) {
    //     std::cout << "hey";
    //     std::cout << Ssend_array[i];
    // }
    // std::cout << std::endl;

    int scatter_pieces = array_size / world_size; //each process will receive "scatter_pieceis" elements

    alt_MPI_Scatter(std::move(Usend_array),     //send buffer
                    scatter_pieces,                          //number of elements
                    MPI_INT,                    //MPI Send Datatype
                    std::move(Urecv_array),     //receive buffer
                    MPI_INT,                    //MPI Receive Datatype
                    sender,                     //sender id
                    MPI::COMM_WORLD);           //MPI Communicator

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
    int array_size = sizeof(sendbuf)/sizeof(int);
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();

    //choose what to do depending on who is the sender
    if (world_rank == sender) {
        int i (0);
        int start (0);
        while (i < world_size) {          //for all processes
            std::cout << "i is: "<< i << std::endl;
            if (i != world_rank) {          //but not the sender
                std::cout << "Begining data distribution..." << std::endl;
                // Creating partition
                uPtr_int sub_array = create_partition(std::move(sendbuf), start, sendcount, array_size);
                start += sendcount;
                MPI::COMM_WORLD.Isend(std::move(&sub_array), sendcount, MPI::INT, i, 0);
                std::cout << "Sender: " << world_rank << " just sent to: " << i << std::endl;
            }
            ++i;
        }
    }
    else {
        MPI_Request request = MPI::COMM_WORLD.Irecv(std::move(&recvbuf), sendcount, MPI::INT, sender, 0);
        MPI_Status stat;
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        if (request != 0) {
            std::cout << "Status for proc: " << world_rank << " is: " << request << std::endl;
            std::cout << "Receiving..." << std::endl;
            std::cout << "Proc: " << world_rank << " just received: ";
            for (int i = 0; i < sendcount; ++i) {
                std::cout << recvbuf[i] << " ";
            }
            std::cout << std::endl;
        }
    }
    return;
}

uPtr_int create_partition(uPtr_int sendbuf, int start, int scatter_pieces, int array_size) {
    uPtr_int partitioned_table(new int[scatter_pieces]);

    for (int i = start; i < scatter_pieces + start; ++i) {
        partitioned_table[i-start] = sendbuf[i];
    }
    std::cout << "for start: " << start << std::endl;
    std::cout << "array is: ";
    for (int i = 0; i<scatter_pieces; ++i) {
        std::cout << partitioned_table[i];
    }

    std::cout << std::endl;

    return std::move(partitioned_table);
}

void fill_array(sPtr_int array, int array_size) {
    for (int i = 0; i < array_size; ++i) {
        array[i] = i;
        std::cout << i;
    }
    std::cout << std::endl;
    
}
