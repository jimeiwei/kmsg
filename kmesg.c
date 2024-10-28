#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define MAX_EVENTS 10
#define BUFFER_SIZE 88

int main() {
    // 创建 epoll 实例
    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    // 打开 /dev/kmsg 设备文件
    int kmsg_fd = open("/dev/kmsg", O_RDONLY);
    if (kmsg_fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 添加 /dev/kmsg 到 epoll 监视列表
    struct epoll_event event;
    event.events = EPOLLIN;  // 监视可读事件
    event.data.fd = kmsg_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, kmsg_fd, &event) < 0) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    // 主事件循环
    while (1) {
        struct epoll_event events[MAX_EVENTS];
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);  // 阻塞等待事件

        for (int i = 0; i < n; i++) {
            if (events[i].events & EPOLLIN) {
                char log_buffer[BUFFER_SIZE];
                int bytes = read(kmsg_fd, log_buffer, sizeof(log_buffer) - 1);
                printf("Log: %d\n", bytes);  // 打印日志

                if (bytes > 0) {
                    log_buffer[bytes] = '\0';  // 添加字符串结束符
                    printf("Log: %s", log_buffer);  // 打印日志
                } else {
                    perror("read");
                }
            }
        }
    }

    // 清理资源
    close(kmsg_fd);
    close(epoll_fd);
    return 0;
}
