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

/*
 * This function handles the command line arguments.
 * We suppose that user can define: 
 * 1. sendbuf size	( -size n 				)
 * 2. sender		( -sender n 			)
 * 3. recipients	( -recv "n1,n2,n3, ..."	)
 * 4. #messages 	( -messages n 			)
 * 
 * A unique ptr to a vector of ints is returned which is
 * the list of recipients of the messages.
 */
uPtr_intVec handle_command_line_args(int argc, char **argv, int *array_size, int *sender, int *messages)
{
	int world_size = MPI::COMM_WORLD.Get_size();
	uPtr_intVec recv(new intVec);
	for (int i = 1; i < argc; i++)
	{
		if (i + 1 != argc)
		{
			if (strcmp(argv[i], "-sender") == 0)
			{									  // This is your parameter name
				*sender = std::stoi(argv[i + 1]); // The next value in the array is your value
				if (*sender >= world_size || *sender < 0)
				{
					std::cerr << "Invalid proc id for sender" << std::endl;
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
			else if (strcmp(argv[i], "-messages") == 0)
			{										// This is your parameter name
				*messages = std::stoi(argv[i + 1]); // The next value in the array is your value
				if (*messages <= 0)
				{
					std::cerr << "Very small number for messages" << std::endl;
					MPI::COMM_WORLD.Abort(-1);
				}
				else
				{
					// std::cout << "#messages is: " << *messages << std::endl;
					i++; // Move to the next flag
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

/*
 * This function fills a given array with numbers 
 * according to this: a[n]=n 
 */
void fill_array(int *array, int array_size)
{
	for (int i = 0; i < array_size; ++i)
	{
		array[i] = i;
	}
}

/*
 * This function creates a vector of ints
 * generated from a given string 
 */

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

/*
 * Given a vector of ints and an integer k, this function
 * generates the same vector with the only change being 
 * that k is in front
 */

uPtr_intVec reorder_vec(sPtr_intVec old_vec, int *sender)
{
	uPtr_intVec new_vec(new intVec);

	new_vec->push_back(sender); //move sender to first position

	for (intVecIter it = old_vec->begin(); it != old_vec->end(); ++it)
	{	// append everything else
		if (*it != *sender)
			new_vec->push_back(*it);
	}

	return std::move(new_vec);
}