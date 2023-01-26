CC = gcc
CFLAGS = -Wall -g
RM = /bin/rm -f
OBJS = january.o
EXECUTABLE = january
DEBUG_INPUT = A 2 2 2 A 10 10 10 A 7 7 7 A 1 1 1 A 2 6 6 Q


all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS)

debug: $(EXECUTABLE)
	valgrind -s --track-origins=yes ./$(EXECUTABLE) $(DEBUG_INPUT)

january.o: january.c
	$(CC) $(CFLAGS) -c january.c

clean:
	$(RM) $(EXECUTABLE) $(OBJS)

