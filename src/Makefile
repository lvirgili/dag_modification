CC = g++
FLAGS = -Wall -O3 -std=c++0x
LMATH = -lm

OBJS = vm_info.o dag.o main.o
OBJS_ONE = vm_info.o dag.o main_oneeach.o
OBJS_ONLY = vm_info.o dag.o main_onlyone.o
TARG = path_mdf
TARG_ONE = one_each
TARG_ONLY = only_one

path_mdf: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

one_each: $(OBJS_ONE)
	$(CC) $(FLAGS) $(OBJS_ONE) -o $(TARG_ONE) $(LMATH)

only_one: $(OBJS_ONLY)
	$(CC) $(FLAGS) $(OBJS_ONLY) -o $(TARG_ONLY) $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

clean:
	-rm -f *.o $(TARG) $(TARG_ONE) $(TARG_ONLY) *~
