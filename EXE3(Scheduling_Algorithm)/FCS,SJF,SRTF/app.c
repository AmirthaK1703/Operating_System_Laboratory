#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "head.h"

void copy_processes(Process src[], Process dest[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void input_processes(Process p[], int n) {
    printf("\n--- Enter Process Details ---\n");
    int i;
    for (i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Process P%d Arrival Time: ", i + 1);
        scanf("%f", &p[i].arrival_time);
        printf("Process P%d Burst Time  : ", i + 1);
        scanf("%f", &p[i].burst_time);
    }
}

int main() {
    int n = 0;
    int choice;
    bool data_entered = false;

    Process *original_proc = NULL;
    Process *work_proc = NULL;

    do {
        printf("\n=========================================\n");
        printf("         CPU SCHEDULING SIMULATOR        \n");
        printf("=========================================\n");
        printf(" 1. Enter Process Data\n");
        printf(" 2. Run First-Come, First-Served (FCFS)\n");
        printf(" 3. Run Shortest Job First (SJF)\n");
        printf(" 4. Run Shortest Remaining Time First (SRTF)\n");
        printf(" 5. Run ALL Algorithms\n");
        printf(" 6. Exit\n");
        printf("=========================================\n");
        printf("Enter your choice (1-6): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                printf("\nEnter number of processes: ");
                if (scanf("%d", &n) != 1 || n <= 0) {
                    printf("Invalid number of processes.\n");
                    n = 0;
                    data_entered = false;
                    break;
                }

                if (original_proc) free(original_proc);
                if (work_proc) free(work_proc);

                original_proc = malloc(n * sizeof(Process));
                work_proc = malloc(n * sizeof(Process));

                input_processes(original_proc, n);
                data_entered = true;
                printf("\nProcess data saved successfully!\n");
                break;

            case 2:
                if (!data_entered) {
                    printf("\n[!] Please enter process data first (Option 1).\n");
                    break;
                }
                copy_processes(original_proc, work_proc, n);
                fcfs(work_proc, n);
                break;

            case 3:
                if (!data_entered) {
                    printf("\n[!] Please enter process data first (Option 1).\n");
                    break;
                }
                copy_processes(original_proc, work_proc, n);
                sjf(work_proc, n);
                break;

            case 4:
                if (!data_entered) {
                    printf("\n[!] Please enter process data first (Option 1).\n");
                    break;
                }
                copy_processes(original_proc, work_proc, n);
                srtf(work_proc, n);
                break;

            case 5:
                if (!data_entered) {
                    printf("\n[!] Please enter process data first (Option 1).\n");
                    break;
                }
                copy_processes(original_proc, work_proc, n);
                fcfs(work_proc, n);

                copy_processes(original_proc, work_proc, n);
                sjf(work_proc, n);

                copy_processes(original_proc, work_proc, n);
                srtf(work_proc, n);
                break;

            case 6:
                printf("\nExiting program....\n");
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 6);

    if (original_proc) free(original_proc);
    if (work_proc) free(work_proc);

    return 0;
}
