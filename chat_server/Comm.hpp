#pragma once
#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int OpenDev(const std::string &dev, int flags)
{
    int fd = open(dev.c_str(), flags);
    if (fd < 0)
    {
        std::cerr << "open error" << std::endl;
        return -1;
    }
    return fd;
    //dup2(fd, 1);
}
