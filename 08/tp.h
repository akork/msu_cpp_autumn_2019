#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <vector>

struct ThreadPool {
    using Task = std::function<void()>;

    int working;
    std::condition_variable condition;
    std::vector<std::thread> threads;
    std::queue<Task> tasks;
    std::mutex mutex;

    ThreadPool(size_t poolSize)
    {

        working = true;
        for (size_t i = 0; i < poolSize; ++i) {
            threads.emplace_back([this]() {
                while (true) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> lock(mutex);
                        condition.wait(lock, [this] {
                            return !working || !tasks.empty();
                        });
                        if (working == false && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }
	
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>
    {
        using ReturnType = decltype(func(args...));

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            [func, args...]() { return func(args...); });
        {
            std::lock_guard<std::mutex> lock(mutex);
            tasks.emplace([task]() { (*task)(); });
        }

        condition.notify_one();
        return task->get_future();
    }

    ~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            working = false;
        }
        condition.notify_all();

        for (auto &thread : threads) {
            thread.join();
        }
    }
};
