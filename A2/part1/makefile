CC = gcc
CFLAGS = -Wall
RM = /bin/rm -f
OBJS = pascals_triangle.o
EXECUTABLE = triang
TEST_N = 10

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS)

debug: $(EXECUTABLES)
	valgrind ./$(EXECUTABLE) $(TEST_N)
	

pascals_triangle.o: pascals_triangle.c
	$(CC) $(CFLAGS) -c pascals_triangle.c

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
