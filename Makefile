PROJECT=atom
WARN = -Wall -Wextra -pedantic
LIB_CARBON=carbon/libcarbon.a

CFLAGS = -g ${WARN} -std=c11 -Icarbon -D_BSD_SOURCE

OBJS =  src/baud.o \
	src/connect.o \
	src/sleep.o \
	src/connected.o \
	src/info.o \
	src/channel.o \
	src/util.o


all: ${PROJECT}

${LIB_CARBON}:
	@$(MAKE) -C carbon

${PROJECT}: ${OBJS} ${LIB_CARBON} src/main.c
	${CC} -o $@ src/main.c ${CFLAGS} ${OBJS} ${LDFLAGS} ${LIB_CARBON}

clean:
	@$(MAKE) -C carbon clean
	rm -f ${OBJS} ${PROJECT} ${LIB_CARBON} *.core
