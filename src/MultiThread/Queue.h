#pragma once
#ifndef QUEUE_H
#define QUEUE_H
#include "Libs.h"

namespace S3GF {
    template <typename T>
    class TaskQueue {
    public:
        TaskQueue(TaskQueue &&) = delete;
        TaskQueue(const TaskQueue &) = delete;
        TaskQueue &operator=(TaskQueue &&) = delete;
        TaskQueue &operator=(const TaskQueue &) = delete;
        
        explicit TaskQueue() = default;
        ~TaskQueue() = default;

        bool push(const T& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _cond_var.wait(lock, [this] {
                return _datas_queue.size() <= _max_size || !_running; 
            });
            if (!_running) return false;
            _datas_queue.push_back(data);
            _cond_var.notify_one();
            return true;
        }

        bool push(T&& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _cond_var.wait(lock, [this] {
                return _datas_queue.size() <= _max_size || !_running; 
            });
            if (!_running) return false;
            _datas_queue.push_back(data);
            _cond_var.notify_one();
            return true;
        }

        bool pop(T& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _cond_var.wait(lock, [this] {
                return !_datas_queue.empty() || !_running;
            });
            if (!_running) return false;
            data = _datas_queue.front();
            _datas_queue.pop_front();
            _cond_var.notify_one();
            return true;
        }

        void clear() {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_deletor) {
                std::for_each(_datas_queue.begin(), _datas_queue.end(), [this](T& data){
                    _deletor(data);
                });
            }
            _datas_queue.clear();
        }

        void start() {
            _running = true;
        }

        void stop() {
            _running = false;
            _cond_var.notify_all();
        }

        [[nodiscard]] bool isRunning() const {
            return _running;
        }

        void setMaxSize(size_t max_size) {
            _max_size = max_size;
        }

        [[nodiscard]] size_t size() const {
            return _datas_queue.size();
        }

        [[nodiscard]] bool empty() const {
            return _datas_queue.empty();
        }

        const T& front() const {
            return _datas_queue.front();
        }

        const T& back() const {
            return _datas_queue.back();
        }
        
        void setDeletor(const std::function<void(T&)>& deletor) {
            _deletor = deletor;
        }
        
    private:
        std::deque<T> _datas_queue;
        std::mutex _mutex;
        std::condition_variable _cond_var;
        std::atomic<bool> _running{false};
        size_t _max_size{50};
        std::function<void(T&)> _deletor;
    };

    class AsyncTaskQueue {
    public:
        enum class Priority {
            Low, Medium, High
        };

        struct Task {
            uint64_t id;
            Priority priority;
            std::function<void()> function;
        };

        struct TaskComp {
            bool operator()(const Task& t1, const Task& t2) {
                if (t1.priority < t2.priority) return true;
                return t1.id > t2.id;
            }
        };

        explicit AsyncTaskQueue(uint16_t max_size = 128)
            : _max_size(max_size), _thread_pool(max_size), _running(false) {
        }

        ~AsyncTaskQueue() {
            if (_running) waitForAllTasks();
        }

        void append(std::function<void()> function, Priority priority = Priority::Low) {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                Task task = {_new_task_id++, priority, std::move(function)};
                _task_queue.emplace(task);
            }
            _con_var.notify_one();
        }

        void startAll() {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _running = true;
            }
            _con_var.notify_all();
            _thread_pool.startAll();
            _run_thread = std::thread(&AsyncTaskQueue::running, this);
        }

        void waitForAllTasks() {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _running = false;
            }
            _thread_pool.wait();
            _con_var.notify_all();
            if (_run_thread.joinable()) _run_thread.join();
        }

        bool isRunning() const { return _running; }
        size_t queueLength() const { return _task_queue.size(); }

    private:
        void running() {
            while (true) {
                std::unique_lock<std::mutex> lock(_mutex);
                _con_var.wait(lock, [this] { return !_task_queue.empty() || !_running; });
                if (!_running || _task_queue.empty()) {
                    _running = false;
                    break;
                };
                _thread_pool.append(_task_queue.top().function);
                _task_queue.pop();
            }
        }
        uint16_t _max_size;
        std::priority_queue<Task, std::deque<Task>, TaskComp> _task_queue;
        ThreadPool _thread_pool;
        std::mutex _mutex;
        std::condition_variable _con_var;
        std::atomic<bool> _running;
        std::atomic<uint64_t> _new_task_id{1};
        std::thread _run_thread;
    };
}

#endif // !QUEUE_H