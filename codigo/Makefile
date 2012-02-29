CC = gcc
CFLAGS = -g -std=c99 -pedantic
CFLAGS2 = -Wall -L/opt/xpressmp/lib -I/opt/xpressmp/include

LIBSXPRESS = -lxprb -lxprs
LIBSMATH = -lm

OBJS1=x_io.o x_dag.o
PROGRAM1=dagmdf

dagmdf: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $(PROGRAM1) $(LIBSMATH)

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS2) -c $<

clean:
	-rm -f $(OBJS1) $(OBJS2) $(PROGRAM1) *~
