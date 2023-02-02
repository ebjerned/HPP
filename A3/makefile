CC = gcc
LD = gcc
CFLAGS = -Wall
LDFLAGS = -Wall
RM = /bin/rm -f
OBJS = galsim.o
EXECUTABLE = galsim

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $(EXECUTABLE)

galsim.o: galsim.c
	$(CC) $(CFLAGS) -c galsim.c

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
