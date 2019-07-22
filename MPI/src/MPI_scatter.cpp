#include <iostream>
#include "/usr/include/mpi/mpi.h"

int main(int argc, char **argv)
{

    MPI::Init(argc, argv);
    int world_size = MPI::COMM_WORLD.Get_size();
    int world_rank = MPI::COMM_WORLD.Get_rank();



    MPI::Finalize();
    return 0;
}