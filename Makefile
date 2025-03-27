# Define variables

CC = g++
CFLAGS = -std=c++20 -fdiagnostics-color=always -Wall -g -D_THREAD_SAFE
INCLUDES = -I include -I /opt/homebrew/include
LIBS = -L lib -L /opt/homebrew/lib -lSDL2 -lSDL2_image -framework OpenGL
EXTERNAL_SRCS = src/glad.c
EXTERNAL_LIBS = lib/libglfw.3.dylib lib/libassimp.5.dylib
SRCDIR = src
BUILDDIR = build
TARGET = bin/main

# List source files
SRCS := $(shell find $(SRCDIR) -type f -name "*.cpp")
OBJS := $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCS))

EXTERNAL_OBJS = $(EXTERNAL_SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

# Default target
all: $(TARGET)

# Rule for linking the target executable
$(TARGET): $(OBJS) $(EXTERNAL_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(EXTERNAL_LIBS)

# Rule for compiling source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Rule for compiling external source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# Phony targets
.PHONY: all clean