# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -Iinclude `root-config --cflags`

# Linker flags
LDFLAGS = `root-config --glibs`

# Directories
BINDIR = bin
OBJDIR = obj
SRCDIR = src
INCDIR = include
MAINDIR = .

# Target executable
TARGET = $(BINDIR)/main

# Source and object files
SOURCES = $(wildcard $(SRCDIR)/*.cc) $(MAINDIR)/main.cc
OBJECTS = $(patsubst %.cc, $(OBJDIR)/%.o, $(notdir $(SOURCES)))

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/main.o: $(MAINDIR)/main.cc
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS) 

.PHONY: all clean