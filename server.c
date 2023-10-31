#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdbool.h>

#define PORT 8080

void *handle_client(void *arg) {
    int client_fd = *((int *)arg);
}

int main(int argc, char *argv[]) {
    int server_fd;
    
    // arpa/inet.h -> server_addr;
    struct sockaddr_in server_addr;

    // create server socket
    // socket(domain, communication_type, protocol)
    // AF_INET: uses IPv4, AF_INET6 uses IPv6
    // SOCK_STREAM uses TCP, reliable and connection oriented
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed!");

        // stdlib.h -> EXIT_FAILURE
        exit(EXIT_FAILURE);
    }

    // config socket
    server_addr.sin_family = AF_INET;

    // INADDR_ANY: the server accepts connections from any network interface
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // bind socket to port 8080
    // the socket will listen to any clients trying to connect to port 8080
    if (bind(server_fd,
            (struct sockaddr *)&server_addr,
            sizeof(server_addr)) < 0) {
        perror("bind failed!");
        exit(EXIT_FAILURE);
    }

    // listen for connections
    // set maximum connections to 10
    // returns 0, otherwise -1 upon failure
    if (listen(server_fd, 10) < 0) {
        perror("listener failed!");
        exit(EXIT_FAILURE);
    }

    printf("\n\n[c_server]: Server listening on port %d\n\n", PORT);
    while(1) {
        // client info
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));

        // accept client connection
        if ((*client_fd = accept(server_fd,
                                (struct sockaddr *)&client_addr,
                                &client_addr_len)) < 0) {
            perror("accept failed!");
            continue;
        }

        // create a new thread to handle client requests
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
        pthread_detach(thread_id);
    }
}