TARGET = pa04
GCC = gcc
CFLAGS = -g -Wall -Wshadow -Werror -lm
CC = $(GCC) $(CFLAGS)
SRCS = answer04.c
OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --verbose --log-file

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

test:
	$(VALGRIND)=./logfile ./pa04 

.c.o:
	$(GCC) $(CFLAGS) -c $*.c

clean:
	rm -f $(OBJS) $(TARGET)
