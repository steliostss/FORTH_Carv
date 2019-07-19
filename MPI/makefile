EXECS?=MPI_bcast CPI MPI_ping_pong MPI_user_def_dtype
MPICXX?=mpiCC
MPIC?=mpicc
src?=src/
exec?=exec/

all: ${EXECS}

MPI_bcast: ${src}MPI_bcast.cpp
	${MPICXX} -o ${exec}MPI_bcast ${src}MPI_bcast.cpp

MPI_ping_pong: ${src}MPI_ping_pong.cpp
	${MPICXX} -o ${exec}MPI_ping_pong ${src}MPI_ping_pong.cpp

MPI_user_def_dtype: ${src}MPI_user_def_dtype.cpp
	${MPICXX} -o ${exec}MPI_user_def_dtype ${src}MPI_user_def_dtype.cpp

CPI: ${src}cpi.c
	${MPIC} -o ${exec}CPI ${src}cpi.c

clean:
	rm ${exec}*
	

