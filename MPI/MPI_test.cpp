#include <iostream> 
#include <unistd.h>
#include "mpi/mpi.h"
// #include <

#define PING_PONG_LIMIT 20

class Message {
    private: 
        int mValue;
        std::string mMessage;
    
    public:
        //getters
        int get_mValue () { return mValue; }
        std::string get_mMessage () { return mMessage; }

        //setters
        void set_mValue (int value) { mValue = value; }
        void set_mValue(std::string message) { mMessage = message; }

        //constructor
        Message (): mValue(0), 
                    mMessage("Hello")
                    { }
};

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
    if (world_rank == 0)
    {
        // If we are rank 0, set the number to -1 and send it to process 1
        number = -1;
        MPI_Send(
            /* data         = */ &number,
            /* count        = */ 1,
            /* datatype     = */ MPI_INT,
            /* destination  = */ 1,
            /* tag          = */ 0,
            /* communicator = */ MPI_COMM_WORLD);
    }
    else if (world_rank == 1)
    {
        MPI_Recv(
            /* data         = */ &number,
            /* count        = */ 1,
            /* datatype     = */ MPI_INT,
            /* source       = */ 0,
            /* tag          = */ 0,
            /* communicator = */ MPI_COMM_WORLD,
            /* status       = */ MPI_STATUS_IGNORE);
        printf("Process 1 received number %d from process 0\n", number);
    }

    Message* send_message = new Message;

    //SEND Section
    for (int i=0; i<world_size-1; ++i) {
        MPI_Send(&i, 1, MPI_INT, i, i+100, MPI_COMM_WORLD);
        std::cout << "Integer sent" << std::endl;
        // MPI_Send(&send_message, 2, MPI_INT, i, i+200, MPI_COMM_WORLD);
        // std::cout << "Class sent" << std::endl;
    }

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

    MPI_Get_processor_name(processor_name, &name_len);

    Message* recv_message = new Message; 
    int recv_int;
    //RECV Section
    for (int i=0; i<world_size-1; ++i) {
        MPI_Recv( &recv_int, 1, MPI_INT, i, i+100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        // MPI_Recv( &recv_message, 2 , _____ , i+200, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    //PING PONG 
    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 2;
    //this example runs with two processes only
    if (world_rank == 2) {
        while (ping_pong_count < PING_PONG_LIMIT) {
            if (world_rank == ping_pong_count % 2)
            {
                // Increment the ping pong count before you send it
                ping_pong_count++;
                MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                        MPI_COMM_WORLD);
                printf("%d sent and incremented ping_pong_count "
                    "%d to %d\n",
                    world_rank, ping_pong_count,
                    partner_rank);
            }
            else
            {
                MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0,
                        MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("%d received ping_pong_count %d from %d\n",
                    world_rank, ping_pong_count, partner_rank);
            }
        }
    }

    double end = MPI_Wtime();
   
    usleep(60);
    // Print off a hello world message
    std::cout << "Hello world from processor " << processor_name;
    std::cout << ", rank " << world_rank;
    std::cout << " out of " << world_size << std::endl;
    std::cout << "The process took " << end - start << " seconds to run." << std::endl;
    fflush(stdout);

    delete recv_message;
    delete send_message;

    // Finalize the MPI environment.
    MPI_Finalize();
    return 0;
}