#pragma once
#include <atomic>
#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <semaphore>

int effort = 10;
int task_count = 100;
int worker_count = 5;

std::counting_semaphore<> completedWorkSemaphore(0);


class ThreadPool {
public:
    ThreadPool(size_t numThreads, bool simulateWorkload = false) : done(false), newWorkSemaphore(0) {
        if (simulateWorkload) {
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
    }

    void addWorker(std::function<void()> task) {
        workers.emplace_back(std::move(task));
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

    std::vector<std::thread> workers;
    std::atomic<bool> done;
    std::counting_semaphore<> newWorkSemaphore;
};
