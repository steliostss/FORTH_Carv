// #include "/usr/include/mpi/mpi.h"
// #include <stdio.h>

// int main(int argc, char *argv[])
// {
//     int myid, numprocs, left, right;
//     int buffer[10], buffer2[10];
//     MPI_Request request;
//     MPI_Status status;

//     MPI_Init(&argc, &argv);
//     MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
//     MPI_Comm_rank(MPI_COMM_WORLD, &myid);

//     right = (myid + 1) % numprocs;
//     left = myid - 1;
//     if (left < 0)
//         left = numprocs - 1;

//     MPI_Irecv(buffer, 10, MPI_INT, left, 123, MPI_COMM_WORLD, &request);    
//     MPI_Send(buffer2, 10, MPI_INT, right, 123, MPI_COMM_WORLD);
//     MPI_Wait(&request, &status);
//     MPI_Finalize();
//     return 0;
// }

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

int main(int argc, char** argv) {
    std::vector<int> vect;

    std::string str = argv[1];
    std::stringstream ss(str);

    for (int i; ss >> i;)
    {
        vect.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }

    for (std::size_t i = 0; i < vect.size(); i++)
        std::cout << vect[i] << std::endl;
}