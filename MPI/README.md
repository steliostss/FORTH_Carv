# MPI Implementation 

## Required Software

MPICH is needed to test/run this project

install mpich on ubuntu:

``` sudo apt-get install mpich ```

## MPI Folder Outline

All files are custom implementation of MPICH functions.

### MPI Broadcast

File: [MPI_bcast.cpp](./src/MPI_bcast.cpp) intends to broadcast an int number to all available processes.

### MPI Ping-Pong

File: [MPI_ping_pong.cpp](./src/MPI_ping_pong.cpp) intends to establish a simple ping pong communication between root and every other process.

### MPI Scatter

File: [MPI_scatter.cpp](./src/MPI_scatter.cpp) intends to scatter an int array to all available processes.

### MPI User Defined Datatype

File: [MPI_user_def_dtype.cpp](./src/MPI_user_def_dtype.cpp) intends to broadcast a user defined datatype to all processes available

### MPI CPI

File: [cpi.c](./src/cpi.c) calculates Pi constant.

