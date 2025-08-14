// Simple TCP client for Part 3
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char** argv) {
    const char* host = "127.0.0.1";
    int port = 5000;
    if (argc >= 2) host = argv[1];
    if (argc >= 3) port = std::stoi(argv[2]);

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
        char buf[8192];
        if (!fgets(buf, sizeof(buf), fp)) break;
        std::cout << buf;
    }
    fclose(fp);
    return 0;
}
