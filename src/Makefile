CC = g++
FLAGS = -Wall -O3 -std=c++0x
LMATH = -lm

OBJS = vm_info.o dag.o main.o
OBJS_ONE = vm_info.o dag.o main_oneeach.o
TARG = dagmdf

dagmdf: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

one_each: $(OBJS_ONE)
	$(CC) $(FLAGS) $(OBJS_ONE) -o one_each $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

clean:
	-rm -f *.o $(TARG) *~
