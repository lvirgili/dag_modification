CC = g++
FLAGS = -std=c++0x -Wall -Wfatal-errors
LMATH = -lm

OBJS = vm_info.o dag.o main.o
TARG = dagmdf

dagmdf: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

clean:
	-rm -f *.o $(TARG) *~
