CC = gcc
CFLAGS = -g -std=c99 -pedantic
CFLAGS2 = -Wall -L/opt/xpressmp/lib -I/opt/xpressmp/include

LIBSXPRESS = -lxprb -lxprs
LIBSMATH = -lm

VG = /home/cchaves/valgrind3.5/bin/valgrind
VGFLAGS = --tool=memcheck --track-origins=yes --leak-check=full --show-reachable=yes

OBJS=x_io.o x_form.o x_relax.o x_list.o tvms.o
OBJS1=x_io.o x_dag.o
OBJS2=x_io.o heft.o
PROGRAM=tvms
PROGRAM1=dagmdf
PROGRAM2=heft

all : tvms dagmdf heft

tvms: $(OBJS)
	$(CC) $(CFLAGS) $(CFLAGS2) $(OBJS) -o $(PROGRAM) $(LIBSXPRESS) #$(LIBSMATH)

dagmdf: $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $(PROGRAM1) $(LIBSMATH)

heft: $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) -o $(PROGRAM2) $(LIBSMATH)

%.o: %.c
	$(CC) $(CFLAGS) $(CFLAGS2) -c $<

clean:
	-rm -f $(OBJS) $(OBJS1) $(OBJS2) $(PROGRAM) $(PROGRAM1) $(PROGRAM2) *~ core*

valgrind:
	$(VG) $(VGFLAGS) $(PROGRAM)
 
