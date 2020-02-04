#include "main.h"

int main()
{
	ReadBuffer recv_buffer;
	recv_buffer.init(1024);
    printf("hello from LinuxEpollServer!\n");
    return 0;
}