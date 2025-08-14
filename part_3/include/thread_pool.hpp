#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include "ts_queue.hpp"

class ThreadPool {
public:
    explicit ThreadPool(size_t n = std::thread::hardware_concurrency())
      : stop_(false)
    {
        if (n == 0) n = 2;
        workers_.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            workers_.emplace_back([this] {
                while (!stop_) {
                    auto job = jobs_.pop();
                    if (!job) break;      // queue stopped and empty
                    (*job)();
                }
            });
        }
    }

    ~ThreadPool() { shutdown(); }

    void submit(std::function<void()> fn) { jobs_.push(std::move(fn)); }

    void shutdown() {
        if (stop_.exchange(true)) return;
        jobs_.stop();
        for (auto& t : workers_) if (t.joinable()) t.join();
    }

private:
    TSQueue<std::function<void()>> jobs_;
    std::vector<std::thread> workers_;
    std::atomic<bool> stop_;
};
