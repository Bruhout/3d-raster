CC = g++
C_FLAGS = -O3 -g3
L_FLAGS = -lm

BUILD = build
INCLUDE = include
SRC = src

LINALG_PATH = /home/ishaang/Documents/C++/sdl/sdl-video-out/nimbus-linalg

game: ${BUILD}/main.o ${BUILD}/tri.o ${BUILD}/global.o ${BUILD}/framerate.o ${LINALG_PATH}/linalg.a
	${CC} -o game ${BUILD}/main.o ${BUILD}/tri.o ${BUILD}/global.o ${BUILD}/framerate.o ${LINALG_PATH}/linalg.a ${L_FLAGS}

${BUILD}/main.o: ${SRC}/main.cpp
	${CC} -o ${BUILD}/main.o ${SRC}/main.cpp ${C_FLAGS} -c

${BUILD}/tri.o: ${SRC}/tri.cpp ${INCLUDE}/tri.h
	${CC} -o ${BUILD}/tri.o ${SRC}/tri.cpp ${C_FLAGS} -c

${BUILD}/global.o: ${SRC}/global.cpp ${INCLUDE}/global.h
	${CC} -o ${BUILD}/global.o ${SRC}/global.cpp ${C_FLAGS} -c

${BUILD}/framerate.o: ${SRC}/framerate.cpp  ${INCLUDE}/framerate.h
	${CC} -o ${BUILD}/framerate.o ${SRC}/framerate.cpp ${C_FLAGS} -c

${LINALG_PATH}/linalg.a:
	make ${LINALG_PATH}/linalg.a


clean:
	rm ${BUILD}/*
	rm output.mp4

run:
	./game 256 256 4 500 | ffmpeg -f rawvideo -pix_fmt rgba -s:v 256x256 -framerate 30 -i - -c:v libx264 -pix_fmt yuv420p output.mp4
# ffmpeg -framerate 20 -i imgs/img%03d.jpg -c:v libx264 -pix_fmt yuv420p output.mp4
