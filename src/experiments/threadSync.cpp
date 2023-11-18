#include <iostream>
#include <thread>
#include <cstdio>

#include <semaphore>
#include <latch>
#include <barrier>
#include <chrono>


int timeMultiplier = 1;
int baseEffort = 1;

int numIterations = 100000; 


void logStart(int id) {
    // printf("\nThread %d starting", id);
}

void logComplete(int id) {
    // printf("\nThread %d completing", id);
}

//----------------
// Semaphore

void semaphoreWorker(std::counting_semaphore<INT32_MAX> &sem1, std::counting_semaphore<INT32_MAX> &sem2, int id, int effort) {
    // logStart(id);
    // std::this_thread::sleep_for(std::chrono::microseconds(std::max(0, effort)));
    // logComplete(id);
    if (id <= 3) sem1.release();
    else if (id <= 5) sem2.release();
}

void semaphoreMain() {
    std::counting_semaphore sem1(0); // threads 1, 2, 3
    std::counting_semaphore sem2(0); // threads 4, 5
    std::thread threads[6];

    // Start threads 1, 2, 3
    for (int i = 0; i < 3; ++i) {
        int id = i + 1;
        int effort = baseEffort - i * timeMultiplier;
        threads[i] = std::thread(semaphoreWorker, std::ref(sem1), std::ref(sem2), id, effort);
    }

    // Wait for threads 1, 2, 3 to complete
    for (int i = 0; i < 3; ++i) {
        sem1.acquire();
    }

    // Start threads 4, 5
    for (int i = 3; i < 5; ++i) {
        int id = i + 1;
        int effort = baseEffort - i * timeMultiplier;
        threads[i] = std::thread(semaphoreWorker, std::ref(sem1), std::ref(sem2), id, effort);
    }

    // Wait for threads 4, 5 to complete
    for (int i = 0; i < 2; ++i) {
        sem2.acquire();
    }

    // Start and wait for thread 6
    threads[5] = std::thread(semaphoreWorker, std::ref(sem1), std::ref(sem2), 6, 4 * timeMultiplier);
    threads[5].join();

    // Join threads 1-5
    for (int i = 0; i < 5; ++i) {
        threads[i].join();
    }
}

//----------------
// Latch

void latchWorker(std::latch &latch1, std::latch &latch2, int id, int effort) {
    // logStart(id);
    // std::this_thread::sleep_for(std::chrono::microseconds(std::max(0, effort)));
    // logComplete(id);
    if (id <= 3) latch1.count_down();
    else if (id <= 5) latch2.count_down();
}

void latchMain() {
    std::latch latch1(3); // threads 1, 2, 3
    std::latch latch2(2); // threads 4, 5
    std::thread threads[6];

    // Start threads 1, 2, 3
    for (int i = 0; i < 3; ++i) {
        int id = i + 1;
        int effort = baseEffort - i * timeMultiplier;
        threads[i] = std::thread(latchWorker, std::ref(latch1), std::ref(latch2), id, effort);
    }

    // Wait for threads 1, 2, 3 to complete
    latch1.wait();

    // Start threads 4, 5
    for (int i = 3; i < 5; ++i) {
        int id = i + 1;
        int effort = baseEffort - i * timeMultiplier;
        threads[i] = std::thread(latchWorker, std::ref(latch1), std::ref(latch2), id, effort);
    }

    // Wait for threads 4, 5 to complete
    latch2.wait();

    // Start and wait for thread 6
    threads[5] = std::thread(latchWorker, std::ref(latch1), std::ref(latch2), 6, 4 * timeMultiplier);
    threads[5].join();

    // Join threads 1-5
    for (int i = 0; i < 5; ++i) {
        threads[i].join();
    }
}

//----------------
// Barrier


void barrierWorker(std::barrier<> &barrier1, std::barrier<> &barrier2, int id, int effort) {
    // logStart(id);
    // std::this_thread::sleep_for(std::chrono::microseconds(std::max(0, effort)));
    // logComplete(id);
    if (id <= 3) {
        barrier1.arrive_and_wait();
    } else if (id <= 5) {
        barrier2.arrive_and_wait();
    }
}

std::barrier<> barrier1(4); // threads 1, 2, 3, and the main thread
std::barrier<> barrier2(3); // threads 4, 5, and the main thread
void barrierMain() {

    std::thread threads[6];

    // Start threads 1, 2, 3
    for (int i = 0; i < 3; ++i) {
        int id = i + 1;
        int effort = baseEffort - i * timeMultiplier;
        threads[i] = std::thread(barrierWorker, std::ref(barrier1), std::ref(barrier2), id, effort);
    }

    // Wait for threads 1, 2, 3 to complete
    barrier1.arrive_and_wait();

    // Start threads 4, 5
    for (int i = 3; i < 5; ++i) {
        int id = i + 1;
        int effort = baseEffort - i * timeMultiplier;
        threads[i] = std::thread(barrierWorker, std::ref(barrier1), std::ref(barrier2), id, effort);
    }

    // Wait for threads 4, 5 to complete
    barrier2.arrive_and_wait();

    // Start and wait for thread 6
    threads[5] = std::thread(barrierWorker, std::ref(barrier1), std::ref(barrier2), 6, 4 * timeMultiplier);
    threads[5].join();

    // Join threads 1-5
    for (int i = 0; i < 5; ++i) {
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
        printf("\nIteration %d starting", i);

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