EXECS = exec/test exec/MPI_scatter exec/MPI_bcast exec/cpi exec/MPI_ping_pong exec/MPI_user_def_dtype
MPICXX?=mpiCC
MPIC?=mpicc
src?=src/
exec?=exec/
obj?=obj/
CFLAGS = -Wall -O2 -c
OFLAGS = -o

all: ${EXECS}

.PHONY: clean all

##################### TEST ##################### 
exec/test: ${obj}test.o
	@echo "\nLinking test..."
	${MPICXX} ${obj}test.o ${OFLAGS} ${exec}test

obj/test.o: ${src}test.cpp
	@echo "\nCompiling test..."
	${MPICXX} ${CFLAGS} ${src}test.cpp ${OFLAGS} ${obj}test.o

############## HELPER_FUNCTIONS ############### 
obj/helper_functions.o: ${src}helper_functions.cpp
	@echo "\nCompiling command line arguments..."
	${MPICXX} ${CFLAGS} ${src}helper_functions.cpp ${OFLAGS} ${obj}helper_functions.o

################# MPI_SCATTER ################# 
exec/MPI_scatter: obj/MPI_scatter.o obj/helper_functions.o
	@echo "\nLinking Scatter..."
	${MPICXX} ${obj}MPI_scatter.o ${obj}helper_functions.o ${OFLAGS} ${exec}MPI_scatter

obj/MPI_scatter.o: ${src}MPI_scatter.cpp
	@echo "\nCompiling Scatter..."
	${MPICXX} ${CFLAGS} ${src}MPI_scatter.cpp ${OFLAGS} ${obj}MPI_scatter.o

################## MPI_BCAST ################## 
exec/MPI_bcast: ${obj}MPI_bcast.o obj/helper_functions.o
	@echo "\nLinking Broadcast..."
	${MPICXX} ${obj}MPI_bcast.o ${obj}helper_functions.o ${OFLAGS} ${exec}MPI_bcast

obj/MPI_bcast.o: ${src}MPI_bcast.cpp
	@echo "\nCompiling Broadcast..."
	${MPICXX} ${CFLAGS} ${src}MPI_bcast.cpp ${OFLAGS} ${obj}MPI_bcast.o

################ MPI_PING_PONG ################ 
exec/MPI_ping_pong: ${obj}MPI_ping_pong.o
	@echo "\nLinking Ping Pong..."
	${MPICXX} ${obj}MPI_ping_pong.o ${OFLAGS} ${exec}MPI_ping_pong

obj/MPI_ping_pong.o: ${src}MPI_ping_pong.cpp
	@echo "\nCompiling Ping Pong..."
	${MPICXX} ${CFLAGS} ${src}MPI_ping_pong.cpp ${OFLAGS} ${obj}MPI_ping_pong.o

############# MPI_USER_DEF_DTYPE ############## 
exec/MPI_user_def_dtype: ${obj}MPI_user_def_dtype.o
	@echo "\nLinking User Defined Datatype..."
	${MPICXX} ${obj}MPI_user_def_dtype.o ${OFLAGS} ${exec}MPI_user_def_dtype

obj/MPI_user_def_dtype.o: ${src}MPI_user_def_dtype.cpp
	@echo "\nCompiling User Defined Datatype..."
	${MPICXX} ${CFLAGS} ${src}MPI_user_def_dtype.cpp ${OFLAGS} ${obj}MPI_user_def_dtype.o

##################### CPI ##################### 
exec/cpi: ${obj}cpi.o
	@echo "\nLinking CPI: Calculating Pi constant..."
	${MPIC} ${obj}cpi.o ${OFLAGS} ${exec}cpi

obj/cpi.o: ${src}cpi.c
	@echo "\nCompiling CPI: Calculating Pi constant..."
	${MPIC} ${CFLAGS} ${src}cpi.c ${OFLAGS} ${obj}cpi.o

#################### CLEAN #################### 
clean:
	@echo "Removing all exec and obj files..."
	rm ${exec}*
	rm ${obj}*
