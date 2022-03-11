#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

#define IN_READ_GRADES  "[Read_Grades]"
#define IN_ROUTINE      "\t[Routine]"
#define APPEND_FN       "bellcurve.txt"
#define READ_FN         "grades.txt"
#define ERROR_HANDLER(en, msg)  { errno = en; perror(msg); exit(EXIT_FAILURE); }
#define BUFF_LEN 256
#define NUM_GRADES 10
#define NUM_THREADS 11

static pthread_barrier_t barrier;
static pthread_mutex_t mutex;

int total_grade = 0;
double total_bellcurve = 0;

typedef struct read_grades_params_s {
    char *filename;
    int *grades[NUM_GRADES];
} read_grades_params_t;

typedef struct routine_params_s {
    int thread_id;
    int *grade;
} routine_params_t;

void *read_grades(void *vptr);

void *routine(void *vptr);

int main(int argc, char **argv) {
    pthread_t threads[NUM_THREADS];
    read_grades_params_t *read_grades_params = (read_grades_params_t *) calloc(1, sizeof(read_grades_params_t));
    read_grades_params->filename = strndup(READ_FN, strlen(READ_FN) + 1);
    for (int i = 0; i < NUM_GRADES; i++) {
        read_grades_params->grades[i] = calloc(1, sizeof(int));
    }

    /* initialize the mutex and barrier */
    /* barrie for read_grades and routine synchronization */
    /* mutext for creating a critical section in the routine so that only one routine can update the global variable thus eliminating the race condition */
    pthread_mutex_init(&mutex, NULL);

    unsigned int barrier_count = NUM_THREADS;
    int ret;
    ret = pthread_barrier_init(&barrier, NULL, barrier_count);
    if(ret) {
        switch(ret) {
            case EAGAIN:
                printf(" Lacks the resources to initialize barrier\n"); 
                break;
            case EINVAL:
                printf(" Barrier Count is equal to zero\n");            
                break;
            case ENOMEM:
                printf(" Insufficient memory exists to init barrier\n");
                break;
        }
        ERROR_HANDLER(ret, "pthread_barrier_init");
    }

    /* create all of the threads including the read_grades */
    for (int i = 0; i < NUM_THREADS - 1; i++) {
        int *tmp =read_grades_params->grades[i];
        pthread_create(&threads[i], NULL, &routine, (void *) tmp);
    }
    pthread_create(&threads[NUM_THREADS-1], NULL, &read_grades, (void *)read_grades_params);        


    /* make the main thread wait for the completion of the joined threads */
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }


    for (int i = 0; i < NUM_GRADES; i++) {
        free(read_grades_params->grades[i]);
    }
    free(read_grades_params->filename);
    free(read_grades_params);
    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&mutex);

    printf("Total grades = %d\n", total_grade);
    printf("Class average before bellcurve %f\n", total_grade/(float) NUM_GRADES);
    printf("Class average after bellcurve %f\n", total_bellcurve/(float) NUM_GRADES);

    return 0;
}

/* reads the grade from grades.txt and store them in the struct */
void *read_grades(void *vptr) {
    read_grades_params_t *params = (read_grades_params_t *) vptr;
    FILE *grades_fd;
    grades_fd = fopen(params->filename, "r");
    char buff[BUFF_LEN];
    int i = 0;

    while (fgets(buff, BUFF_LEN, grades_fd) != NULL && i < NUM_GRADES) {
        int *tmp = params->grades[i];
        *tmp = atoi(buff);
        i++;
    }

    fclose(grades_fd);

    /* read_grades threads will reach the barrier checkpoint after reading the grades.txt file and updating the struct */
    pthread_barrier_wait(&barrier);

    pthread_exit(NULL);
}

void *routine(void *vptr) {
    /* this thread routine will reach the barrier checkpoint at the start of the routine and will wait here until read_grades routine reaches the barrier checkpoint. */
    // this routine should wait for the read_grade routine before proceeding
    pthread_barrier_wait(&barrier);


    /* Entering critical section since updating global variable */
    pthread_mutex_lock(&mutex);
    /* ---------- CRITICAL SECTION STARTS ----------- */

    int *grade_ptr = (int *) vptr;
    int grade = *grade_ptr;
    double bellcurve_grd;


    // add grade to total_grade
    total_grade += grade;

    // add bellcurved grade to total_bellcurve
    bellcurve_grd = grade * 1.50;
    total_bellcurve += bellcurve_grd;

    // append the bellcurved grade to bellcurve.txt file
    FILE *bellcurve_fd = fopen("bellcurve.txt", "a");
    char buff[BUFF_LEN];
    snprintf(buff, BUFF_LEN, "%f\n", bellcurve_grd);
    fputs(buff, bellcurve_fd);
    fclose(bellcurve_fd);

    pthread_mutex_unlock(&mutex);
    /* ----------- CRITICAL SECTION ENDS -------------  */

    pthread_exit(NULL);
}