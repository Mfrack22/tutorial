//
// Created by ahmetkca on 2022-02-28.
//

#define _XOPEN_SOURCE 600 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#define NUM_STUDENTS 10


struct student
{
    char *name;
    char *student_id;
    int grade;
};

int total_grade = 0;

struct student *sinfo(void);
void *class_total(void *vptr);

int main(int argc, char **argv) {
    int num_students = NUM_STUDENTS;
    pthread_t workers[num_students];
    struct student *students[NUM_STUDENTS];
    for(int i = 0; i < num_students; i++) {
        students[i] = sinfo();
    }
    for(int i = 0; i < num_students; i++){
        pthread_create(&workers[i], NULL, class_total, students[i]);
    }

    for(int i = 0; i < num_students; i++){
        pthread_join(workers[i], NULL);
    }

    printf("Class total is %d\n", total_grade);
    return 0;
}

void *class_total(void *vptr) {
    struct student *std = (struct student *) vptr;
    printf("%d += %d -> ", total_grade, std->grade);
    pthread_mutex_lock(&mutex);
    /* critical section starts */

    total_grade += std->grade;

    /* critical section ends*/
    pthread_mutex_unlock(&mutex);
    printf("%d\n", total_grade);
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
