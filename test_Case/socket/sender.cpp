#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <chrono>

#define SOCKET_PATH "/tmp/my_local_socket"

int main() {
    int sock_fd;
    struct sockaddr_un addr;

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        return 1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        return 1;
    }

    // Create timestamp and message
    auto current_time_chrono = std::chrono::high_resolution_clock::now();
    double current_time_double = std::chrono::duration<double>(current_time_chrono.time_since_epoch()).count();

    // Convert the double time to a string
    std::string current_time_str = std::to_string(current_time_double);

    std::string message = current_time_str;

    write(sock_fd, message.c_str(), message.size());
    std::cout << "Message sent.\n";

    close(sock_fd);
    return 0;
}
