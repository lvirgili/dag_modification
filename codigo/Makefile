CC = gcc
CFLAGS = -g -std=c99 -pedantic
CFLAGS2 = -Wall -L/opt/xpressmp/lib -I/opt/xpressmp/include

LIBSXPRESS = -lxprb -lxprs
LIBSMATH = -lm

OBJS1=x_io.o x_dag.o
PROGRAM1=dagmdf
TEST=graph.o test.o
TPROG=test

dagmdf: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $(PROGRAM1) $(LIBSMATH)

test: $(TEST)
	$(CC) $(CFLAGS) $(TEST) -o $(TPROG)

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS2) -c $<

clean:
	-rm -f *.o $(PROGRAM1) $(TPROG) *~
