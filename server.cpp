#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>

#define PI 3.14159265

struct Params {
    int request_number { 0 };
    int client_fd { -1 };
};

void *thread_job(void *arg) {
    auto p = (Params *) arg;
    int request_number = p->request_number;
    int client_fd = p->client_fd;

    std::cout << "Thread started for request: " << request_number << std::endl;
    std::string response = "Request number " + std::to_string(request_number) + " has been processed";

    int err = write(client_fd, response.c_str(), response.length());
    if (err < 0) {
        std::cerr << "ERROR writing to socket: " << std::strerror(err) << std::endl;
    }

    double result;
    for (unsigned int i = 0; i < 100'000'000; i++) {
        result = sin(30 * PI / 180) * pow(cos(60 * PI / 180), 2);
    }

    shutdown(client_fd, SHUT_WR);
    close(client_fd);

    return 0;
}

int main() {
    int err, sock;

    sockaddr_in server, client;
    socklen_t sin_len = sizeof(client);

    // Server setup
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Can't open sock" << std::endl;
        exit(-1);
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (int *) 1, sizeof(int));

    int port = 8080;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    err = bind(sock, (sockaddr *) &server, sizeof(server));
    if (err != 0) {
        std::cerr << "Can't bind" << std::strerror(err) << std::endl;
        close(sock);
        exit(-1);
    }

    // Threads setup
    pthread_t thread;
    pthread_attr_t thread_attr;
    err = pthread_attr_init(&thread_attr);
    if (err != 0) {
        std::cerr << "Cannot create thread attribute: " << std::strerror(err) << std::endl;
        exit(-1);
    }

    err = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (err != 0) {
        std::cerr << "Setting detach state attribute failed: " << std::strerror(err) << std::endl;
        exit(-1);
    }

    listen(sock, 10);
    Params thread_params;

    while (true) {
        thread_params.client_fd = accept(sock, (sockaddr *) &client, &sin_len);
        std::cout << "Connection accepted" << std::endl;
        if (thread_params.client_fd == -1) {
            std::cerr << "Can't accept" << std::endl;
            exit(-1);
        }

        thread_params.request_number++;
        err = pthread_create(&thread, &thread_attr, thread_job, (void *) &thread_params);
        if (err != 0) {
            std::cout << "Cannot create a thread: " << std::strerror(err) << std::endl;
            exit(-1);
        }
    }
}
