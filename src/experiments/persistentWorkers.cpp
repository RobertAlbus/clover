#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <semaphore>

#include "ThreadPool.h"

int main() {
    bool simulate_workload = true;
    ThreadPool pool(worker_count, simulate_workload);
    pool.addWorker([&pool] {
                    printf("\nstarting: custom");
                    while (true) {
                        pool.newWorkSemaphore.acquire();
                        if (pool.done) {
                            printf("\ndone: custom");
                            break;
                        }
                        printf("\nworking: custom");
                        std::this_thread::sleep_for(std::chrono::milliseconds(effort));
                        completedWorkSemaphore.release();
                    }
                });

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