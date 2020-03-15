/*******************************************************************
 * a1.c                                                            *
 * Submitting student: Xinman Liu                                  *
 * Student ID:                                                     *
 * Collaborating classmates: No                                    *
 * Other collaborators: No                                         *
 * References (excluding textbook and lecture slides): No          *
 *******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* external variable */
int num_machines;
int num_jobs;

/* prototypes */
void read_in_instance(void);
void print_instance(int job[][3]);
void generate_random_instances(void);
void initialize_number_generator(void);


int main(int argc, char *argv[]) {

    /* command-line options parsing */
    if ((argc != 2) || ((strcmp(argv[1], "-i") != 0) && (strcmp(argv[1], "-r") != 0))) {
        printf("%s -i | -r\n", argv[0]);
        return -10;
    }

    /* -i: read in an instance */
    if (strcmp(argv[1], "-i") == 0) {
        read_in_instance();
    }

    /* -r: generate random instances */
    if (strcmp(argv[1], "-r") == 0) {
       generate_random_instances();
    }

    return 0;
}

/*********************************************************
 * read_in_instance: read in an instance from the user   *
 *                   and print it out                    *
 *********************************************************/
void read_in_instance(void) {

    /* ask user to input information for constructing instances and check the inputs */
    printf("Enter the number of machines in stage 1: ");
    if (scanf("%d", &num_machines) != 1) {
        printf("Error in reading the instance!\n");
        return;
    }

    printf("Enter the number of jobs: ");
    if (scanf("%d", &num_jobs) != 1) {
        printf("Error in reading the instance!\n");
        return;
    }

    int job[num_jobs][3]; //request a consecutive memory slot to save it in order to run program

    printf("Enter in each line the processing times for a job: \n");
    /* store the job processing time information into an array */
    for (int i = 0; i < num_jobs; i++) {
        for (int j = 0; j < 3; j++) {
            if (scanf("%d", &job[i][j]) != 1) {
                printf("Error in reading the instance!\n");
                return;
            }
        }
    }

    /* print the instance to stdout */
    print_instance(job);
    return;
}

/*********************************************************
 * print_instance: print out the instance using fixed    *
 *                 format                                *
 *********************************************************/
void print_instance(int job[][3]) {

    printf("\n#instance%d_%d_%d.txt\n", num_machines, num_jobs, 1);

    printf("#number of machines in stage 1\n%d\n", num_machines);
    
    printf("#number of jobs\n%d\n", num_jobs);
    
    printf("#job processing times\n");

    for (int k = 0; k < num_jobs; k++) {
        for (int w = 0; w < 3; w++) {
            printf("%d ", job[k][w]);
        }
        printf("\n");
    }

    printf("#end of instance\n");
}

/*********************************************************
 * generate_random_instance: ask for instance requirement*
 *                           from the user and print it  *
 *                           out as requested            *
 *********************************************************/
void generate_random_instances(void) {
    int t_1, t_2, num_instances;

    printf("Generating random instances ...\n");

    /* ask user to input information for constructing instances and check the inputs */
    printf("Enter the number of machines in stage 1: ");
    if (scanf("%d", &num_machines) != 1) {
        printf("Error in reading the instance!\n");
        return;
    }

    printf("Enter the number of jobs: ");
    if (scanf("%d", &num_jobs) != 1) {
        printf("Error in reading the instance!\n");
        return;
    }

    printf("Enter the processing time interval [t_1, t_2]: ");
    if (scanf("%d%d", &t_1, &t_2) != 2) {
        printf("Error in reading the instance!\n");
        return;
    }

    printf("Enter the number of instances to be generated: ");
    if (scanf("%d", &num_instances) != 1) {
        printf("Error in reading the instance!\n");
        return;
    }

    /* print the instance to stdout */
    initialize_number_generator();
    for (int i = 1; i <= num_instances; i++) {
        printf("\n#instance%d_%d_%d.txt\n", num_machines, num_jobs, i);
        printf("#number of machines in stage 1\n%d\n", num_machines);
        printf("#number of jobs\n%d\n", num_jobs);
        printf("#job processing time\n");
        
        for (int j = 0; j < num_jobs; j++) {
            for (int k = 0; k < 3; k++) {
                printf("%d ", (rand() % (t_2 - t_1 + 1)) + t_1);
            }
            printf("\n");
        }

        printf("#end of instance\n\n\n");
    }
    return;
}

/*********************************************************
 * initialize_number_generator: initializes the random   *
 *                              number generator using   *
 *                              the time of day          *
 *********************************************************/
void initialize_number_generator(void) {
    srand((unsigned) time(NULL));
}
