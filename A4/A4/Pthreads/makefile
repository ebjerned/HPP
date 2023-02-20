CC = gcc
LD = gcc
CFLAGS = -Wall -lm -O2 -ffast-math -march=native -pthread
LDFLAGS = -Wall -lm -O2 -ffast-math -march=native -pthread
RM = /bin/rm -f
OBJS = galsim_naive.o
EXECUTABLE = galsim

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

galsim.o: galsim_naive.c
	$(CC) -c galsim_naive.c $(CFLAGS)

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
