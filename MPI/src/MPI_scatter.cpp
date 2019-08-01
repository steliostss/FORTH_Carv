#include "/usr/include/mpi/mpi.h"
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

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

void fill_array(sPtr_int array, int array_size);
uPtr_intVec string_to_vec(std::string myStr);
uPtr_intVec handle_command_line_args(int argc, char **argv, int *array_size, int *sender);
int calculate_partition_size_for_proc(sPtr_intVec recv, int world_rank, int array_size, int sender);

uPtr_int create_partition(sPtr_int sendbuf, // address of send buffer
                          int elements,     // elements of partition
                          int offset,       // offset to create partition
                          int array_size    // send buffer size
);

void alt_MPI_Scatter(sPtr_int sendbuf,       // address of send buffer
                     int array_size,         // size of buffer
                     sPtr_intVec recv,       // recipients
                     MPI::Datatype sendtype, // type of data in send buffer
                     sPtr_int recvbuf,       // address of receive buffer
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

  sPtr_int Urecv_array(new int[array_size]);

  sPtr_int Ssend_array(new int[array_size]);
  fill_array(Ssend_array, array_size);

  // int world_rank = MPI::COMM_WORLD.Get_rank();
  // if (MPI::COMM_WORLD.Get_rank() != sender)
  //   std::cout << "________" << world_rank << "_________" << std::endl;

  alt_MPI_Scatter(Ssend_array,      // send buffer
                  array_size,       // size of buffer
                  recv,             // recipients
                  MPI_INT,          // MPI Send Datatype
                  Urecv_array,      // receive buffer
                  MPI_INT,          // MPI Receive Datatype
                  sender,           // sender id
                  MPI::COMM_WORLD); // MPI Communicator

  MPI::Finalize();
  return 0;
}

/*************************************************
 ********* IMPLEMENTATIONS OF FUNCTIONS **********
 *************************************************/

void alt_MPI_Scatter(sPtr_int sendbuf,       // address of send buffer
                     int array_size,         // size of buffer
                     sPtr_intVec recv,       // recipients
                     MPI::Datatype sendtype, // type of data in send buffer
                     sPtr_int recvbuf,       // address of receive buffer
                     MPI::Datatype recvtype, // type of data in receive buffer
                     int sender,             // rank of sender process
                     MPI_Comm comm           // communicator
)
{

  // std::cout << std::endl;

  // int world_size = MPI::COMM_WORLD.Get_size();
  int world_rank = MPI::COMM_WORLD.Get_rank();

  int recipients = recv->size();
  int counter(0);
  int offset_of_partition(0);

  if (world_rank == sender)
  { //send to all
    while (counter++ < recipients)
    {
      int elements = calculate_partition_size_for_proc(recv, world_rank, array_size, sender);
      uPtr_int partitioned_table = create_partition(sendbuf, elements, offset_of_partition, array_size);
      offset_of_partition += elements;
      // std::cout << "Partitioned table contains: ";
      // for (int i=0; i<elements; ++i)
      // {
      //   std::cout << partitioned_table[i] << " ";
      // }
      // std::cout << std::endl;
      // std::cout << std::endl;
      std::cout << "Counter: " << counter << ", Elements: " << elements << ", Original contains: ";
      for (int i=0; i<elements; ++i)
      {
        std::cout << sendbuf[offset_of_partition + i] << ' ';
      } 
      std::cout << std::endl;
      // MPI::COMM_WORLD.Isend(std::move(&partitioned_table), elements, MPI::INT, next_recip, 0);
      // std::cout << "Sender: " << world_rank << " just sent to: " << next_recip << std::endl;
    }
  }
  else if (std::find(recv->begin(), recv->end(), world_rank) != recv->end())
  { //receive from sender

    // int world_rank = MPI::COMM_WORLD.Get_rank();
    // uPtr_int partitioned_table = create_partition(sendbuf, counter, recv, next_recip, array_size, sender);

    // MPI_Request request = MPI::COMM_WORLD.Irecv(std::move(&recvbuf), elements, MPI::INT, sender, 0);
    // // MPI_Status stat;
    // MPI_Wait(&request, MPI_STATUS_IGNORE);
  }

  return;
}

// creates sub arrays from sendbuf
uPtr_int create_partition(sPtr_int sendbuf, int elements, int offset, int array_size )
{
  uPtr_int send_array(new int[elements]);
  for (auto i = 0; i<0; ++i)
  {
    send_array[i] = sendbuf[offset+i];
  }

  return std::move(send_array);
}

void fill_array(sPtr_int array, int array_size)
{
  for (int i = 0; i < array_size; ++i)
  {
    array[i] = i;
  }
}

uPtr_intVec string_to_vec(std::string myStr)
{
  uPtr_intVec myVec(new intVec);

  std::stringstream ss(myStr);

  for (int i; ss >> i;)
  {
    myVec->push_back(i);
    if (ss.peek() == ',')
      ss.ignore();
  }

  return std::move(myVec);
}

uPtr_intVec handle_command_line_args(int argc, char **argv, int *array_size, int *sender)
{
  int world_size = MPI::COMM_WORLD.Get_size();
  uPtr_intVec recv(new intVec);
  for (int i = 1; i < argc; i++)
  {
    if (i + 1 != argc)
    {
      if (strcmp(argv[i], "-sender") == 0)
      { // This is your parameter name
        *sender =
            std::stoi(argv[i + 1]); // The next value in the array is your value
        if (*sender >= world_size)
        {
          std::cerr << "Very large proc id for sender" << std::endl;
          MPI::COMM_WORLD.Abort(-1);
        }
        else
        {
          // std::cout << "Sender is: " << *sender << std::endl;
          i++; // Move to the next flag
        }
      }
      else if (strcmp(argv[i], "-size") == 0)
      {
        *array_size = std::stoi(argv[i + 1]);
        if (*array_size <= 0)
        {
          std::cerr << "Array size not valid" << std::endl;
          MPI::COMM_WORLD.Abort(-1);
        }
        i++;
      }
      else if (strcmp(argv[i], "-recv") == 0)
      {
        std::string recip = argv[i + 1];
        i++;
        for (int j = 0; j < world_size; ++j)
        {
          recv = string_to_vec(recip);
        }
      }
    }
  }

  // std::cout << "Sender is: " << *sender << std::endl;
  // std::cout << "Array size = " << *array_size << std::endl;

  if (*array_size < 0)
  { // Array size not defined in command line
    // std::cout << "Default array size value is 10." << std::endl;
    *array_size = 10;
  }
  if (*sender < 0)
  { // Sender not defined in command line
    // std::cout << "Default sender is 0." << std::endl;
    *sender = 0;
  }
  if (recv->empty())
  { // Recipients not defined in command line
    // std::cout << "Default recipients are all." << std::endl;
    for (int i = 0; i < world_size; ++i)
    {
      if (i != *sender)
        recv->push_back(i);
    }
  }

  // std::cout << "Recipients = ";
  // for (auto it = recv->begin(); it != recv->end(); ++it)
  // {
  //   std::cout << *it << " ";
  // }
  // std::cout << std::endl;
  // std::cout << std::endl;
  // std::cout << recv->size();
  // std::cout << std::endl;
  // std::cout << std::endl;

  return std::move(recv);
}

int calculate_partition_size_for_proc(sPtr_intVec recv, int world_rank, int array_size, int sender)
{

  int elements = array_size / recv->size(); // similar to floor(array_size /recv->size();)
  int position = 0;
  for (auto it = recv->begin(); it != recv->end(); ++it)
  {
    if (*it == world_rank)
      break;
    position++;
  }
  std::cout << "Position: " << position;

  int remainder = array_size - elements * recv->size();
  std::cout << std::endl << "Remainder: " << remainder << std::endl;
  if (position < remainder)
  {
    std::cout << "test";
    elements++;
  }
  return elements;
}