CC = gcc

# Compila com otimizações (-O2) para melhor representar o comportamento da cache em execução real.
CFLAGS = -Wall -Wextra -Werror -g -O2

TARGET = teste2

SRCS = main.c cache_size.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c cache_size.h
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OBJS) $(TARGET)

