include config.mk

OBJS = buffer.o md.o parse.o
OUT  = md

.PHONY: all clean install uninstall

all: $(OUT)

clean:
	rm -f $(OBJS) $(OUT)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(OUT) $(DESTDIR)$(PREFIX)/bin

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(OUT)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)
