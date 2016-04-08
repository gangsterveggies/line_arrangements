EXEC_NAME=dec
CC=g++

# CFLAGS= -Wall -Wno-write-strings -O0 -g
CFLAGS= -w -Wno-write-strings -O3 -Wall -g
CLIBS= -Iinclude -L. -lpng -lpngwriter -DNO_FREETYPE

SRC =            		\
	Debug.cpp		\
	GeoUtils.cpp		\
	DCEL.cpp		\
	main.cpp

OBJ =  ${SRC:.cpp=.o}

#------------------------------------------------------------

all: ${EXEC_NAME}

${EXEC_NAME}: ${OBJ}
	${CC} ${CFLAGS} -o ${EXEC_NAME} ${OBJ} ${CLIBS}

%.o: %.cpp
	${CC} ${CFLAGS} -c -o $@ $+ ${CLIBS}

clean:
	rm ${EXEC_NAME} *.o *~ *# -rf
