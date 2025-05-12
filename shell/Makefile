TARGET = myShell

CC = gcc
CFLAGS = -Wall -g

SRCS = main.c prompt.c history.c builtin.c execommand.c shellfunct.c

OBJS = $(SRCS:.c=.o)

LIBS = -lreadline -lncurses

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

check:
	@ldconfig -p | grep readline > /dev/null || echo "⚠ readline 또는 ncurses 라이브러리가 없습니다. 설치하세요:"
	@ldconfig -p | grep readline > /dev/null || echo "  sudo apt install libreadline-dev libncurses-dev"
