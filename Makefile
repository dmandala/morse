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
	rm -f *.o *~ core 

test:
	./morse -l1 -s50 -tS -f./text_files/text.txt -p24


all: clean morse test
