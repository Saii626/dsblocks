PREFIX := /usr/local
#PREFIX := ${HOME}/.local

CC := gcc
CFLAGS := -Wall -Wextra -pthread

X11CFLAGS := $(shell pkg-config --cflags x11)
X11LIBS := $(shell pkg-config --libs x11)

BLOCKS := $(wildcard blocks/*.c)

.PHONY: all debug release clean install uninstall

all: dsblocks sigdsblocks/sigdsblocks xgetrootname/xgetrootname

debug: CFLAGS += -DDEBUG
debug: all

release: CFLAGS += -O3
release: all

dsblocks.o: dsblocks.c blocks.h shared.h taskqueue.h
	${CC} -o $@ -c ${CFLAGS} -Wno-missing-field-initializers -Wno-unused-parameter ${X11CFLAGS} $<

taskqueue.o: taskqueue.c taskqueue.h
	${CC} -o $@ -c ${CFLAGS} -Wno-missing-field-initializers -Wno-unused-parameter $<

util.o: util.c util.h shared.h
	${CC} -o $@ -c ${CFLAGS} ${X11CFLAGS} $<

blocks/%.o: blocks/%.c blocks/%.h util.h shared.h
	${CC} -o $@ -c ${CFLAGS} -Wno-unused-parameter $<

dsblocks: dsblocks.o util.o taskqueue.o ${BLOCKS:c=o}
	${CC} -o $@ $^ ${X11LIBS} ${CFLAGS}

sigdsblocks/sigdsblocks: sigdsblocks/sigdsblocks.c
	${CC} -o $@ ${CFLAGS} $<

xgetrootname/xgetrootname: xgetrootname/xgetrootname.c
	${CC} -o $@ ${CFLAGS} ${X11CFLAGS} $< ${X11LIBS}

clean:
	rm -f blocks/*.o *.o dsblocks sigdsblocks/sigdsblocks xgetrootname/xgetrootname

install:
	$(MAKE) clean
	$(MAKE) release
	mkdir -p ${DESTDIR}${PREFIX}/bin
	install -m 0755 dsblocks ${DESTDIR}${PREFIX}/bin/dsblocks
	install -m 0755 sigdsblocks/sigdsblocks ${DESTDIR}${PREFIX}/bin/sigdsblocks
	install -m 0755 xgetrootname/xgetrootname ${DESTDIR}${PREFIX}/bin/xgetrootname

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/dsblocks ${DESTDIR}${PREFIX}/bin/sigdsblocks ${DESTDIR}${PREFIX}/bin/xgetrootname

.PHONY: all clean install uninstall
