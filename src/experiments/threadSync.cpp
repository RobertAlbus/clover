#include <iostream>
#include <thread>
#include <cstdio>

#include <semaphore>
#include <latch>
#include <barrier>
#include <chrono>

int worker_count_layer_1 = 3;
int worker_count_layer_2 = 2;
int worker_count_layer_3 = 1;

int worker_count_total_layer_3 = 
        worker_count_layer_1 +
        worker_count_layer_2 +
        worker_count_layer_3;

int worker_count_total_layer_2 = 
        worker_count_layer_1 +
        worker_count_layer_2;

int last_worker_index = worker_count_total_layer_3 - 1;



int numIterations = 10000; 


template<typename T>
struct BaseThreadWorker {
    virtual void work(T& mechanism) = 0;
};

//----------------
// Semaphore
struct SemaphoreThreadWorker : public BaseThreadWorker<std::counting_semaphore<INT32_MAX>> {
    void work(std::counting_semaphore<INT32_MAX>& mechanism) override {
        mechanism.release();
    }
};

void semaphoreMain() {
    std::vector<SemaphoreThreadWorker> workers(worker_count_total_layer_3);
    std::counting_semaphore semaphore(0);
    std::thread threads[worker_count_total_layer_3];

    for (int i = 0; i < worker_count_layer_1; ++i) {
        auto task = [&workers, &semaphore, i]() {
            workers[i].work(semaphore);
        };
        threads[i] = std::thread(task);
    }

    for (int i = 0; i < worker_count_layer_1; ++i) {
        semaphore.acquire();
    }

    for (int i = worker_count_layer_1; i < worker_count_total_layer_2; ++i) {
        auto task = [&workers, &semaphore, i]() {
            workers[i].work(semaphore);
        };
        threads[i] = std::thread(task);
    }

    for (int i = 0; i < worker_count_layer_2; ++i) {
        semaphore.acquire();
    }

    auto task = [&workers, &semaphore]() {
        workers[last_worker_index].work(semaphore);
    };
    threads[last_worker_index] = std::thread(task);
    semaphore.acquire();

    for (int i = 0; i < worker_count_total_layer_3; ++i) {
        threads[i].join();
    }
}

//----------------
// Latch
struct LatchThreadWorker : public BaseThreadWorker<std::latch> {
    void work(std::latch& mechanism) override {
        mechanism.count_down();
    }
};

void latchMain() {
    std::vector<LatchThreadWorker> workers(worker_count_total_layer_3);
    std::thread threads[worker_count_total_layer_3];
    std::latch latch1(worker_count_layer_1);
    std::latch latch2(worker_count_layer_2);
    std::latch latch3(worker_count_layer_3);

    for (int i = 0; i < worker_count_layer_1; ++i) {
        auto task = [&workers, &latch1, i]() {
            workers[i].work(latch1);
        };
        threads[i] = std::thread(task);
    }

    latch1.wait();

    for (int i = worker_count_layer_1; i < worker_count_total_layer_2; ++i) {
        auto task = [&workers, &latch2, i]() {
            workers[i].work(latch2);
        };
        threads[i] = std::thread(task);
    }

    latch2.wait();

    auto task = [&workers, &latch3]() {
        workers[last_worker_index].work(latch3);
    };
    threads[last_worker_index] = std::thread(task);

    latch3.wait();

    for (int i = 0; i < worker_count_total_layer_3; ++i) {
        threads[i].join();
    }
}

//----------------
// Barrier

struct BarrierThreadWorker : public BaseThreadWorker<std::barrier<>> {
    void work(std::barrier<>& mechanism) override {
        mechanism.arrive_and_wait();
    }
};

void barrierMain() {
    std::vector<BarrierThreadWorker> workers(worker_count_total_layer_3);
    std::thread threads[worker_count_total_layer_3];
    std::barrier<> barrier1(worker_count_layer_1 + 1);
    std::barrier<> barrier2(worker_count_layer_2 + 1);
    std::barrier<> barrier3(worker_count_layer_3 + 1);

    for (int i = 0; i < worker_count_layer_1; ++i) {
        auto task = [&workers, &barrier1, i]() {
            workers[i].work(barrier1);
        };
        threads[i] = std::thread(task);
    }

    barrier1.arrive_and_wait();

    for (int i = worker_count_layer_1; i < worker_count_total_layer_2; ++i) {
        auto task = [&workers, &barrier2, i]() {
            workers[i].work(barrier2);
        };
        threads[i] = std::thread(task);
    }

    barrier2.arrive_and_wait();

    auto task = [&workers, &barrier3]() {
        workers[last_worker_index].work(barrier3);
    };
    threads[last_worker_index] = std::thread(task);

    barrier3.arrive_and_wait();

    for (int i = 0; i < worker_count_total_layer_3; ++i) {
        threads[i].join();
    }
}



int main() {
    std::this_thread::sleep_for(std::chrono::microseconds(2000));

    int64_t semaphoreDuration = 0;
    int64_t latchDuration = 0;
    int64_t barrierDuration = 0;
    int64_t iterationDuration = 0;

    auto completeStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numIterations; i++) {
        auto iterationStart = std::chrono::high_resolution_clock::now();

        auto semaphoreStart = std::chrono::high_resolution_clock::now();
        semaphoreMain();
        auto semaphoreEnd = std::chrono::high_resolution_clock::now();
        semaphoreDuration += std::chrono::duration_cast<std::chrono::microseconds>(semaphoreEnd - semaphoreStart).count();
        
        auto latchStart = std::chrono::high_resolution_clock::now();
        latchMain();
        auto latchEnd = std::chrono::high_resolution_clock::now();
        latchDuration += std::chrono::duration_cast<std::chrono::microseconds>(latchEnd - latchStart).count();

        auto barrierStart = std::chrono::high_resolution_clock::now();
        barrierMain();
        auto barrierEnd = std::chrono::high_resolution_clock::now();
        barrierDuration += std::chrono::duration_cast<std::chrono::microseconds>(barrierEnd - barrierStart).count();
        
        auto iterationEnd = std::chrono::high_resolution_clock::now();
        iterationDuration += std::chrono::duration_cast<std::chrono::microseconds>(iterationEnd - iterationStart).count();
    }

    auto completeEnd = std::chrono::high_resolution_clock::now();
    auto completeDuration = std::chrono::duration_cast<std::chrono::microseconds>(completeEnd - completeStart).count();

    printf("\n\n");
    printf("\ntotal:     %9i microseconds", static_cast<int>(completeDuration));
    printf("\niteration: %9i microseconds", static_cast<int>(iterationDuration));
    printf("\niteration: %9i microseconds average", static_cast<int>(iterationDuration / numIterations));
    printf("\n");
    printf("\nsemaphore: %9i microseconds average", static_cast<int>(semaphoreDuration / numIterations));
    printf("\nlatch:     %9i microseconds average", static_cast<int>(latchDuration / numIterations));
    printf("\nbarrier:   %9i microseconds average", static_cast<int>(barrierDuration / numIterations));
    printf("\n");
    printf("\nsemaphore: %9i microseconds", static_cast<int>(semaphoreDuration));
    printf("\nlatch:     %9i microseconds", static_cast<int>(latchDuration));
    printf("\nbarrier:   %9i microseconds", static_cast<int>(barrierDuration));
    printf("\n");
}