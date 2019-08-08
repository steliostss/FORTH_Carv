#include <iostream>
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"
#include <memory>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

/*************************************************
 ****************** TYPEDEF **********************
 *************************************************/
#define SIMPLE_SEND 1
#define OPTIMIZED_SEND 2

// using uPtr_int = std::unique_ptr<int[]>;

using uPtr_int 		= std::unique_ptr<int[]>;
using sPtr_int 		= std::shared_ptr<int[]>;
using intVec 		= std::vector<int>;
using intVecIter 	= std::vector<int>::iterator;
using uPtr_intVec 	= std::unique_ptr<intVec>;
using sPtr_intVec 	= std::shared_ptr<intVec>;

/*************************************************
 ********* DECLARATIONS OF FUNCTIONS *************
 *************************************************/

void fill_array(int *array, int array_size);
uPtr_intVec string_to_vec(std::string myStr);
uPtr_intVec handle_command_line_args(int argc, char **argv, int *array_size, int *sender, int *messages, int* method);
uPtr_intVec reorder_vec(sPtr_intVec old_vec, int *sender);
int highestPowerof2(int n);