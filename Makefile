CC = gcc
CFLAGS = -Wall -Wextra -pthread -std=c11
SRCS = server.c utils.c request.c response.c handler.c
OBJS = $(SRCS:.c=.o)
TARGET = http_server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)