PREFIX    = /usr/local
MANPREFIX = $(PREFIX)/share/man

CC = cc

CFLAGS  = -std=c99 -Wall -Wextra -Wpedantic -D_DEFAULT_SOURCE \
                                            -D_POSIX_C_SOURCE=200809L \
                                            -D_XOPEN_SOURCE=70
LDFLAGS =
