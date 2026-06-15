CC = gcc

# Compila com otimizações (-O2) para melhor representar o comportamento da cache em execução real.
CFLAGS = -Wall -Wextra -Werror -g -O2

TARGET = teste3

SRCS = main.c matrix_mult.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c matrix_mult.h
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(TARGET)

