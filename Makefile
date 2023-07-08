CC := g++
CFLAGS := -Os -march=native -pipe -std=gnu++17 -Wall

TARGET := broff
INSTALL_DIR := /usr/local/bin

.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): broff.cpp
	$(CC) $(CFLAGS) $^ -o $@

install: $(TARGET)
	cp $(TARGET) $(INSTALL_DIR)

uninstall:
	rm $(INSTALL_DIR)/$(TARGET)

clean:
	rm $(TARGET)
