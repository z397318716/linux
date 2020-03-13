source = $(wildcard *.c)
target = $(patsubast %.c, %, $(source))
cflags = -wall - g
all:$(target)

$(target):%:%.c
	$(gcc) $< $(cflags) -o $@


