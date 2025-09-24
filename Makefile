# CPP Fundamentals Makefile
# Quick build system for development

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -g -O0
INCLUDES = -Isrc -Iutils
SRCDIR = src
BUILDDIR = build
BINDIR = bin

# Create directories
$(shell mkdir -p $(BUILDDIR) $(BINDIR))

# Module targets
FUNDAMENTALS_SRCS = $(wildcard $(SRCDIR)/01_fundamentals/*.cpp)
MEMORY_SRCS = $(wildcard $(SRCDIR)/02_memory_management/*.cpp)
STL_SRCS = $(wildcard $(SRCDIR)/03_stl_containers/*.cpp)
ALGORITHMS_SRCS = $(wildcard $(SRCDIR)/04_algorithms/*.cpp)
TEMPLATES_SRCS = $(wildcard $(SRCDIR)/05_templates/*.cpp)
CONCURRENCY_SRCS = $(wildcard $(SRCDIR)/06_concurrency/*.cpp)

# Default target
all: syntax types control functions classes memory stl algorithms templates concurrency

# Individual module targets
syntax: $(BINDIR)/syntax_basics
types: $(BINDIR)/data_types  
control: $(BINDIR)/control_flow
functions: $(BINDIR)/functions_demo
classes: $(BINDIR)/classes_demo

$(BINDIR)/syntax_basics: $(SRCDIR)/01_fundamentals/syntax_basics.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/data_types: $(SRCDIR)/01_fundamentals/data_types.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/control_flow: $(SRCDIR)/01_fundamentals/control_flow.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/functions_demo: $(SRCDIR)/01_fundamentals/functions.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/classes_demo: $(SRCDIR)/01_fundamentals/classes.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/memory_management: $(MEMORY_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/stl_containers: $(STL_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/algorithms: $(ALGORITHMS_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/templates: $(TEMPLATES_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BINDIR)/concurrency: $(CONCURRENCY_SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -pthread -o $@ $^

# Test targets
test: build_tests
	./build/tests/all_tests

build_tests:
	mkdir -p build/tests
	$(CXX) $(CXXFLAGS) $(INCLUDES) -lgtest -lgtest_main -pthread \
		$(wildcard tests/*.cpp) -o build/tests/all_tests

# Clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR)

# Install (for development)
install: all
	@echo "Built executables in $(BINDIR)/"
	@echo "Run individual modules:"
	@echo "  ./bin/syntax_basics"
	@echo "  ./bin/data_types"
	@echo "  ./bin/control_flow"
	@echo "  ./bin/functions_demo"
	@echo "  ./bin/classes_demo"
	@echo "  ./bin/memory_management"
	@echo "  ./bin/stl_containers" 
	@echo "  ./bin/algorithms"
	@echo "  ./bin/templates"
	@echo "  ./bin/concurrency"

.PHONY: all syntax types control functions classes memory stl algorithms templates concurrency test build_tests clean install