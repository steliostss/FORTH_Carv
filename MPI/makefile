EXECS=MPI_test CPI
MPICXX?=mpic++
MPIC?=mpicc

all: ${EXECS}

MPI_test: MPI_test.cpp
	${MPICXX} -o exec/MPI_test MPI_test.cpp

CPI: cpi.c
	${MPIC} -o exec/CPI cpi.c

clean:
	rm ${EXECS}
