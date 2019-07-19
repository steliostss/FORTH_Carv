EXECS = MPI_bcast CPI MPI_ping_pong MPI_user_def_dtype
MPICXX?=mpiCC
MPIC?=mpicc
src?=src/
exec?=exec/
FLAGS = -Wall -c -o2


all: ${EXECS}

MPI_bcast: ${src}MPI_bcast.cpp
	@echo "Compiling Broadcast..."
	@${MPICXX} ${FLAGS} -o ${exec}MPI_bcast ${src}MPI_bcast.cpp

MPI_ping_pong: ${src}MPI_ping_pong.cpp
	@echo "Compiling Ping Pong..."
	@${MPICXX} ${FLAGS} -o ${exec}MPI_ping_pong ${src}MPI_ping_pong.cpp

MPI_user_def_dtype: ${src}MPI_user_def_dtype.cpp
	@echo "Compiling MPI with User Defined type..."
	@${MPICXX} ${FLAGS} -o ${exec}MPI_user_def_dtype ${src}MPI_user_def_dtype.cpp

CPI: ${src}cpi.c
	@echo "Compiling CPI (test)..."
	@${MPIC} ${FLAGS} -o ${exec}CPI ${src}cpi.c

clean:
	@echo "Removing all exec files..."
	@rm ${exec}*
	

