#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (index + 4) % N
#define RIGHT (index + 1) % N

int state[N];

sem_t mutex;
sem_t forks[N];

void test(int index) {
	if (state[index] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
		// state that eating
		state[index] = EATING;
		sleep(2);
		printf("Philosopher %d takes fork %d and %d\n", index + 1, LEFT + 1, index + 1);
		printf("Philosopher %d is Eating\n", index + 1);
		sem_post(&forks[index]);
	}
}

// take up forks
void take_fork(int index) {
	sem_wait(&mutex);
	// state that hungry
	state[index] = HUNGRY;
	printf("Philosopher %d is Hungry\n", index + 1);
	// eat if neighbours are not eating
	test(index);
	sem_post(&mutex);
	// if unable to eat wait to be signalled
	sem_wait(&forks[index]);
	sleep(1);
}

// put down chopsticks
void put_fork(int index) {
	sem_wait(&mutex);
	// state that thinking
	state[index] = THINKING;
	printf("Philosopher %d putting fork %d and %d down\n", index + 1, LEFT + 1, index + 1);
	printf("Philosopher %d is thinking\n", index + 1);
	test(LEFT);
	test(RIGHT);
	sem_post(&mutex);
}

void* philosopher(void* arg) {
	while (1) {
		int* index = (int*)arg;
		sleep(1);
		take_fork(*index);
		sleep(0);
		put_fork(*index);
	}
}

int main() {
	pthread_t threads[N];
	// initialize the semaphores
	sem_init(&mutex, 0, 1);
	for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 0);
    }

    int phil[N] = {0, 1, 2, 3, 4};
	for (int i = 0; i < N; i++) {
		// create philosopher processes
		pthread_create(&threads[i], NULL, philosopher, &phil[i]);
		printf("Philosopher %d is thinking\n", i + 1);
	}

	for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;	
}
