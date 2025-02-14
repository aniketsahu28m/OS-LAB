#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>  // Added for the sleep function

#define NUM_READERS 5  // Number of reader threads
#define NUM_WRITERS 3  // Number of writer threads

// Shared data
int shared_data = 0; // Shared resource (data)

// Semaphores
sem_t mutex;  // Semaphore to protect reader_count
sem_t write_lock; // Semaphore to allow exclusive access to writers
int reader_count = 0; // Number of readers currently reading

// Reader thread function
void* reader(void* param) {
    while (1) {
        // Request permission to read
        sem_wait(&mutex); // Access the reader_count safely
        reader_count++;
        if (reader_count == 1) {
            sem_wait(&write_lock); // Block writers if this is the first reader
        }
        sem_post(&mutex); // Release access to reader_count

        // Reading the shared data
        printf("Reader %ld is reading shared data: %d\n", (long)param, shared_data);

        // Done reading, now update reader_count
        sem_wait(&mutex); // Access the reader_count safely
        reader_count--;
        if (reader_count == 0) {
            sem_post(&write_lock); // Release the writer lock if this is the last reader
        }
        sem_post(&mutex); // Release access to reader_count

        // Simulate time taken for reading
        sleep(1); // Sleep for 1 second
    }
}

// Writer thread function
void* writer(void* param) {
    while (1) {
        // Request permission to write (ensure exclusive access)
        sem_wait(&write_lock); // Wait for exclusive access to the resource

        // Writing to the shared data
        shared_data++;
        printf("Writer %ld is writing new value: %d\n", (long)param, shared_data);

        // Simulate time taken for writing
        sleep(2); // Sleep for 2 seconds

        // Done writing, release the lock
        sem_post(&write_lock);
    }
}

int main() {
    // Initialize semaphores
    sem_init(&mutex, 0, 1); // Binary semaphore for reader_count management
    sem_init(&write_lock, 0, 1); // Binary semaphore for writer exclusive access

    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];

    // Create reader threads
    for (long i = 0; i < NUM_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, (void*)i);
    }

    // Create writer threads
    for (long i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, (void*)i);
    }

    // Wait for reader and writer threads to finish (in this case, they run indefinitely)
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&write_lock);

    return 0;
}
