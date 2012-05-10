CC = g++
FLAGS = -Wall -O3 -std=c++0x
LMATH = -lm

OBJS = vm_info.o dag.o main.o
TARG = dagmdf

dagmdf: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

clean:
	-rm -f *.o $(TARG) *~
