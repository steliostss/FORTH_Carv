#include <iostream>
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"
#include <memory>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>

/*************************************************
 ****************** TYPEDEF **********************
 *************************************************/
#define SIMPLE_SEND 1
#define OPTIMIZED_SEND 2

typedef std::unique_ptr<int[]> uPtr_int;
typedef std::shared_ptr<int[]> sPtr_int;
typedef std::vector<int> intVec;
typedef std::vector<int>::iterator intVecIter;
typedef std::unique_ptr<intVec> uPtr_intVec;
typedef std::shared_ptr<intVec> sPtr_intVec;

/*************************************************
 ********* DECLARATIONS OF FUNCTIONS *************
 *************************************************/

void fill_array(int *array, int array_size);
uPtr_intVec string_to_vec(std::string myStr);
uPtr_intVec handle_command_line_args(int argc, char **argv, int *array_size, int *sender, int *messages, int* method);
uPtr_intVec reorder_vec(sPtr_intVec old_vec, int *sender);
