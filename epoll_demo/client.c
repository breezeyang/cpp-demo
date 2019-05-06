

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SER_PORT (8788)
#define IPADDRESS ("127.0.0.1")
#define EPOLLEVENTS (100)
#define MAX_SIZE (1024)
#define FDSIZE (1000)


//########################fun desc

static void handle_connection(int sockfd);

static void handle_events(int epollfd, struct epoll_event* events, int num, int sockfd, char *buf);

static void handle_read(int epollfd, int fd, int sockfd, char* buf);

static void handle_write(int epollfd, int fd, int sockfd, char* buf);

static void add_event(int epollfd, int fd, int state);

static void delete_event(int epollfd, int fd, int state);

static void modify_event(int epollfd, int fd, int state);

//########################fun impl
//
static void handle_connection(int sockfd)
{
    int epollfd;
    struct epoll_event events[EPOLLEVENTS];
    char buf[MAX_SIZE];
    int ret;
    epollfd = epoll_create(FDSIZE);
    add_event(epollfd, STDIN_FILENO, EPOLLIN);
    for(;;) 
    {
        ret = epoll_wait(epollfd, events, EPOLLEVENTS, -1);
        handle_events(epollfd, events, ret, sockfd, buf);
    }
    close(epollfd);
}
static void handle_events(int epollfd, struct epoll_event* events, int num, int sockfd, char *buf)
{
    int fd;
    int i;
    for(i=0; i<num; i++) 
    {
        fd = events[i].data.fd;
        if (events[i].events & EPOLLIN)
        {
            handle_read(epollfd, fd, sockfd, buf);
        } else if (events[i].events & EPOLLOUT) {
            handle_write(epollfd, fd, sockfd, buf);
        }
    }
}

static void handle_read(int epollfd, int fd, int sockfd, char* buf) 
{
    int nread;
    nread = read(fd, buf, MAX_SIZE);
    if (nread == -1)
    {
        perror("read error");
        close(fd);
    } else if (nread == 0) 
    {
        fprintf(stderr, "server close.\n");
        close(fd);
    } else {
        if (fd == STDIN_FILENO) {
            add_event(epollfd, sockfd, EPOLLOUT);
        } else {
            delete_event(epollfd, sockfd, EPOLLIN);
            add_event(epollfd, STDOUT_FILENO, EPOLLOUT);
        }
    }
}
static void handle_write(int epollfd, int fd, int sockfd, char* buf) 
{
    int nwrite;
    nwrite = write(fd, buf, strlen(buf));
    if ( nwrite == -1) 
    {
        perror("write error!");
        close(fd);
    } else {
        if (fd == STDOUT_FILENO) {
            delete_event(epollfd, fd, EPOLLOUT);
        } else {
            modify_event(epollfd, fd, EPOLLIN);
        }
    }
    memset(buf, 0, MAX_SIZE);
}

static void add_event(int epollfd, int fd, int state) 
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

static void delete_event(int epollfd, int fd, int state) 
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

static void modify_event(int epollfd, int fd, int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

int main(int argc, char* argv[]) 
{
    fprintf(stderr, "main....\n");
    int sockfd = -1;
    struct sockaddr_in serv_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SER_PORT);
    inet_pton(AF_INET, IPADDRESS, &serv_addr.sin_addr);
    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    // 处理连接
    handle_connection(sockfd);
    close(sockfd);
    return 0;
}
