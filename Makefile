PROJECT=atom
WARN = -Wall -Wextra -pedantic
LIB_CARBON=carbon/libcarbon.a

CFLAGS = -g ${WARN} -std=c11 -I../carbon -D_BSD_SOURCE

OBJS =  src/connect.o \
	src/sleep.o \
	src/connected.o \
	src/info.o \
	src/send.o

all: ${PROJECT}

${LIB_CARBON}:
	@$(MAKE) -C carbon

${PROJECT}: ${OBJS} ${LIB_CARBON} src/main.c
	${CC} -o $@ src/main.c ${CFLAGS} ${OBJS} ${LDFLAGS} ${LIB_CARBON}

clean:
	@$(MAKE) -C carbon clean
	rm -f ${OBJS} ${PROJECT} ${LIB_CARBON} *.core
