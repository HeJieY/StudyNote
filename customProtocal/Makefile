.PHONY:all
all:tcpServer tcpClient
tcpServer:NetCalServer.cpp
	g++ -o $@ $^ -std=c++14 -ljsoncpp
tcpClient:NetClient.cpp
	g++ -o $@ $^ -std=c++14 -ljsoncpp
.PHONY:clean
clean:
	rm -rf tcpClient tcpServer
