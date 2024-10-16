#include "TcpServer.hpp"
#include <memory>

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " local_port\n" << std::endl;
}

int main(int argc,char *argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        return 1;
    }
    uint16_t port=std::stoi(argv[1]);
    std::unique_ptr<TcpServer> tsvr=std::make_unique<TcpServer>(port);
    tsvr->InitServer();
    tsvr->Loop();
}