include config.mk
LDFLAGS += -ljansson

headers := tui.h util.h config.h

.PHONY: all
all: proj

proj:	main.o util.o tui.o
	gcc $(LDFLAGS) -o $@ $^

main.o: main.c $(headers)
	gcc $(CFLAGS) -c $<

tui.o: tui.c $(headers)
	gcc $(CFLAGS) -c $<

util.o:	util.c $(headers)
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
