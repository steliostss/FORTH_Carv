EXECS = MPI_bcast CPI MPI_ping_pong MPI_user_def_dtype
MPICXX?=mpiCC
MPIC?=mpicc
src?=src/
exec?=exec/
FLAGS = -Wall -o2 -o 


all: ${EXECS}

MPI_bcast: ${src}MPI_bcast.cpp
	@echo "Compiling Broadcast..."
	${MPICXX} ${FLAGS} ${exec}MPI_bcast ${src}MPI_bcast.cpp

MPI_ping_pong: ${src}MPI_ping_pong.cpp
	@echo "\nCompiling Ping Pong..."
	${MPICXX} ${FLAGS} ${exec}MPI_ping_pong ${src}MPI_ping_pong.cpp

MPI_user_def_dtype: ${src}MPI_user_def_dtype.cpp
	@echo "\nCompiling MPI with User Defined type..."
	${MPICXX} ${FLAGS} ${exec}MPI_user_def_dtype ${src}MPI_user_def_dtype.cpp

CPI: ${src}cpi.c
	@echo "\nCompiling CPI (test)..."
	${MPIC} ${FLAGS} ${exec}CPI ${src}cpi.c

clean:
	@echo "Removing all exec files..."
	rm ${exec}*
	

