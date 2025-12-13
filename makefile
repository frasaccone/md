include config.mk

OBJS = md.o
OUT  = md

.PHONY: all

all: $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)
