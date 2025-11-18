
#ifndef S3GF_THREADPOOL_H
#define S3GF_THREADPOOL_H
#include "Libs.h"
#include "Utils/Logger.h"

namespace S3GF {
    class ThreadPool {
    public:
        explicit ThreadPool(uint32_t max_waiting_threads, uint32_t nums = std::thread::hardware_concurrency())
            : _nums_of_threads(nums), _max_threads_count(max_waiting_threads), _running(true) {
            if (!nums) {
                Logger::log("ThreadPool: Argument error: No one thread in thread pool!", Logger::FATAL);
                throw std::invalid_argument("ThreadPool: Argument error: No one thread in thread pool!");
            }
            if (!_max_threads_count) {
                Logger::log("ThreadPool: Argument error: No one thread in waiting queue!", Logger::FATAL);
                throw std::invalid_argument("ThreadPool: Argument error: No one thread in waiting thread queue!");
            }
            while (nums--) {
                _thread_list.emplace_back([this]{
                    std::function<void()> this_task;
                    while (true) {
                        std::unique_lock<std::mutex> lock(_mutex);
                        _condition_var.wait(lock, [this] {
                            return !_waiting_queue.empty() || !_running;
                        });
                        if (!_running && _waiting_queue.empty()) return;
                        if (_waiting_queue.empty()) continue;
                        this_task = std::move(_waiting_queue.front());
                        _waiting_queue.pop();
                        _condition_var.notify_one();
                        _running_thread_count += 1;
                        lock.unlock();
                        try {
                            this_task();
                        } catch (const std::exception& e) {
                            Logger::log(std::format("ThreadPool: Task failed! "
                                                    "Exception: {}", e.what()), Logger::ERROR);
                        }
                        lock.lock();
                        _running_thread_count -= 1;
                        lock.unlock();
                    }
                });
            }
        }

        ~ThreadPool() {
            stopAll();
        }

        bool append(std::function<void()> function) {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition_var.wait(lock, [this] {
                return _waiting_queue.size() < _max_threads_count || !_running;
            });
            if (!_running) return false;
            _waiting_queue.emplace(std::move(function));
            _condition_var.notify_one();
            return true;
        }

        void startAll() {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _running = true;
            }
            _condition_var.notify_all();
        }

        void wait() {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition_var.wait(lock, [this] {
                return _waiting_queue.empty() || _running_thread_count == 0;
            });
        }

        void stopAll() {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                if (!_running) return;
                _running = false;
                _waiting_queue = {};
            }
            _condition_var.notify_all();
            for (auto& th : _thread_list) {
                if (th.joinable()) th.join();
            }
        }

        [[nodiscard]] bool isRunning() const { return _running; }
        [[nodiscard]] uint32_t threadsCount() const { return _nums_of_threads; }
        [[nodiscard]] uint32_t waitingQueueCount() {
            std::unique_lock<std::mutex> lock(_mutex);
            return _waiting_queue.size();
        }
        [[nodiscard]] uint32_t maxWaitingQueueCount() const { return _max_threads_count; }
        [[nodiscard]] uint32_t runningThreadsCount() const { return _running_thread_count; }

    private:
        std::vector<std::thread> _thread_list;
        uint32_t _nums_of_threads;
        uint32_t _max_threads_count;
        std::atomic<uint32_t> _running_thread_count{0};
        std::mutex _mutex;
        std::condition_variable _condition_var;
        std::queue<std::function<void()>> _waiting_queue;
        std::atomic<bool> _running;
    };
}

#endif //S3GF_THREADPOOL_H
