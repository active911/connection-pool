DEP_HEAD := DummyConnection.h

all: test


test: test/test_1
	/usr/bin/perl run_tests.pl

example: example.o 
	$(CXX) $^ -o $@ -L/usr/local/lib 

test/test_%: test/test_%.o
	$(CXX) $^ -o $@ -L/usr/local/lib

test/%.o: test/%.cpp $(DEP_HEAD)
	$(CXX) -Wall $(INC) -c $< -o $@

%.o: %.cpp $(DEP_HEAD)
	$(CXX) -Wall $(INC) -c $< -o $@


clean:
	$(RM) *.o  test/*.o test/test_1 

