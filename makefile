# ===============================
# 	 	   DEFINITIONS
# ===============================
# Directories
SRCDIR := src
BINDIR := bin
BUILDDIR := build
TARGET := main

# Compiler
CXX := g++
CXXFLAGS := -ftest-coverage -fprofile-arcs -g -Wall 
LIBS := -lpthread
INC := -I include -I third-party

# Module auto-discovery
MODULES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(notdir $(MODULES)))
OBJDIR = $(addprefix $(BUILDDIR)/,$(OBJECTS))

# Tests auto-discovery
TESTDIR := tests
TESTSBINDIR = $(TESTDIR)/bin
TESTS1 = $(wildcard $(TESTDIR)/test_*.cpp) 
TESTS2 = $(patsubst %.cpp, %.o, $(TESTS1))
TESTBASE := test_base.cpp

# Coverage files
COVER = $(addsuffix .gcov,$(MODULES))

# ===============================
# 	 	  COMPILATION
# ===============================
# Base compilation rule
all: $(TARGET).o

# Module compilation rule
$(OBJDIR): $(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@echo OBJECT: $@
	$(shell mkdir -p $(BUILDDIR))
	$(CXX) $(INC) $(CXXFLAGS) -c $< -o $ $@
	@echo ""

# Main module compilation rule
$(TARGET).o: $(OBJDIR)
	@echo COMPILING MAIN
	$(shell mkdir -p $(BINDIR))
	$(CXX) $(INC) $(CXXFLAGS) $(OBJDIR) $(TARGET).cpp $(LIBS) -o $(BINDIR)/$(TARGET)

# Base execution rule
run:
	@./$(BINDIR)/main $(ARGS)

# ===============================
# 	 	      TESTING
# ===============================
# Base testing rule: compile and run all tests inside 'tests' folder
tester: $(TARGET).o 
	$(shell mkdir -p $(TESTDIR))
	$(shell mkdir -p $(TESTSBINDIR))
	$(CXX) $(INC) $(CXXFLAGS) $(TESTS1) $(OBJDIR) $(LIBS) -o $(TESTSBINDIR)/$(patsubst %.o,%,$(notdir $@))
	-$(TESTSBINDIR)/$(patsubst %.o,%,$(notdir $@))
	$(RM) test_*.gcno
	$(RM) test_*.gcda
	@echo ""

# Aditional rule: compile and run a single test file
# 		Use: make tests/test_[name].o
$(TESTS2): %.o : %.cpp | $(TARGET).o
	$(shell mkdir -p $(TESTDIR))
	$(shell mkdir -p $(TESTSBINDIR))
	@echo TEST: $<
	$(CXX) $(INC) $(CXXFLAGS) $< $(TESTDIR)/$(TESTBASE) $(OBJDIR) $(LIBS) -o $(TESTSBINDIR)/$(patsubst %.o,%,$(notdir $@))
	-$(TESTSBINDIR)/$(patsubst %.o,%,$(notdir $@))
	$(RM) test_*.gcno
	$(RM) test_*.gcda
	@echo ""

# More tests: compile and run a tests from another folder
TESTDIR2 := tests1
TESTSDIR2 = $(wildcard $(TESTDIR2)/test_*.cpp) 
tester1: $(TARGET).o
	$(shell mkdir -p $(TESTDIR2))
	$(shell mkdir -p $(TESTDIR2)/$(BINDIR))
	$(CXX) $(INC) $(CXXFLAGS) $(TESTSDIR2) $(OBJDIR) $(LIBS) -o $(TESTDIR2)/$(BINDIR)/$(patsubst %.o,%,$(notdir $@))
	-$(TESTDIR2)/$(BINDIR)/$(patsubst %.o,%,$(notdir $@))
	$(RM) test_*.gcno
	$(RM) test_*.gcda
	@echo ""

# Run all tests
tester_all: tester tester1

# ===============================
# 	 	    COVERAGE
# ===============================
# Base rule: run gcov for all modules
coverage: $(COVER) lcov
$(COVER): src/%.gcov : src/%
	@echo ""
	@echo COV FOR: $@
	gcov $(patsubst %.cpp.gcov,%.cpp,$@) -l -p -o build
	$(RM) *usr#include*

# Pretty report using lcov
lcov:
	$(RM) coverage.info
	$(shell lcov -q --capture --directory build --output-file coverage.info)
	$(shell genhtml -q coverage.info --output-directory lcov_report)
	$(RM) *.cpp.gcov
	$(RM) *.gcov
	$(RM) *.gcno
	$(RM) coverage.info

# ===============================
# 	 	    CLEANING
# ===============================
clean:
	$(RM) -r build/* coverage/* *.gcda *.gcno *.gcov *.exe *.o bin/* tests/bin/* lcov_report main tests1/bin/*

.PHONY: clean