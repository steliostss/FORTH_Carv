#include "/usr/include/mpi/mpi.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include "cmd_line_args.h"

/*************************************************
 ****************** TYPEDEF **********************
 *************************************************/

typedef std::unique_ptr<int[]> uPtr_int;
typedef std::shared_ptr<int[]> sPtr_int;
typedef std::vector<int> intVec;
typedef std::vector<int>::iterator intVecIter;
typedef std::unique_ptr<intVec> uPtr_intVec;
typedef std::shared_ptr<intVec> sPtr_intVec;
typedef std::tuple<int, int> int_Tup;

/*************************************************
 ********* DECLARATIONS OF FUNCTIONS *************
 *************************************************/

int calculate_partition_size_for_proc(sPtr_intVec recv, int world_rank, int array_size, int sender);
void alt_MPI_Scatter(int* sendbuf,       // address of send buffer
                     int array_size,         // size of buffer
                     sPtr_intVec recv,       // recipients
                     MPI::Datatype sendtype, // type of data in send buffer
                     MPI::Datatype recvtype, // type of data in receive buffer
                     int sender,             // rank of sender process
                     MPI_Comm comm           // communicator
);

/*************************************************
 ******************** MAIN ***********************
 *************************************************/

int main(int argc, char **argv)
{
  MPI::Init(argc, argv);
  // int world_rank = MPI::COMM_WORLD.Get_rank();

  int array_size = -1;
  int sender = -1;
  sPtr_intVec recv = handle_command_line_args(argc, argv, &array_size, &sender);

  int* Ssend_array(new int[array_size]);
  fill_array(Ssend_array, array_size);

  alt_MPI_Scatter(Ssend_array,      // send buffer
                  array_size,       // size of buffer
                  recv,             // recipients
                  MPI_INT,          // MPI Send Datatype
                  MPI_INT,          // MPI Receive Datatype
                  sender,           // sender id
                  MPI::COMM_WORLD); // MPI Communicator

  MPI::Finalize();
  return 0;
}

/*************************************************
 ********* IMPLEMENTATIONS OF FUNCTIONS **********
 *************************************************/

void alt_MPI_Scatter(int* sendbuf,       // address of send buffer
                     int array_size,         // size of buffer
                     sPtr_intVec recv,       // recipients
                     MPI::Datatype sendtype, // type of data in send buffer
                     MPI::Datatype recvtype, // type of data in receive buffer
                     int sender,             // rank of sender process
                     MPI_Comm comm           // communicator
)
{

  // std::cout << std::endl;

  // int world_size = MPI::COMM_WORLD.Get_size();
  int world_rank = MPI::COMM_WORLD.Get_rank();

  int recipients = recv->size();
  intVecIter next_recip = recv->begin();
  int counter(0);
  int offset_of_partition(0);

  if (world_rank == sender)
  { // send to all
    while (counter++ < recipients)
    { // for all recipients

      int elements = calculate_partition_size_for_proc(recv, *next_recip, array_size, sender);
      int *partitioned_table = new int[elements];
      // std::cout << "Sending: ";
      for (int i = 0; i < elements; ++i)
      { // create partitioned table from original
        partitioned_table[i] = sendbuf[offset_of_partition + i];
        // std::cout << partitioned_table[i] << " ";
      }
      offset_of_partition += elements;

      MPI_Request request = MPI::COMM_WORLD.Isend(partitioned_table, elements, sendtype, *next_recip, 0);
      MPI_Wait(&request, MPI_STATUS_IGNORE);

      // std::cout << "Sender: " << world_rank << " just sent to: " << *next_recip << std::endl;
      if (next_recip != recv->end())
        next_recip++;
    }
  }

  else if (std::find(recv->begin(), recv->end(), world_rank) != recv->end())
  { //receive from sender

    int world_rank = MPI::COMM_WORLD.Get_rank();
    int elements = calculate_partition_size_for_proc(recv, world_rank, array_size, sender);
    int *recvbuf = new int[elements];
    MPI_Request request = MPI::COMM_WORLD.Irecv(recvbuf, elements, recvtype, sender, 0);
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    std::cout << "Proc: " << world_rank << " just recieved: ";
    for (int i=0; i<elements; ++i) 
      std::cout << recvbuf[i] << ' ';
    std::cout << std::endl;
  }

  return;
}

int calculate_partition_size_for_proc(sPtr_intVec recv, int world_rank, int array_size, int sender)
{

  int elements = array_size / recv->size(); // similar to floor(array_size /recv->size();)
  int position = 0;
  for (auto it = recv->begin(); it != recv->end(); ++it)
  { // define position of proc with id "world_rank" in the recv list
    if (*it == world_rank)
      break;
    position++;
  }

  int remainder = array_size - elements * recv->size();
  if (position < remainder) // evenly distribute the remainder
    elements++;

  return elements;
}