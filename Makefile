# EC Language Makefile
# Supports Windows (MinGW) and Linux/macOS

CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm

SRC = src/ec.c
TARGET_WIN = EC.exe
TARGET_UNIX = EC

# Detect OS
ifeq ($(OS),Windows_NT)
    TARGET = $(TARGET_WIN)
    RM = del /Q
else
    TARGET = $(TARGET_UNIX)
    RM = rm -f
endif

.PHONY: all clean test help

all: $(TARGET)

$(TARGET_WIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET_UNIX): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

windows:
	$(CC) $(CFLAGS) -o $(TARGET_WIN) $(SRC) $(LDFLAGS)

linux:
	$(CC) $(CFLAGS) -o $(TARGET_UNIX) $(SRC) $(LDFLAGS)

clean:
ifeq ($(OS),Windows_NT)
	-del /Q $(TARGET_WIN) 2>nul
else
	-rm -f $(TARGET_UNIX)
endif

test: $(TARGET)
	./$(TARGET) examples/01_hello.ec
	./$(TARGET) examples/09_multiplication.ec

help:
	@echo "EC Language Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all      - Build for current platform"
	@echo "  windows  - Build Windows executable (EC.exe)"
	@echo "  linux    - Build Linux/macOS executable (EC)"
	@echo "  clean    - Remove built executables"
	@echo "  test     - Run example programs"
	@echo "  help     - Show this help message"
