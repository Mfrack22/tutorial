#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_GRADES 5

// global array holds students grades


// bellcurve function
void *bellcurve(void *param){
    // derefrence param to int pointer
    int grade = *(int*)param;
    int curve = grade * 1.5;
    if(curve > 100){
        curve = 100;
        printf("Student %d Grade: %d \n", grade+1, curve);
    }else
    {
        printf("Student %d Grade: %d \n", grade+1, curve);
    }
    pthread_exit(0);
}

int main(int argc, char* argv[]){

    pthread_t threads[NUM_THREADS];\

    int grades[NUM_GRADES];
    //int *grades;
    //Take in 5 student grades
    printf("Please enter in 5 students grades\n");
    for(int i = 0; i < NUM_GRADES; i++){
        printf("%d :", i+1);
        scanf("%d", &grades[i]);
    }

    printf("\nStudent Grades after Bell Curve\n");
    printf("_______________________________\n");

    //Create 5 threads and call bellcurve
    for(int j = 0; j < NUM_THREADS; j++){
        // allocate memory to pointer
        int *a = malloc(sizeof(int));
        *a = j;
        if(pthread_create(&threads[j], NULL, bellcurve, &grades[j]) != 0){
            perror("Failed to create thread");
        }
    }

    //Join threads back to process
    for(int k = 0; k < NUM_THREADS; k++){
        if(pthread_join(threads[k],NULL)!=0){
            perror("Faled to join thread");
        }
    }
    return(0);
}
