CC = g++
C_FLAGS = -Wall -Wextra -O3 -g3
L_FLAGS = -lSDL2 -lm

BUILD_DIR = build
INCLUDE_DIR = include
SRC_DIR = src

LINALG_PATH = /home/ishaan-user/Documents/C++/nimbus-linalg

game: ${SRC_DIR}/main.cpp ${BUILD_DIR}/tri.o ${BUILD_DIR}/global.o ${BUILD_DIR}/framerate.o ${LINALG_PATH}/linalg.a
	${CC} -o game ${SRC_DIR}/main.cpp ${BUILD_DIR}/tri.o ${BUILD_DIR}/global.o ${BUILD_DIR}/framerate.o ${LINALG_PATH}/linalg.a ${C_FLAGS} ${L_FLAGS}
	
${BUILD_DIR}/tri.o: ${SRC_DIR}/tri.cpp ${INCLUDE_DIR}/tri.h
	${CC} -o ${BUILD_DIR}/tri.o ${SRC_DIR}/tri.cpp ${C_FLAGS} -c

${BUILD_DIR}/global.o: ${SRC_DIR}/global.cpp ${INCLUDE_DIR}/global.h
	${CC} -o ${BUILD_DIR}/global.o ${SRC_DIR}/global.cpp ${C_FLAGS} -c

${BUILD_DIR}/framerate.o: ${SRC_DIR}/framerate.cpp  ${INCLUDE_DIR}/framerate.h
	${CC} -o ${BUILD_DIR}/framerate.o ${SRC_DIR}/framerate.cpp ${C_FLAGS} -c

${LINALG_PATH}/linalg.a:
	make linalg.a
