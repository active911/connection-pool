#GEN_SRC := tests.cpp example.cpp
DEP_HEAD := Fastcache.h
#gGEN_OBJ := $(patsubst %.cpp,%.o, $(GEN_SRC))

#THRIFT_DIR := /usr/local/include/thrift
#INC := -I$(THRIFT_DIR) -Igen-cpp/ -Iinclude/ -Iinih/ -Iinih/cpp/

all: test example


test: test/test_1 test/test_2 test/test_3 test/test_4 test/test_5
	/usr/bin/perl run_tests.pl

example: example.o 
	$(CXX) $^ -o $@ -L/usr/local/lib  -lboost_thread

test/test_%: test/test_%.o
	$(CXX) $^ -o $@ -L/usr/local/lib -lboost_thread

test/%.o: test/%.cpp $(DEP_HEAD)
	$(CXX) -Wall $(INC) -c $< -o $@

%.o: %.cpp $(DEP_HEAD)
	$(CXX) -Wall $(INC) -c $< -o $@


clean:
	$(RM) *.o  test/*.o test/test_1 test/test_2 test/test_3 test/test_4 test/test_5 example

