#include <iostream>
#include <unistd.h>
#include "/usr/include/mpi/mpi.h"
#include <memory>
#include <sstream>
#include <ostream>
#include <string>
#include <vector>
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
					// std::cout << recip << std::endl;
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

void fill_array(int *array, int array_size)
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