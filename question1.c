#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


void *hello_world(void *param){

    printf("Hello World");
    return NULL;
}

void *goodbye(void *param){

    printf("Goodbye");
    sleep(rand()%10);
    return NULL;
}

int main(void)
{
    time_t tt;
    time(&tt);
    srand(tt);
    pthread_t firstid;     // id for thread one
    pthread_t secondid;    // id for thread two

    pthread_create(&firstid, NULL, hello_world, NULL);  // create thread call function
    pthread_create(&secondid, NULL, goodbye, NULL);     // create thread call function

    pthread_join(firstid, NULL);  // wait for first thread to terminate
    pthread_join(secondid, NULL); // wait for second thread to terminate

}