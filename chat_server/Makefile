.PHONY:all
all:udpserver udpclient

udpserver:Main.cc
	g++ -o $@ $^ -std=c++14 -lpthread
udpclient:UdpClient.cc
	g++ -o $@ $^ -std=c++14 -lpthread
.PHONY:clean
clean:
	rm -f udpserver udpclient