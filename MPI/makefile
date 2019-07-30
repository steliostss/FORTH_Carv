EXECS = test MPI_scatter MPI_bcast CPI MPI_ping_pong MPI_user_def_dtype
MPICXX?=mpiCC
MPIC?=mpicc
src?=src/
exec?=./
FLAGS = -Wall -o2 -o

all: ${EXECS}

.PHONY: clean all

test: ${src}test.cpp
	@echo "\nCompiling test..."
	${MPICXX} ${FLAGS} ${exec}test ${src}test.cpp

MPI_scatter: ${src}MPI_scatter.cpp
	@echo "\nCompiling Scatter..."
	${MPICXX} ${FLAGS} ${exec}MPI_scatter ${src}MPI_scatter.cpp
	@echo "--------------------------------------------------------------------------------------------------------------------------------------------------------------------"
	@echo ""
	@echo "Correct way to execute: mpiexec -n <number_of_procs> <path/to/MPI_scatter> -sender <sender_id> -size <array_size> -recv <list_of_recipients_comma_separated> "
	@echo ""
	@echo "--------------------------------------------------------------------------------------------------------------------------------------------------------------------"

MPI_bcast: ${src}MPI_bcast.cpp
	@echo "\nCompiling Broadcast..."
	${MPICXX} ${FLAGS} ${exec}MPI_bcast ${src}MPI_bcast.cpp

MPI_ping_pong: ${src}MPI_ping_pong.cpp
	@echo "\nCompiling Ping Pong..."
	${MPICXX} ${FLAGS} ${exec}MPI_ping_pong ${src}MPI_ping_pong.cpp

MPI_user_def_dtype: ${src}MPI_user_def_dtype.cpp
	@echo "\nCompiling MPI with User Defined type..."
	${MPICXX} ${FLAGS} ${exec}MPI_user_def_dtype ${src}MPI_user_def_dtype.cpp

CPI: ${src}cpi.c
	@echo "\nCompiling CPI (Calculating Pi) (test)..."
	${MPIC} ${FLAGS} ${exec}CPI ${src}cpi.c

clean:
	@echo "Removing all exec files..."
	rm ${EXECS}
