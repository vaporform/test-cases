#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <chrono>
#include <iomanip> // STD fixed

#define SOCKET_PATH "/tmp/my_local_socket"

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[512];

    // Create socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    unlink(SOCKET_PATH);

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen");
        return 1;
    }

    std::cout << "Waiting for a connection...\n";

    client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd == -1) {
        perror("accept");
        return 1;
    }

    ssize_t bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    if (bytes > 0) {
        buffer[bytes] = '\0';

        // Parse timestamp
        std::string msg(buffer);
        auto receive_time_chrono = std::chrono::high_resolution_clock::now();
	double receive_time = std::chrono::duration<double>(receive_time_chrono.time_since_epoch()).count();

	std::cout << std::fixed << std::setprecision(10);
        std::cout << "Sent time: " << msg << std::endl;
        std::cout << "Elapsed time: " << receive_time << std::endl;
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}

