#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_GRADES 5

// student structure
struct student  
{ 
	char name[10]; 
  	char student_id[10]; 
  	int grade[NUM_GRADES]; 
}; 

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


struct student info(void){

    struct student std;
    printf("Enter the student name: \n");
    scanf("%s", std.name);

    printf("Enter the students id: \n");
    scanf("%s", std.student_id);

    printf("Enter students grade: \n");
    scanf("%d", std.grade);

}

void student_info(struct student std){
    printf("Student Name: %c", std.name);
}

int main(int argc, char* argv[]){
    
    struct student array[5];        // five students

    for(int i = 0; i < 2; i++){
        array[i] = info();
    }
    printf("\nTest%s",&array[1].name);

    // for(int k = 0; k < 2; k++){
    //     student_info(array[k]);
    // }
    
    
    
    return 0;
}