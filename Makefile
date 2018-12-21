CC= gcc
VPATH= src/ src/fifo
CFLAGS += -Wall -Wextra -Werror -pedantic -std=c99
LDLIBS += ${shell sdl2-config --libs}
LDLIBS += -lSDL2_ttf
LDLIBS += -lSDL2_mixer
LDLIBS += -lSDL2_image
LDLIBS += -lm

SRC= anim.c reinit.c mobs.c audio.c hud.c render.c body.c game.c control.c physics.c parser.c descr.c modify.c reify.c light.c path.c
OBJS= ${SRC:.c=.o}
BIN= kodama

CK_SRC= checker.c
CK_OBJ= ${CK_SRC:.c=.o}
CK_BIN= checker

.PHONY: all clean checker

all: ${BIN}

${BIN}: CFLAGS+= ${shell sdl2-config --cflags}
${BIN}: ${OBJS}

${CK_BIN}: ${CK_OBJ}
	gcc ${CK_OBJ} -o ${CK_BIN}

asan: CFLAGS+= -fsanitize=address
asan: LDFLAGS+= -fsanitize=address
asan: all

gdb: CFLAGS+= -g2
gdb: all

clean:
	${RM} ${OBJS} ${BIN} ${CK_OBJ} ${CK_BIN}
