CC = gcc
LD = gcc
CFLAGS = -Wall -lm -O3
LDFLAGS = -Wall -lm -O3
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
