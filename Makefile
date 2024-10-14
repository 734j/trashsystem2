CC=g++
CFLAGS_TESTBIN=-O0 -Wfatal-errors -Wall -Werror -Wextra -g3 -fsanitize=address -fsanitize=leak -Wpedantic -Wformat=2 -Wshadow -Wformat-truncation=2 -Wformat-overflow -fno-common -std=c++20 -DDEBUG_S
CFLAGS=-O3 -flto -march=native -DNDEBUG -fomit-frame-pointer -s -static -std=c++20
TARGET=tsr
TESTTARGET=tsr-TESTING
INSTALL_DIRECTORY=/usr/local/bin
MAKEFLAGS += 
SRCS=trashsystem2.cpp

all: release
clean:
	rm -f $(TARGET)
	rm -f test/$(TESTTARGET)

tests:
	$(CC) $(CFLAGS_TESTBIN) $(SRCS) -o test/$(TESTTARGET)

install:
	cp $(TARGET) $(INSTALL_DIRECTORY)

release:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)
