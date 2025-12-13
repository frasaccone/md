include config.mk

OBJS = md.o
OUT  = md

.PHONY: all clean

all: $(OUT)

clean:
	rm -f $(OBJS) $(OUT)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJS):
	$(CC) $(CFLAGS) -c -o $@ $(@:.o=.c)
