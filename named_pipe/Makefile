.PHONY:all
all:pipe_client pipe_server

pipe_server:PipeServer.cc
	g++ -o $@ $^ -std=c++11
pipe_client:PipeClient.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f pipe_client pipe_server