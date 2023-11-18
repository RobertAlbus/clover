#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <semaphore>

int effort_ms = 100;
int task_count = 100;


class ThreadPool {
public:
    ThreadPool(size_t numThreads) : done(false), semaphore(0) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this, i] {
                printf("\nstarting: %i", i);
                while (true) {
                    semaphore.acquire();
                    if (done) {
                        printf("\ndone: %i", i);
                        break;
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(effort_ms));
                    
                    printf("\nworking: %i", i);
                }
            });
        }
    }

    ~ThreadPool() {
        done = true;
        for (size_t i = 0; i < workers.size(); ++i) {
            semaphore.release();
        }
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void enqueue() {
        semaphore.release();
    }

private:
    std::vector<std::thread> workers;
    std::atomic<bool> done;
    std::counting_semaphore<> semaphore;
};

int main() {
    ThreadPool pool(4);

    for (int i = 0; i < task_count; ++i) {
        pool.enqueue();
    }

    std::this_thread::sleep_for(std::chrono::seconds(4));


    return 0;
}