.PHONY:all
all:ClientUdp ServerUdp
ServerUdp:EchoServerMain.cc
	g++ -o $@ $^ -std=c++14
ClientUdp:EchoClient.cc
	g++ -o $@ $^ -std=c++14
.PHONY:clean
clean:
	rm -rf ServerUdp ClientUdp
