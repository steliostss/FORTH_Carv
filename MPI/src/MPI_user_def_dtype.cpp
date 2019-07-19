#include <iostream>
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"

class Message {
private:
    int mValue;
    std::string mMessage;

public:
    //getters
    int get_mValue() { return mValue; }
    std::string get_mMessage() { return mMessage; }

    //setters
    void set_mValue(int value) { mValue = value; }
    void set_mValue(std::string message) { mMessage = message; }

    //constructor
    Message() : mValue(0),
                mMessage("Hello")
    {}
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

    Message *send_message = new Message;
    Message *recv_message = new Message;

    delete recv_message;
    delete send_message;
}