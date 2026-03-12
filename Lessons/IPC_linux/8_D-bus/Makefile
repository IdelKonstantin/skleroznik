CC = gcc
CFLAGS = -Wall -Wextra -g
PKG_CONFIG = pkg-config

# Получаем флаги компиляции и линковки для dbus-1
CFLAGS += $(shell $(PKG_CONFIG) --cflags dbus-1)
LDFLAGS += $(shell $(PKG_CONFIG) --libs dbus-1)

all: robot_server robot_client

robot_server: robot_server.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

robot_client: robot_client.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f robot_server robot_client

.PHONY: all clean