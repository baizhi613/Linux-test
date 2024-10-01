.PHONY:all
all:udpserver udpclient
udpserver:Main.cc
	g++ -o $@ $^ -std=c++14
udpclient:UdpClient.cc
	g++ -o $@ $^ -std=c++14
.PHONY:clean
clean:
	rm -f udpserver,udpClient