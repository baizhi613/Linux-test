#include <iostream>
#include <string>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

void Usage(std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " serverip serverport\n"
              << std::endl;
}

// ./tcp_client serverip serverport
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "socket error" << std::endl;
        exit(2);
    }

    // tcp client 要bind，不要显示的bind.
    struct sockaddr_in server;
    // 构建目标主机的socket信息
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);
    server.sin_addr.s_addr = inet_addr(serverip.c_str());

    int n = connect(sockfd, (struct sockaddr *)&server, sizeof(server));
    if (n < 0)
    {
        std::cerr << "connect error" << std::endl;
        exit(3);
    }

    while(true)
    {
        std::cout << "Please Enter# ";
        std::string outstring;
        std::getline(std::cin, outstring);

        ssize_t s = send(sockfd, outstring.c_str(), outstring.size(), 0); //write
        if(s > 0)
        {
            char inbuffer[1024];
            ssize_t m = recv(sockfd, inbuffer, sizeof(inbuffer)-1, 0);
            if(m > 0)
            {
                inbuffer[m] = 0;
                std::cout << inbuffer<< std::endl;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    //shutdown(sockfd, SHUT_WR);
    ::close(sockfd);
    return 0;
}
