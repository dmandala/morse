ifndef _ARCH
_ARCH := $(shell arch)
export _ARCH
# set to x86_64 on PC
# set to armv7l on Raspberry Pi 3B
endif

CC=/usr/bin/gcc
# Always be pedantic on errors
CFLAGS=-I. -Wall -pthread
LDFLAGS=-L/usr/local/lib

# If we are building on a PC there is no GPIO
#ifeq ($(_ARCH),x86_64) 
#define x86_64
#LIBS=-lm -lconfuse
#endif

# if buiding on ARM (Raspberry Pi) there is GPIO and need libs for it
#ifeq ($(_ARCH),armv7l)
#define RASPBERRY_PI
#LIBS=-lm -lconfuse -lrt

LIBS=-lm -lconfuse -lpigpiod_if2 -lrt

#endif

DEPS = morse.h

OBJ = morse.o display_morse.o process_command_line.o process_config_file.o process_file.o


%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

morse: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

.PHONY: clean

clean:
	/bin/rm -f *.o *~ core


.PHONY: test5

test5:
	./morse -l1 -s5 -tS -f./text_files/text.txt -p8

.PHONY: test10

test10:
	./morse -l1 -s10 -tS -f./text_files/text.txt -p8

.PHONY: test15

test15:
	./morse -l2 -s15 -tS -f./text_files/text.txt -p8

.PHONY: test20

test20:
	./morse -l4 -s20 -tS -f./text_files/text.txt -p8

.PHONY: test30

test30:
	./morse -l4 -s30 -tS -f./text_files/text.txt -p8

.PHONY: test50

test50:
	./morse -l6 -s50 -tS -f./text_files/text.txt -p8

.PHONY: test75

test75:
	./morse -l8 -s75 -tS -f./text_files/text.txt -p8

.PHONY: install

install:
	/bin/cp morse /usr/local/bin/morse
	/bin/cp config_samples/etc_morsecode.cfg /etc/morsecode.cfg
	/bin/cp config_samples/dot_morsecode.cfg /home/$$SUDO_USER/.morsecode.cfg
	/bin/chown $$SUDO_USER:$$SUDO_USER /home/$$SUDO_USER/.morsecode.cfg


all: clean morse test20
