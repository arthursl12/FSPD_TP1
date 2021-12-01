CXX := g++
MYDIR := $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
SRCDIR := src
BIN := bin
BUILDDIR := build
TARGET := main

SRCEXT := cpp
CXXFLAGS := -ftest-coverage -fprofile-arcs -g -Wall 
LIBS := -lpthread
INC := -I include -I third-party

MODULES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o, $(notdir $(MODULES)))
OBJDIR = $(addprefix build/,$(OBJECTS))

TESTDIR := tests
TESTSBINDIR = $(TESTDIR)/bin
TESTS1 = $(wildcard $(TESTDIR)/test_*.cpp) 
TESTS2 = $(patsubst %.cpp, %.o, $(TESTS1))

COVER = $(addsuffix .gcov,$(MODULES))

all: $(TARGET).o

$(OBJDIR): $(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@echo OBJETO: $@
	$(shell mkdir -p $(BUILDDIR))
	$(CXX) $(INC) $(CXXFLAGS) -c $(MYDIR)/$< -o $(MYDIR)/$@
	@echo ""

$(TARGET).o: $(OBJDIR)
	@echo COMPILANDO MAIN
	$(CXX) $(INC) $(CXXFLAGS) $(OBJDIR) $(TARGET).cpp $(LIBS) -o $(BIN)/$(TARGET)
	

# make tester: compila e roda todos os testes da pasta tests
# make tests/test_[nome].o : compila esse teste específico
# tester: $(TESTS2)
# $(TESTS2): %.o : %.cpp | $(TARGET).o
# 	$(shell mkdir -p $(TESTDIR))
# 	$(shell mkdir -p $(TESTSBINDIR))
# 	@echo TESTE: $<
# 	$(CXX) $(INC) $(CXXFLAGS) $< $(OBJDIR) $(LIBS) -o $(TESTSBINDIR)/$(patsubst %.o,%,$(notdir $@))
# 	$(TESTSBINDIR)/$(patsubst %.o,%,$(notdir $@))
# 	$(RM) test_*.gcno
# 	$(RM) test_*.gcda
# 	@echo ""

# make tester: compila e roda todos os testes da pasta tests
tester: $(TARGET).o 
	$(shell mkdir -p $(TESTDIR))
	$(shell mkdir -p $(TESTSBINDIR))
	@echo TESTES:
	$(CXX) $(INC) $(CXXFLAGS) $(TESTS1) $(OBJDIR) $(LIBS) -o $(TESTSBINDIR)/$(patsubst %.o,%,$(notdir $@))
	-$(TESTSBINDIR)/$(patsubst %.o,%,$(notdir $@))
	$(RM) test_*.gcno
	$(RM) test_*.gcda
	@echo ""

run:
	@./$(BIN)/main $(ARGS)

coverage: $(COVER) lcov
$(COVER): src/%.gcov : src/%
	@echo ""
	@echo POSIÇAO: $@
	gcov $(patsubst %.cpp.gcov,%.cpp,$@) -l -p -o build
	$(RM) *usr#include*


lcov:
	$(RM) coverage.info
	$(shell lcov -q --capture --directory build --output-file coverage.info)
	$(shell genhtml -q coverage.info --output-directory lcov_report)
	$(RM) *.cpp.gcov
	$(RM) *.gcov
	$(RM) *.gcno
	$(RM) coverage.info


clean:
	$(RM) -r build/* coverage/* *.gcda *.gcno *.gcov *.exe *.o bin/* tests/bin/* lcov_report

.PHONY: clean