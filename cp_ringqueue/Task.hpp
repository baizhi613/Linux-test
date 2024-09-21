#pragma once

#include <iostream>
#include <functional>

using Task = std::function<void()>;


void Download()
{
    std::cout << "this is a download task" << std::endl;
}