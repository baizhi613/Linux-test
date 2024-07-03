#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
void writer(int wfd)
{
    const char* str="hello father,I am child";
    char buffer[128];
    int cnt=0;
    pid_t pid=getpid();
    while(1)
    {
        snprintf(buffer,sizeof(buffer),"message:%s,pid:%d,count:%d",str,pid,cnt);
        write(wfd,buffer,strlen(buffer));
        cnt++;
        sleep(1);
    }
}
void reader(int rfd)
{
    char buffer[1024];
    while(1)
    {
        ssize_t n=read(rfd,buffer,sizeof(buffer)-1);
        (void)n;
        printf("father get a message %s",buffer);
    }
}
int main()
{
    int pipefd[2];
    int n=pipe(pipefd);
    if(n<0) return 1;
    printf("pipefd[0]:%d,pipefd[1]:%d\n",pipefd[0],pipefd[1]);//3,4

    pid_t id=fork();
    if(id==0)
    {
        //child:r
        close(pipefd[0]);

        writer(pipefd[1]);

        exit(0);
    }
    //father:w
    close(pipefd[1]);

    reader(pipefd[0]);

    wait(NULL);
}