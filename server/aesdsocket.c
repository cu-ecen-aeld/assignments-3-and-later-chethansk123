#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <features.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9000
#define FILE_PATH "/var/tmp/aesdsocketdata"
#define BUFFER_SIZE 1024
int server_socket = -1;
int status = -1;


void handle_signal(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        syslog(LOG_INFO, "Caught signal, exiting");
        if (server_socket != -1) {
            close(server_socket);
        }
        unlink(FILE_PATH);
        closelog();
        exit(0);
    }
}

void setup_signal_handler() {
	
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    //sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}


int main()
{
struct sockaddr_in server_addr, client_addr;
socklen_t client_addr_len = sizeof(client_addr);
int client_socket;
char buffer[BUFFER_SIZE];
ssize_t bytes_received;
openlog("aesdsocket", LOG_PID, LOG_USER);

setup_signal_handler();

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        syslog(LOG_ERR, "socket creation failed: %s", strerror(errno));
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        syslog(LOG_ERR, "bind failed: %s", strerror(errno));
        close(server_socket);
        return -1;
    }

     if (listen(server_socket, 10) == -1) {
        syslog(LOG_ERR, "listen failed: %s", strerror(errno));
        close(server_socket);
        return -1;
    }
     while(1)
     {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            syslog(LOG_ERR, "accept failed: %s", strerror(errno));
            continue;
        }

	 syslog(LOG_INFO, "Accepted connection from %s", inet_ntoa(client_addr.sin_addr));
     
     

      int file_fd = open(FILE_PATH, O_RDWR | O_CREAT | O_APPEND, 0666);
        if (file_fd == -1) {
            syslog(LOG_ERR, "file open failed: %s", strerror(errno));
            close(client_socket);
            continue;
        }

	 while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
            buffer[bytes_received] = '\0';

            if (write(file_fd, buffer, bytes_received) == -1) {
                syslog(LOG_ERR, "file write failed: %s", strerror(errno));
                break;
            }

            if (strchr(buffer, '\n') != NULL) {
                lseek(file_fd, 0, SEEK_SET);
                while ((bytes_received = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
                    send(client_socket, buffer, bytes_received, 0);
                }
            }
        }

        close(file_fd);
        close(client_socket);
        syslog(LOG_INFO, "Closed connection from %s", inet_ntoa(client_addr.sin_addr));
}
 close(server_socket);
    closelog();
 return 0;

}
