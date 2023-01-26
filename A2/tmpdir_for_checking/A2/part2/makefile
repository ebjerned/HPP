CC = gcc
CFLAGS = -Wall -g
RM = /bin/rm -f
OBJS = readfile.o
EXECUTABLE = readfile

all:$(EXECUTABLE)


$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS)

readfile.o: readfile.c
	$(CC) $(CFLAGS) -c readfile.c

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
