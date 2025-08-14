#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class TSQueue {
public:
    void push(T v) {
        {
            std::lock_guard<std::mutex> lk(m_);
            q_.push(std::move(v));
        }
        cv_.notify_one();
    }

    // Blocks until item is available or stop() is called.
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [&]{ return stop_ || !q_.empty(); });
        if (stop_ && q_.empty()) return std::nullopt;
        T v = std::move(q_.front()); q_.pop();
        return v;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lk(m_);
            stop_ = true;
        }
        cv_.notify_all();
    }

private:
    std::queue<T> q_;
    std::mutex m_;
    std::condition_variable cv_;
    bool stop_ = false;
};
