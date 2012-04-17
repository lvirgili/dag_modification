CC = g++
FLAGS = -std=c++0x -Wall
LMATH = -lm

OBJS = dag.o main.o
TARG = dagmdf

dagmdf: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

clean:
	-rm -f *.o $(TARG) *~
