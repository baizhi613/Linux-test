#include<iostream>
#include<cerrno>
#include<cstring>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>

using namespace std;

void handler(int sig)
{
    cout<<"get a sig:"<<sig<<endl;
    exit(1);
}

int main()
{
    signal(SIGFPE,handler);
    int a=10;
    a/=0;
    while(true) sleep(1);
    return 0;
}
// int g_cnt=0;

// void handler(int sig)
// {
//     cout<<"get a sig:"<<sig<<"g_cnt:"<<g_cnt<<endl;
//     exit(0);
// }

// int main()
// {
//     signal(SIGALRM,handler);
//     alarm(1);
//     while (true)
//     {
//         g_cnt++;
//     }
//     // int cnt=0;
//     // while (true)
//     // {
//     //     cout<<"cnt:"<<cnt++<<endl;
//     // }
// }

// void handler(int signumber)
// {
//     std::cout<<"get a sinal,number is:"<<signumber<<std::endl;
// }

// int main()
// {
//     signal(2,handler);
//     int cnt=0;
//     while (true)
//     {
//         cout<<"cnt:"<<cnt++<<endl;
//         sleep(1);
//         if(cnt%5==0)
//         {
//             cout<<"send 2 to caller"<<endl;
//             raise(2);
//         }
//     } 
// }

// int main(int argc, char *argv[]) {
//     // 检查命令行参数的数量是否为3（包括程序名称本身）
//     if (argc != 3) {
//         std::cout << "Usage:" << argv[0] << " -signumber pid" << std::endl; // 打印使用说明
//         return 1; // 参数数量不正确，返回错误码1
//     }

//     // 从第一个命令行参数中提取信号编号，跳过前导的'-'字符
//     int signumber = std::stoi(argv[1] + 1);
//     // 从第二个命令行参数中提取进程ID
//     int pid = std::stoi(argv[2]);

//     // 使用kill函数发送信号给指定进程，返回0表示成功，-1表示失败
//     int n = kill(pid, signumber);
//     // 检查kill函数的返回值
//     if (n < 0) {
//         // 如果返回值小于0，说明发送信号失败，打印错误信息
//         std::cerr << "kill error," << strerror(errno) << std::endl;
//     }
//     // 如果没有错误，程序正常结束，返回0
//     return 0;
// }

// void handler(int signo)
// {
//     std::cout<<"get a sig,number is:"<<signo<<std::endl;
//     //exit(100);
// }

// int main()
// {
//     signal(SIGINT,handler);
//     while(true)
//     {
//         std::cout<<"I am activing...,pid:"<<getpid()<<std::endl;
//         sleep(1);
//     }
//     return 0;
// }