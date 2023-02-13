CC = gcc
LD = gcc
CFLAGS = -Wall -lm -O0
LDFLAGS = -Wall -lm -O0
RM = /bin/rm -f
OBJS = galsim_keywords.o
EXECUTABLE = galsim

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

galsim.o: galsim_keywords.c
	$(CC) -c galsim_keywords.c $(CFLAGS)

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
