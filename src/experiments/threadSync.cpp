#include <iostream>
#include <thread>
#include <cstdio>

#include <semaphore>
#include <latch>
#include <barrier>
#include <chrono>

void logStart(int id) {
    printf("\nThread %d starting", id);
}

void logComplete(int id) {
    printf("\nThread %d completing", id);
}

//----------------
// Semaphore

std::counting_semaphore sem1(0); // threads 1, 2, 3
std::counting_semaphore sem2(0); // threads 4, 5

void semaphoreWorker(int id, int effort) {
    logStart(id);
    std::this_thread::sleep_for(std::chrono::milliseconds(effort));
    logComplete(id);
    if (id <= 3) sem1.release();
    else if (id <= 5) sem2.release();
}

void semaphoreMain() {
    std::thread threads[6];

    // Start threads 1, 2, 3
    for (int i = 0; i < 3; ++i) {
        threads[i] = std::thread(semaphoreWorker, i + 1, 1000 - i * 100);
    }

    // Wait for threads 1, 2, 3 to complete
    for (int i = 0; i < 3; ++i) {
        sem1.acquire();
    }

    // Start threads 4, 5
    for (int i = 3; i < 5; ++i) {
        threads[i] = std::thread(semaphoreWorker, i + 1, 1000 - i * 100);
    }

    // Wait for threads 4, 5 to complete
    for (int i = 0; i < 2; ++i) {
        sem2.acquire();
    }

    // Start and wait for thread 6
    threads[5] = std::thread(semaphoreWorker, 6, 400);
    threads[5].join();

    // Join threads 1-5
    for (int i = 0; i < 5; ++i) {
        threads[i].join();
    }
}

//----------------
// Latch

std::latch latch1(3); // threads 1, 2, 3
std::latch latch2(2); // threads 4, 5

void latchWorker(int id, int effort) {
    logStart(id);
    std::this_thread::sleep_for(std::chrono::milliseconds(effort));
    logComplete(id);
    if (id <= 3) latch1.count_down();
    else if (id <= 5) latch2.count_down();
}

void latchMain() {
    std::thread threads[6];

    // Start threads 1, 2, 3
    for (int i = 0; i < 3; ++i) {
        threads[i] = std::thread(latchWorker, i + 1, 1000 - i * 100);
    }

    // Wait for threads 1, 2, 3 to complete
    latch1.wait();

    // Start threads 4, 5
    for (int i = 3; i < 5; ++i) {
        threads[i] = std::thread(latchWorker, i + 1, 1000 - i * 100);
    }

    // Wait for threads 4, 5 to complete
    latch2.wait();

    // Start and wait for thread 6
    threads[5] = std::thread(latchWorker, 6, 400);
    threads[5].join();

    // Join threads 1-5
    for (int i = 0; i < 5; ++i) {
        threads[i].join();
    }
}

//----------------
// Barrier

std::barrier<> barrier1(4); // threads 1, 2, 3, and the main thread
std::barrier<> barrier2(3); // threads 4, 5, and the main thread

void barrierWorker(int id, int effort) {
    logStart(id);
    std::this_thread::sleep_for(std::chrono::milliseconds(effort));
    logComplete(id);
    if (id <= 3) {
        barrier1.arrive_and_wait();
    } else if (id <= 5) {
        barrier2.arrive_and_wait();
    }
}

void barrierMain() {
    std::thread threads[6];

    // Start threads 1, 2, 3
    for (int i = 0; i < 3; ++i) {
        threads[i] = std::thread(barrierWorker, i + 1, 1000 - i * 100);
    }

    // Wait for threads 1, 2, 3 to complete
    barrier1.arrive_and_wait();

    // Start threads 4, 5
    for (int i = 3; i < 5; ++i) {
        threads[i] = std::thread(barrierWorker, i + 1, 1000 - i * 100);
    }

    // Wait for threads 4, 5 to complete
    barrier2.arrive_and_wait();

    // Start and wait for thread 6
    threads[5] = std::thread(barrierWorker, 6, 400);
    threads[5].join();

    // Join threads 1-5
    for (int i = 0; i < 5; ++i) {
        threads[i].join();
    }
}



int main() {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    printf("\n\nSemaphore");
    auto semaphoreStart = std::chrono::high_resolution_clock::now();
    semaphoreMain();
    auto semaphoreEnd = std::chrono::high_resolution_clock::now();
    auto semaphoreDuration = std::chrono::duration_cast<std::chrono::milliseconds>(semaphoreEnd - semaphoreStart).count();
    printf("\nTime taken | semaphore: %6i milliseconds", static_cast<int>(semaphoreDuration));


    printf("\n\nLatch");
    auto latchStart = std::chrono::high_resolution_clock::now();
    latchMain();
    auto latchEnd = std::chrono::high_resolution_clock::now();
    auto latchDuration = std::chrono::duration_cast<std::chrono::milliseconds>(latchEnd - latchStart).count();
    printf("\nTime taken | latch: %6i milliseconds", static_cast<int>(latchDuration));

    printf("\n\nBarrier");
    auto barrierStart = std::chrono::high_resolution_clock::now();
    barrierMain();
    auto barrierEnd = std::chrono::high_resolution_clock::now();
    auto barrierDuration = std::chrono::duration_cast<std::chrono::milliseconds>(barrierEnd - barrierStart).count();
    printf("\nTime taken | latch: %6i milliseconds", static_cast<int>(barrierDuration));

}