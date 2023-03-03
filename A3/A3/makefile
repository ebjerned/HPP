CC = gcc
LD = gcc
CFLAGS = -Wall -lm -fopt-info-vec -O3 -ffast-math -march=native -ftree-vectorize -fopt-info-vec-missed
LDFLAGS = -Wall -lm -fopt-info-vec -O3 -ffast-math -march=native -ftree-vectorize -fopt-info-vec-missed
RM = /bin/rm -f
OBJS = galsim_after_feedback.o
EXECUTABLE = galsim_new

all:$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)

galsim.o: galsim_after_feedback.c
	$(CC) -c galsim_after_feedback.c $(CFLAGS)

clean:
	$(RM) $(EXECUTABLE) $(OBJS)
