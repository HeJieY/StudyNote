.PHONY:all
all:volatile test
test:test.cc
	g++ -o $@ $^ -std=c++11
volatile:volatile.cc
	g++ -o $@ $^ -O3 -std=c++11
.PHONY:clean
clean:
	rm -rf test volatile