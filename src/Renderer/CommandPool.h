#ifndef MYENGINE_COMMANDPOOL_H
#define MYENGINE_COMMANDPOOL_H
#include "BaseCommand.h"

namespace MyEngine {
    namespace RenderCommand {
        class AbstractCommandPool {
        public:
            explicit AbstractCommandPool() = default;
            virtual ~AbstractCommandPool() = 0;
        };

        template<typename T>
        class CommandPool : public AbstractCommandPool {
        public:
            explicit CommandPool(uint32_t max_commands = 1024) : _cmd_max_count(max_commands) {
                _command_deque.resize(max_commands / 4);
            }
            
            ~CommandPool() override = default;

            template <typename ...Args>
            T *acquire(Args ...args) {
                std::unique_lock<std::mutex> _lock(_mutex);
                if (_command_deque.empty()) {
                    return new T(args...);
                }
                T *ptr = _command_deque.front().release();
                if (ptr) {
                    /// Call reset(...) function.
                    ptr->reset(args...);
                } else {
                    ptr = new T(args...);
                }
                _command_deque.pop_front();
                return ptr;
            }

            void release(std::unique_ptr<T> command) {
                std::unique_lock<std::mutex> _lock(_mutex);
                if (_command_deque.size() + 1 >= _cmd_max_count) {
                    /// Remove from the front of the queue.
                    _command_deque.erase(_command_deque.begin());
                }
                _command_deque.push_back(std::move(command));
            }

        private:
            std::deque<std::unique_ptr<T>> _command_deque;
            std::mutex _mutex;
            uint32_t _cmd_max_count;
        };
    }
}

#endif //MYENGINE_COMMANDPOOL_H
