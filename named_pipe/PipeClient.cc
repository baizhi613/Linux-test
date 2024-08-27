#include"Comm.hpp"

int main()
{
    int wfd=open(Path,O_WRONLY);
    if(wfd<0)
    {
        cerr<<"open failed,errno:"<<errno<<",errstring:"<<strerror(errno)<<endl;
        return 1;
    }
    string inbuffer;
    while (true)
    {
        cout<<"Please Enter Your Message#";
        std::getline(cin,inbuffer);
        if(inbuffer=="quit")break;
        ssize_t n=write(wfd,inbuffer.c_str(),inbuffer.size());
        if(n<0)
        {
            cerr<<"write failed,errno:"<<errno<<",errstring:"<<strerror(errno)<<endl;
            break;
        }
    }
    
    close(wfd);
    return 0;
}