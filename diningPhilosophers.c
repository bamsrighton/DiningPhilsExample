#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>

#define N 5
#define NUM_BITES 10000
#define LEFT ((i+N-1)%N)
#define RIGHT ((i+1)%N)
#define THINKING 0
#define HUNGRY 1
#define EATING 2

// DINING PHILOSOPHERS
// example multithreaded program in C
// author: SAM BRIGHTON

int state[N]; // state of each philosopher
sem_t mutex; // the mutual exclusion semaphore for the state array
sem_t s[N]; // semaphores for each philosopher

//private helper methods
// i is the philosopher number (from the philosopher thread argument)
void test(int i) {
   if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
      state[i] = EATING;
      sem_post(&s[i]);
   }
}

void take_forks(int i) {
   sem_wait(&mutex);
   state[i] = HUNGRY;
   test(i);
   sem_post(&mutex);
   sem_wait(&s[i]);
}

void put_forks(int i) {
   sem_wait(&mutex);
   state[i] = THINKING;
   test(LEFT);
   test(RIGHT);
   sem_post(&mutex);
}

// a philosopher (whose number is philNum) will do this 100 times:
//   take_forks(philNum);
//   eat a bite of food (i.e. descriptive print statement)
//   put_forks(philNum);

void* philosopher(void* i) {

   int argumentValue = *((int *) i);
   int bites;
   
   for(bites = 0; bites < NUM_BITES; bites++) {
      take_forks(argumentValue);
      printf("Philosopher numba %d on bite numba %d.\n", argumentValue, bites);
      put_forks(argumentValue);
   }
   printf("Philosopher numba %d is full", argumentValue);
   pthread_exit(0);
}

// define main down here at the bottom of the file

int main() {

   sem_init(&mutex, 0, 1);

   pthread_t philThread[N];
   int p;
   for(p=0; p<N; p++){
       state[p] = 0;
       sem_init(&s[p], 0, 0);
   }

   int i;
   for(i=0; i<N; i++){
      
      int *temp = (int *) malloc(sizeof(int *));
      *temp = i; 

      pthread_create(&philThread[i], 0, philosopher, (void *) temp);

   }
   
   int y;
   for(y=0; y<N; y++){
      pthread_join(philThread[y], 0);
   }

   sem_close(&mutex);

   int k;
   for(k=0; k<N; k++){
      sem_close(&(s[k]));
   }

   return 0;
}
