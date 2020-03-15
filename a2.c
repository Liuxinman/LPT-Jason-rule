/*******************************************************************
 * a2.c                                                            *
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
#include <stdbool.h>

/* prototypes */

/* define a linked list to store every job and machine info */
typedef struct node{
    int data;
    struct node* next;
} NODE;
void print_list(NODE* head);
NODE* push(NODE* head, int val);

/* modified quicksort to sort job in the increasing order */
void quicksort(NODE* job[], int left, int right);
int split(NODE* job[], int left, int right);

/* functions for -i */
void read_in_instance(int *, int *);
void print_instance(NODE* job[], int *, int *);

// lpt rule
int lpt(NODE* job[], int t, int str, int end, int *num_machines_p);
int next_avail(NODE* is_avail[], int *num_machines_p);
int find_longest_time(NODE* is_avail[], int *num_machines_p);

// johnson rule
int johnson(NODE* job[], int t, int str, int end);
void johnson_order(NODE* job[], int str, int end, NODE* jobson_job[]);
int johnson_order_helper(NODE* job[], int str, int end, bool is_selected[], bool *if_put_first);

/* functions for -r */
void generate_random_instances(int *, int *);
void initialize_number_generator(void);


int main(int argc, char **argv) {
    int num_machines, *num_machines_p = &num_machines;
    int num_jobs, *num_jobs_p = &num_jobs;

    /* command-line options parsing */
    if ((argc != 2) || ((strcmp(argv[1], "-i") != 0) && (strcmp(argv[1], "-r") != 0))) {
        printf("%s -i | -r\n", argv[0]);
        return -10;
    }

    /* -i: read in an instance */
    if (strcmp(argv[1], "-i") == 0) {
        read_in_instance(num_machines_p, num_jobs_p);
    }

    /* -r: generate random instances */
    if (strcmp(argv[1], "-r") == 0) {
       generate_random_instances(num_machines_p, num_jobs_p);
    }

    return 0;
}

/*
 * read_in_instance: read in an instance from the user and print it out.
 *                   Then, use lpt and johnson's rule to calculate the job information, 
 *                   machine information and time makespan. Print all the information out.
 */
void read_in_instance(int *num_machines_p, int *num_jobs_p) {

    /* ask user to input information for constructing instances and validating the inputs */
    printf("Enter the number of machines in stage 1: ");
    if (scanf("%d", num_machines_p) != 1) {
        printf("Error in reading the instance!\n");
        return;
    }

    printf("Enter the number of jobs: ");
    if (scanf("%d", num_jobs_p) != 1) {
        printf("Error in reading the instance!\n");
        return;
    }

    NODE* job[*num_jobs_p];

    printf("Enter in each line the processing times for a job: \n");
    /* 
     * store the processing times of each job into a linked list
     * then store the head of each linked list into array job
     * linked list pattern: job_id -> aj -> bj -> cj
     */
    int processing_time;
    for (int i = 0; i < *num_jobs_p; i++) {
        NODE* head = (NODE*) malloc(sizeof (NODE));
        head->data = i+1;
        head->next = NULL;
        for (int j = 0; j < 3; j++) {
            if (scanf("%d", &processing_time) != 1) {
                printf("Error in reading the instance!\n");
                return;
            }
            push(head, processing_time);
        }
        job[i] = head;
    }

    printf("\n");
    
    /* print the instance to stdout */
    //print_instance(job, num_machines_p, num_jobs_p);

    int T1, T2, T3, T4, t;

    /* 
     * Sort job based on aj to divided it into two subarray
     * job[num_jobs - num_machines, ..., num_jobs - 1] is processed by lpt first then johnson
     * job[0, ..., num_jobs - num_machines - 1] is processed by johnson first then lpt
     */
    quicksort(job, 0, *num_jobs_p-1);

    if (*num_jobs_p > *num_machines_p) { /* only in this case we need do the partition for job */
       
        T1 = lpt(job, 0, *num_jobs_p - *num_machines_p, *num_jobs_p-1, num_machines_p);
        T2 = johnson(job, 0, 0, *num_jobs_p - *num_machines_p - 1);

        t = (T1 > T2)? T1:T2;

        T3 = lpt(job, t, 0, *num_jobs_p - *num_machines_p - 1, num_machines_p);
        T4 = johnson(job, t, *num_jobs_p - *num_machines_p, *num_jobs_p - 1);
        t += ((T3 > T4)? T3:T4);
        }

    else {
        T3 = lpt(job, 0, 0, *num_jobs_p - 1, num_machines_p);
        T4 = johnson(job, T3, 0, *num_jobs_p - 1);
        t = T3 + T4;
    }

    printf("The LPT-Johnson schedule has an overall makespan %d.\n\n", t);
    return;
}

/*
 * print_instance: print out the instance using fixed
 *                 format
 */
void print_instance(NODE* job[], int *num_machines_p, int *num_jobs_p) {

    printf("\n#instance%d_%d_%d.txt\n", *num_machines_p, *num_jobs_p, 1);

    printf("#number of machines in stage 1\n%d\n", *num_machines_p);
    
    printf("#number of jobs\n%d\n", *num_jobs_p);
    
    printf("#job processing times\n");

    for (int k = 0; k < *num_jobs_p; k++) {
        print_list(job[k]);
    }

    printf("#end of instance\n");
    printf("\n\n");
}

/*
 * generate_random_instance: ask for instance requirement
 *                           from the user and print it
 *                           out as requested
 */
void generate_random_instances(int *num_machines_p, int *num_jobs_p) {
    int t_1, t_2, num_instances;

    printf("Generating random instances ...\n");

    /* ask user to input information for constructing instances and check the inputs */
    printf("Enter the number of machines in stage 1: ");
    if (scanf("%d", num_machines_p) != 1) {
        printf("Error in reading the instance!\n");
        return;
    }

    printf("Enter the number of jobs: ");
    if (scanf("%d", num_jobs_p) != 1) {
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
        printf("\n#instance%d_%d_%d.txt\n", *num_machines_p, *num_jobs_p, i);
        printf("#number of machines in stage 1\n%d\n", *num_machines_p);
        printf("#number of jobs\n%d\n", *num_jobs_p);
        printf("#job processing time\n");
        
        for (int j = 0; j < *num_jobs_p; j++) {
            for (int k = 0; k < 3; k++) {
                printf("%d ", (rand() % (t_2 - t_1 + 1)) + t_1);
            }
            printf("\n");
        }

        printf("#end of instance\n\n\n");
    }
    return;
}

/*
 * initialize_number_generator: initializes the random
 *                              number generator using
 *                              the time of day
 */
void initialize_number_generator(void) {
    srand((unsigned) time(NULL));
}

/* 
 * quicksort: modified quicksort to sort job in the increasing order based on aj
 */
void quicksort(NODE* job[], int left, int right) {
	
	if (left >= right) {// termination condition: at most 1 element in the array
		return;
	}

	int mid = split(job, left, right);
	
	quicksort(job, left, mid - 1);
	quicksort(job, mid + 1, right);

	return;
}

int split(NODE* job[], int left, int right) {

	NODE* pivot = job[left];

    while (left < right) {// done when they are equal
        while (right > left && ((job[right]->next->data) >= (pivot->next->data))) {
            right--;
        }
        if (right > left)
            job[left++] = job[right];

        while (left < right && ((job[left]->next->data) < (pivot->next->data))) {
            left++;
        }
        if (left < right)
            job[right--] = job[left];
    }

    job[left] = pivot;
	    return left;
}

/* 
 * print_list: print out a linked list 
 */
void print_list(NODE* head) {

    NODE * temp;

    for (temp = head->next; temp; temp = temp->next)
        printf("%d ", temp->data);
    printf("\n");
}

/* 
 * push: add an new node with data val to the 
 *       end of a linked list with head head 
 */
NODE* push(NODE* head, int val) {

    NODE* temp = (NODE*) malloc(sizeof (NODE));
    temp->data = val;
    temp->next = NULL;

    if (head == NULL) {
        head = temp;
    }
    else {
        NODE* current = head;
        /* traverse to the end of the linked list */
        while (current->next != NULL) {
            current = current->next;
        }
        /* now we can add a new variable */
        current->next = temp;
    }
    return head;
}

/*
 * lpt: process job on machines in stage 1 using longest processing time (LPT) rule.
 *      print out the LPT order, job information and machines information.
 *      returns the time point at which the process is done(T)
 */
int lpt(NODE* job[], int t, int str, int end, int *num_machines_p) {

    NODE* machinesA[*num_machines_p+1];
    int T = 0;

    /* initialize a node* array to store the machines A1, A2,..., An information */
    /* pattern: t(next_available_time) -> job_id1 -> job_starting_time1 -> job_id2 -> job_starting_time2 ... */
    for (int i = 1; i <= *num_machines_p; i++) {
        NODE* head = (NODE*) malloc(sizeof (NODE));
        head->data = t;
        head->next = NULL;
        machinesA[i] = head;
    }

    /* print out lpt order */
    printf("LPT order:");
    for (int i = end; i >= str; i--) {
        (i == end)? printf(" %d", (job[i]->data)):printf(", %d", (job[i]->data));
    }
    printf("\n\n\n");

    /* print out job information and store the info into the machinesA array for each machine */
    printf("Job information: \n");
    for (int i = end; i >= str; i--) {
        int next = next_avail(machinesA, num_machines_p);
        push(machinesA[next], job[i]->data); // push into job_id
        push(machinesA[next], machinesA[next]->data); // push into job_starting_time
        printf("Job %d is processed on A_%d starting %d", job[i]->data, next, machinesA[next]->data);
        (i == str)? printf(".\n"):printf(";\n");
        machinesA[next]->data += job[i]->next->data; // calculate machine's next available time point
    }
    printf("\n\n");

    /* print out the machines information */
    printf("Machine information: \n");
    for (int i = 1; i <= *num_machines_p; i++) {
        printf("A_%d processes", i);
        NODE * temp;
        for (temp = machinesA[i]->next; temp; temp = temp->next->next)
            if (temp == machinesA[i]->next)
                printf(" job %d at %d", temp->data, temp->next->data);
            else
                printf(", job %d at %d", temp->data, temp->next->data);
        (i == *num_machines_p)? printf(".\n"):printf(";\n");

    }
    printf("\n\n");

    /* calculate time interval and makespan */
    T = find_longest_time(machinesA, num_machines_p);

    printf("The job processing time interval is [%d, %d], and the makespan is %d.\n\n\n", t, T, T-t);

    return (T-t);
}

/* 
 * next_avail: find the next available machine by comparing the time point 
 *             at which they finish their last job. returns the NO.machine.
 */
int next_avail(NODE* machinesA[], int *num_machines_p) {
    int next = 1;
    int min = machinesA[1]->data;

    for (int i = 2; i <= *num_machines_p; i++) {
        if (machinesA[i]->data < min) {
            min = machinesA[i]->data;
            next = i;
        }
    }
    return next;
}

/* 
 * longest_time: calculates and returns the time point at which the process is done(T)
 */
int find_longest_time(NODE* machinesA[], int *num_machines_p) {

    int max = machinesA[1]->data;

    for (int i = 2; i <= *num_machines_p; i++) {
        if (machinesA[i]->data > max) {
            max = machinesA[i]->data;
        }
    }
    return max;
}

/*
 * johnson: process jobs on the two-machine flowshop (first B then C) using johnson's rule
 *          print out the johnson's order, job information, machines informatioin and time makespan
 *          returns the time point at which the process is done(T)
 */
int johnson(NODE* job[], int t, int str, int end) {

    int T = 0;
    NODE* johnson_job[end - str + 1];
    NODE* machinesbc[2];

    /* calculate the order of johnson's rule */
    johnson_order(job, str, end, johnson_job);

    printf("Johnson's order:");
    for (int i = 0; i < end - str + 1; i++) {
        (i == 0)? printf(" %d", johnson_job[i]->data):printf(", %d", johnson_job[i]->data);
    }
    printf("\n\n\n");

    /* initialize an node array to store machines infomation */
    /* pattern: t(next_available_time) -> job_id1 -> job_starting_time1 -> job_id2 -> job_starting_time2 ... */
    for (int i = 0; i < 2; i++) {
        NODE* head = (NODE*) malloc(sizeof (NODE));
        head->data = t;
        head->next = NULL;
        machinesbc[i] = head;
    }
    machinesbc[1]->data += (johnson_job[0]->next->next->data); // calculate the first next available time point for C

    /* print out the job information and store the info into machinesbc array */
    printf("Job information: \n");
    for (int i = 0; i < end - str + 1; i++) {

        push(machinesbc[0], johnson_job[i]->data); // push job_id for B
        push(machinesbc[0], machinesbc[0]->data); // push job_starting time for B
        push(machinesbc[1], johnson_job[i]->data); // push job_id for C
        push(machinesbc[1], machinesbc[1]->data); // push job_starting_time for C

        printf("Job %d is processed on B staring %d, on C starting %d", johnson_job[i]->data, machinesbc[0]->data, machinesbc[1]->data);
        (i == end - str)? printf(".\n"):printf(";\n");
        
        machinesbc[0]->data += johnson_job[i]->next->next->data; // calculate the next_available_time for B

        /* calculate the next_available_time for B */
        machinesbc[1]->data += johnson_job[i]->next->next->next->data; 
        if ((i < end - str) && ((machinesbc[0]->data + johnson_job[i+1]->next->next->data) > machinesbc[1]->data)) { /* check if B finish first or C */
            machinesbc[1]->data = (machinesbc[0]->data + johnson_job[i+1]->next->next->data); 
        }
    }
    printf("\n\n");

    /* print out the machine B and C infomation */
    printf("Machines infomation: \n");
    // machines B information
    printf("B processes");
    NODE * tempb;
    for (tempb = machinesbc[0]->next; tempb; tempb = tempb->next->next)
        if (tempb == machinesbc[0]->next)
            printf(" job %d at %d", tempb->data, tempb->next->data);
        else
            printf(", job %d at %d", tempb->data, tempb->next->data);
    printf(";\n");

    // machines C information
    printf("C processes");
    NODE * tempc;
    for (tempc = machinesbc[1]->next; tempc; tempc = tempc->next->next)
        if (tempc == machinesbc[1]->next)
            printf(" job %d at %d", tempc->data, tempc->next->data);
        else
            printf(", job %d at %d", tempc->data, tempc->next->data);
    printf(".\n\n\n");

    /* calculate the time interval and makespan */
    T  = machinesbc[1]->data;

    printf("The job processing time interval is [%d, %d], and the makespan is %d.\n\n\n", t, T, T-t);

    return (T-t);
}

/* 
 * johnson_order: find the order of jobs by Johnson's rule 
 *                and insert each job into a new array
 */
void johnson_order(NODE* job[], int str, int end, NODE* johnson_job[]) {

    bool is_selected[end - str + 1]; //if the job is already in johnson_job
    int i = 0, j = end - str;
    bool find_b_min = true, *if_put_first = &find_b_min;
    int next;

    /* initialize is_selected and set all element to be not selected at first */
    for (int i = 0; i < end - str + 1; i++) {
        is_selected[i] = false;
    }

    while (i <= j) {
        next = johnson_order_helper(job, str, end, is_selected, if_put_first);
        if (find_b_min) { /* bj contains the smallest element */
            johnson_job[i] = job[next];
            i++;
        }
        else { /* cj contains the smallest element */
            johnson_job[j] = job[next];
            j--;
        }
    }
}

/* 
 * johnson_order_helper: find the min of unselected jobs based on bj or cj
 *                       returns the index of next selected job in the original job array
 */
int johnson_order_helper(NODE* job[], int str, int end, bool is_selected[], bool *find_b_min) {
    
    int next_b, next_c, min_b, min_c;

    /* find the first job that is not selected */
    next_b = str;
    while (is_selected[next_b - str] == true && next_b < end) next_b++;
    next_c = next_b;

    /* min based on bj */
    min_b = job[next_b]->next->next->data;
    for (int i = next_b; i <= end; i++) {
        if ((job[i]->next->next->data < min_b) && (is_selected[i-str] == false)) {
            min_b = job[i]->next->next->data;
            next_b = i;
        }
    }

    /* min based on cj */
    min_c = job[next_c]->next->next->next->data;
    for (int i = next_c; i <= end; i++) {
        if ((job[i]->next->next->next->data < min_c) && (is_selected[i-str] == false)) {
            min_c = job[i]->next->next->next->data;
            next_c = i;
        }
    }

    /* compare min_b and min_c, and choose the smaller one */
    if (min_b <= min_c) {
        is_selected[next_b - str] = true;
        *find_b_min = true;
        return next_b;
    }
    else {
        is_selected[next_c - str] = true;
        *find_b_min = false;
        return next_c;
    }
}
