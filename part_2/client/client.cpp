// client.cpp
// Simple TCP client: reads stdin lines, sends to server, prints the first response line.
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: client <host> <port>\n";
        return 1;
    }
    const char* host = argv[1];
    int port = std::stoi(argv[2]);

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) { perror("socket"); return 1; }

    sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
        std::cerr << "bad host\n"; return 1;
    }
    if (connect(fd, (sockaddr*)&addr, sizeof(addr)) != 0) {
        perror("connect"); return 1;
    }

    FILE* fp = fdopen(fd, "r+");
    if (!fp) { close(fd); return 1; }

    std::string line;
    while (std::getline(std::cin, line)) {
        fprintf(fp, "%s\n", line.c_str());
        fflush(fp);

        // Read one response line (sufficient for the commands we support)
        char buf[8192];
        if (!fgets(buf, sizeof(buf), fp)) break;
        std::cout << buf;
    }
    fclose(fp);
    return 0;
}
