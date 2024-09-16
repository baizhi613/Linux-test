#include<iostream>
#include<cerrno>
#include<cstring>
#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<assert.h>

using namespace std;

void Print(sigset_t &pending)
{
    // 打印当前进程的挂起信号位图
    cout << "curr process pending:";
    // 遍历信号编号，从31（最大的信号编号）到1
    for (int sig = 31; sig >= 1; sig--)
    {
        // 使用sigismember函数检查信号是否在挂起信号集中
        if (sigismember(&pending, sig))
        {
            // 如果信号在挂起信号集中，打印1
            cout << "1";
        }
        else
        {
            // 如果信号不在挂起信号集中，打印0
            cout << "0";
        }
    }
    // 换行
    cout << endl;
}

void handler(int signo)
{
    // 信号处理函数
    cout << "signal:" << signo << endl;
    // 创建一个信号集，用于存储挂起的信号
    sigset_t pending;
    // 初始化信号集为空集
    sigemptyset(&pending);
    // 创建一个无限循环，不断检查是否有信号被挂起
    while (true)
    {
        // 使用sigpending函数获取当前进程的挂起信号集
        sigpending(&pending);
        // 调用Print函数打印挂起信号位图
        Print(pending);
        // 暂停进程执行1秒
        sleep(1);
    }
}

int main()
{
    // 定义信号处理结构体
    struct sigaction act, oact;
    // 设置信号处理函数为handler
    act.sa_handler = handler;
    // 设置信号行为的标志为0
    act.sa_flags = 0;
    // 初始化信号掩码为空集
    sigemptyset(&act.sa_mask);
    // 使用sigaction函数设置SIGINT信号的处理方式
    sigaction(2, &act, &oact);
    // 创建一个无限循环，不断执行
    while (true)
    {
        // 暂停进程执行1秒
        sleep(1);
    }
    // 程序正常结束
    return 0;
}


// void handler(int signo)
// {
//     sigset_t pending;
//     sigemptyset(&pending);
//     int n = sigpending(&pending);
//     assert(n == 0);
//     cout<<"递达中...:";
//     PrintSig(pending);
//     cout<<signo<<"2号信号被递达处理..."<<endl; 
// }

// void PrintSig(sigset_t &pending)
// {
//     // 打印当前进程的挂起信号位图
//     cout << "Pending bitmap:";
//     // 遍历信号编号，从31（最大的信号编号）到1
//     for (int signo = 31; signo > 0; signo--)
//     {
//         // 使用sigismember函数检查信号是否在挂起信号集中
//         if (sigismember(&pending, signo))
//         {
//             // 如果信号在挂起信号集中，打印1
//             cout << "1";
//         }
//         else
//         {
//             // 如果信号不在挂起信号集中，打印0
//             cout << "0";
//         }
//     }
//     // 换行
//     cout << endl;
// }

// int main()
// {
//     signal(2,handler);
//     // 定义两个信号集变量，用于屏蔽和挂起信号
//     sigset_t block, oblock;
//     // 初始化block信号集为空集
//     sigemptyset(&block);
//     // 将信号2添加到block信号集中
//     sigaddset(&block, 2);
//     // 遍历从1到31的所有信号，并将它们添加到block信号集中
//     for (int signo = 1; signo <= 31; signo++)
//     {
//         sigaddset(&block, signo);
//     }
//     // 使用sigprocmask函数设置block信号集为当前进程的信号屏蔽字
//     int n = sigprocmask(SIG_SETMASK, &block, &oblock);
//     // 断言n的值为0，如果不是0，则程序会退出
//     assert(n == 0);
//     // 打印设置信号屏蔽字成功的消息
//     cout << "block 2 signal success" << endl;
//     cout<<"pid:"<<getpid()<<endl;
//     int cnt=0;
//     // 创建一个无限循环，用于持续检查挂起信号
//     while (true)
//     {
//         // 初始化pending信号集为空集
//         sigset_t pending;
//         sigemptyset(&pending);
//         // 使用sigpending函数获取当前进程的挂起信号集
//         n = sigpending(&pending);
//         // 断言n的值为0，如果不是0，则程序会退出
//         assert(n == 0);
//         // 调用PrintSig函数打印挂起信号位图
//         PrintSig(pending);
//         cnt++;
//         if(cnt==20)
//         {
//             cout<<"解除对2号信号的屏蔽"<<endl;
//             n=sigprocmask(SIG_UNBLOCK,&block,&oblock);
//             assert(n==0);
//         }
//         // 暂停进程执行1秒
//         sleep(1);
//     }
// }

// void handler(int sig)
// {
//     cout<<"get a sig:"<<sig<<endl;
//     exit(1);
// }

// int main()
// {
//     signal(SIGFPE,handler);
//     int a=10;
//     a/=0;
//     while(true) sleep(1);
//     return 0;
// }
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