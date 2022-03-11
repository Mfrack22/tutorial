#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 5
#define NUM_GRADES 5

// student structure
struct student  
{ 
	char *name; 
  	char *student_id; 
  	int grade; 
};

void *bellcurve(void *param);
struct student* sinfo(void);

int main(int argc, char* argv[]){
    int num_students = 5;
    pthread_t pthreads[num_students];
    struct student *students[5];    // five students
    for(int i = 0; i < num_students; i++){
        struct student *s = sinfo();
        students[i] = s;

    }
    for(int i = 0; i < num_students; i++){
        pthread_create(&pthreads[i], NULL, bellcurve, students[i]);
    }

    for(int i = 0; i < num_students; i++){
        pthread_join(pthreads[i], NULL);
    }

    for(int i = 0; i < num_students; i++){
        free(students[i]->name);
        free(students[i]->student_id);
        free(students[i]);
    }
    
    return 0;
}

// bellcurve function
void *bellcurve(void *param){
    // derefrence param to int pointer
    struct student *std = (struct student*) param;
    int curve = std->grade * 1.5;
    if(curve > 100) {
        curve = 100;
        printf("Student %s:%s Grade: %d \n", std->name, std->student_id, curve);
    } else {
        printf("Student %s:%s Grade: %d \n", std->name, std->student_id, curve);
    }
    pthread_exit(0);
}

struct student *sinfo(void){
    struct student *std;
    std = (struct student *) calloc(1, sizeof(struct student));
    printf("Enter the student name: ");
    char name[50];
    scanf("%s", name);
    std->name  = (char *) strndup(name, strlen(name) + 1);
    printf("Enter the students id: ");
    char std_id[50];
    scanf("%s", std_id);
    std->student_id = (char *) strndup(std_id, strlen(std_id) + 1);
    printf("Enter students grade: ");
    scanf("%d", &std->grade);
    return std;
}
