.PHONY:all
all:shm_client shm_server

shm_server:ShmServer.cc
	g++ -o $@ $^ -std=c++11
shm_client:ShmClient.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f shm_client shm_server