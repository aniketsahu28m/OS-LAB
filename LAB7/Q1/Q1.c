#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  // Include this header for the sleep function

#define CAPACITY 5 // Capacity of the buffer

// Buffer and semaphores
int buffer[CAPACITY];
int in = 0; // Points to the next empty slot
int out = 0; // Points to the next full slot
sem_t empty;  // Semaphore to count the empty slots
sem_t full;   // Semaphore to count the full slots
sem_t mutex;  // Binary semaphore to protect buffer access

// Function to simulate producing an item
int produce_item() {
    return rand() % 100;  // Produce a random item between 0 and 99
}

// Producer thread function
void* producer(void* param) {
    while (1) {
        int item = produce_item();
        
        sem_wait(&empty);  // Wait for an empty slot
        sem_wait(&mutex);  // Acquire mutex to access the buffer
        
        buffer[in] = item;  // Produce item and put it in the buffer
        printf("Produced item: %d at index %d\n", item, in);
        in = (in + 1) % CAPACITY;  // Update the in index
        
        sem_post(&mutex);  // Release mutex
        sem_post(&full);   // Signal that there's a full slot

        sleep(1);  // Simulate time to produce an item
    }
}

// Consumer thread function
void* consumer(void* param) {
    while (1) {
        sem_wait(&full);  // Wait for a full slot
        sem_wait(&mutex); // Acquire mutex to access the buffer

        int item = buffer[out];  // Consume the item
        printf("Consumed item: %d from index %d\n", item, out);
        out = (out + 1) % CAPACITY;  // Update the out index

        sem_post(&mutex);  // Release mutex
        sem_post(&empty);  // Signal that there's an empty slot

        sleep(1);  // Simulate time to consume an item
    }
}

int main() {
    // Initialize semaphores
    sem_init(&empty, 0, CAPACITY);  // Initially, the buffer is empty (capacity slots available)
    sem_init(&full, 0, 0);  // Initially, no item is in the buffer
    sem_init(&mutex, 0, 1); // Mutex is available (1) to access the buffer

    pthread_t producer_thread, consumer_thread;

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for both threads to finish (in this case, they run indefinitely)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
