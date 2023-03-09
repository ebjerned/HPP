CC = gcc
LD = gcc
CFLAGS = -Wall -lm -g
LDFLAGS = -Wall -lm -g
RM = /bin/rm -f
OBJS = gol3_1.o
EXECUTABLE = gol3

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

galsim.o: gol3_1.c
	$(CC) -c gol3_1.c $(CFLAGS)

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
