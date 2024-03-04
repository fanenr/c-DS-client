MODE = debug

include config.mk

proj_srcs := main.c page.c table.c util.c tui.c
proj_objs := $(proj_srcs:%.c=%.o)
proj_libs := -ljansson
headers   := tui.h util.h page.h config.h

.PHONY: all
all: proj

proj: $(proj_objs)
	gcc $(LDFLAGS) $(proj_libs) -o $@ $^

$(proj_objs): %.o: %.c $(headers)
	gcc $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o proj

.PHONY: purge
purge: clean
	rm -f *.json

.PHONY: json
json: purge
	bear -- make
