CC=/usr/bin/g++
CFLAGS=-Wall
TESTSOURCES=./TestHistory.cpp
TESTOBJECTS=$(TESTSOURCES:.cpp=.o)
TESTEXECUTABLE=$(TESTSOURCES:.cpp=.exe)

all: $(TESTEXECUTABLE)

TestHistory.exe: TestHistory.cpp ../History.cpp ../History.h
	 $(CC) $(CFLAGS) TestHistory.cpp ../History.cpp -I. -o $@

run: $(TESTEXECUTABLE)
	$(TESTEXECUTABLE)
	
clean:
	rm -rf *.exe

