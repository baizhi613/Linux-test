.PHONY:all
all:tcpserver tcpclient
tcpserver:Main.cc
	g++ -o $@ $^ -std=c++14
tcpclient:MainClient.cc
	g++ -o $@ $^ -std=c++14
.PHONY:clean
clean:
	rm -f tcpserver,tcpClient