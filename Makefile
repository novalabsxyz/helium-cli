PROJECT=helium
WARN = -Wall -Wextra -pedantic
LIB_HELIUM_CLIENT=helium-client/libhelium-client.a

CFLAGS = -g ${WARN} -std=c11 -Ihelium-client -D_BSD_SOURCE -D_DEFAULT_SOURCE

OBJS =  src/baud.o \
	src/connect.o \
	src/sleep.o \
	src/reset.o \
	src/connected.o \
	src/info.o \
	src/channel.o \
	src/util.o


all: ${PROJECT}

${LIB_HELIUM_CLIENT}:
	@$(MAKE) -C helium-client

${PROJECT}: ${OBJS} ${LIB_HELIUM_CLIENT} src/main.c
	${CC} -o $@ src/main.c ${CFLAGS} ${OBJS} ${LDFLAGS} ${LIB_HELIUM_CLIENT}

.PHONY: clean
clean:
	@$(MAKE) -C helium-client clean
	rm -f ${OBJS} ${PROJECT} ${LIB_HELIUM_CLIENT} *.core
