include config.mk

proj_libs := -ljansson
headers   := tui.h util.h page.h config.h

.PHONY: all
all: proj

proj: main.o page.o table.o util.o tui.o
	gcc $(LDFLAGS) $(proj_libs) -o $@ $^

main.o: main.c $(headers)
	gcc $(CFLAGS) -c $<

page.o: page.c $(headers)
	gcc $(CFLAGS) -c $<

table.o: table.c $(headers)
	gcc $(CFLAGS) -c $<

util.o: util.c $(headers)
	gcc $(CFLAGS) -c $<

tui.o: tui.c $(headers)
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
