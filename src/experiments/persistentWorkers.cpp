#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <semaphore>

int effort = 10;
int task_count = 100;
int worker_count = 20;

std::counting_semaphore<> completedWorkSemaphore(0);


class ThreadPool {
public:
    ThreadPool(size_t numThreads) : done(false), newWorkSemaphore(0) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this, i] {
                printf("\nstarting: %i", i);
                while (true) {
                    newWorkSemaphore.acquire();
                    if (done) {
                        printf("\ndone: %i", i);
                        break;
                    }
                    printf("\nworking: %i", i);
                    std::this_thread::sleep_for(std::chrono::milliseconds(effort));
                    completedWorkSemaphore.release();
                }
            });
        }
    }

    ~ThreadPool() {
        done = true;
        for (size_t i = 0; i < workers.size(); ++i) {
            newWorkSemaphore.release();
        }
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        printf("\n");
    }

    void enqueue() {
        newWorkSemaphore.release();
    }

private:
    std::vector<std::thread> workers;
    std::atomic<bool> done;
    std::counting_semaphore<> newWorkSemaphore;
};

int main() {
    ThreadPool pool(worker_count);
    std::this_thread::sleep_for(std::chrono::seconds(2));


    auto start = std::chrono::high_resolution_clock::now();

    printf("\nbatch 1 ----------------");
    int batch_size_1 = task_count / 2;
    for (int i = 0; i < batch_size_1; ++i) {
        pool.enqueue();
    }
    for (int i = 0; i < batch_size_1; ++i) {
        completedWorkSemaphore.acquire();
    }

    printf("\nbatch 2 ----------------");
    int batch_size_2 = task_count - batch_size_1;
    for (int i = 0; i < batch_size_2; ++i) {
        pool.enqueue();
    }
    for (int i = 0; i < batch_size_2; ++i) {
        completedWorkSemaphore.acquire();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("\n");
    printf("\neffort time:  %i milliseconds", effort * task_count);
    printf("\nactual time:  %i milliseconds", duration);
    printf("\neffort saved: %i%%", (int)(100.f - ((float)duration) / ((float)effort * task_count) * 100.f));
    printf("\n");

    return 0;
}