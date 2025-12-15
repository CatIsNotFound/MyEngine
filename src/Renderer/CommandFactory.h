#ifndef MYENGINE_COMMANDFACTORY_H
#define MYENGINE_COMMANDFACTORY_H
#include "CommandPool.h"

namespace MyEngine {
    namespace RenderCommand {
        template<typename T> class CommandPool;
        class CommandFactory {
        public:
            template<typename T, typename ...Args>
            static T* acquire(Args... args) {
                return getPool<T>().acquire(args...);
            }

            template<typename T>
            static void release(std::unique_ptr<T> command) {
                getPool<T>().release(std::move(command));
            }

            template<typename T>
            static void registerCommand() {
                getPool<T>();
            }

        private:
            template<typename T>
            static CommandPool<T>& getPool() {
                static CommandPool<T> cmd_pool;
                return cmd_pool;
            }
        };
    }
}

#endif //MYENGINE_COMMANDFACTORY_H