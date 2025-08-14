// Thread-pool TCP server for Part 3
#include <iostream>
#include <thread>
#include <string>
#include <sstream>
#include <mutex>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "graph.hpp"
#include "thread_pool.hpp"

static Graph G;
static std::mutex G_MTX;

static void trim_newlines(std::string& s) {
    while (!s.empty() && (s.back()=='\n' || s.back()=='\r')) s.pop_back();
}

static void print_unknown(FILE* fp) {
    fprintf(fp,
        "ERR unknown cmd\n"
        "Use: ADD_NODE <id> | ADD_EDGE <u> <v> [w] | BFS <src> | SHORTEST_PATH <src> <dst> | QUIT | HELP\n"
    );
    fflush(fp);
}

static void handle_client(int client_fd) {
    FILE* fp = fdopen(client_fd, "r+");
    if (!fp) { close(client_fd); return; }
    char* line = nullptr; size_t n = 0;

    while (true) {
        ssize_t r = getline(&line, &n, fp);
        if (r == -1) break;

        std::string cmd(line);
        trim_newlines(cmd);
        if (cmd.empty()) { fprintf(fp, "ERR empty\n"); fflush(fp); continue; }
        if (cmd == "QUIT") { fprintf(fp, "OK Bye\n"); fflush(fp); break; }
        if (cmd == "HELP") { print_unknown(fp); continue; }

        std::istringstream iss(cmd);
        std::string op; iss >> op;

        if (op == "ADD_NODE") {
            int id; if (!(iss >> id)) { fprintf(fp, "ERR bad args\n"); fflush(fp); continue; }
            { std::lock_guard<std::mutex> lk(G_MTX); G.addNode(id); }
            fprintf(fp, "OK\n"); fflush(fp);

        } else if (op == "ADD_EDGE") {
            int u,v,w=1; if (!(iss >> u >> v)) { fprintf(fp, "ERR bad args\n"); fflush(fp); continue; }
            if (!(iss >> w)) w = 1;
            bool ok;
            { std::lock_guard<std::mutex> lk(G_MTX); ok = G.addEdge(u,v,w); }
            if (!ok) fprintf(fp, "ERR no such node\n"); else fprintf(fp, "OK\n");
            fflush(fp);

        } else if (op == "BFS") {
            int s; if (!(iss >> s)) { fprintf(fp, "ERR bad args\n"); fflush(fp); continue; }
            std::vector<int> order;
            { std::lock_guard<std::mutex> lk(G_MTX); order = G.bfs(s); }
            if (order.empty()) { fprintf(fp, "EMPTY\n"); fflush(fp); continue; }
            for (size_t i=0; i<order.size(); ++i) fprintf(fp, "%d%c", order[i], (i+1==order.size()?'\n':' '));
            fflush(fp);

        } else if (op == "SHORTEST_PATH") {
            int s,d; if (!(iss >> s >> d)) { fprintf(fp, "ERR bad args\n"); fflush(fp); continue; }
            auto ans = [&](){ std::lock_guard<std::mutex> lk(G_MTX); return G.shortestPathUnweighted(s,d);}();
            if (!ans) fprintf(fp, "UNREACHABLE\n"); else fprintf(fp, "%d\n", *ans);
            fflush(fp);

        } else {
            print_unknown(fp);
        }
    }

    if (line) free(line);
    fclose(fp); // closes client_fd
}

int main(int argc, char** argv) {
    int port = 5000;
    if (argc >= 2) port = std::stoi(argv[1]);

    int srv = socket(AF_INET, SOCK_STREAM, 0);
    if (srv < 0) { perror("socket"); return 1; }
    int opt=1; setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_addr.s_addr = INADDR_ANY; addr.sin_port = htons(port);
    if (bind(srv, (sockaddr*)&addr, sizeof(addr)) != 0) { perror("bind"); return 1; }
    if (listen(srv, 64) != 0) { perror("listen"); return 1; }

    std::cout << "Part3 server listening on " << port << std::endl;

    ThreadPool pool(4); // 4 workers; change if needed

    while (true) {
        int cfd = accept(srv, nullptr, nullptr);
        if (cfd < 0) { perror("accept"); continue; }
        pool.submit([cfd]{ handle_client(cfd); });
    }
}
