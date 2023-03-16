CC = gcc
LD = gcc
CFLAGS = -Wall -lm -g -fopt-info-vec -fopenmp -O3
LDFLAGS = -Wall -lm -g -fopt-info-vec -fopenmp -O3
RM = /bin/rm -f
OBJS = gol3_2.o
EXECUTABLE = gol3

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

galsim.o: gol3_2.c
	$(CC) -c gol3_2.c $(CFLAGS)

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
