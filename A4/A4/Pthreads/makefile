CC = gcc
LD = gcc
CFLAGS = -Wall -lm -O2 -ffast-math -march=native -pthread
LDFLAGS = -Wall -lm -O2 -ffast-math -march=native -pthread
RM = /bin/rm -f
OBJS = galsim.o
EXECUTABLE = galsim

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

galsim.o: galsim.c
	$(CC) -c galsim.c $(CFLAGS)

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
