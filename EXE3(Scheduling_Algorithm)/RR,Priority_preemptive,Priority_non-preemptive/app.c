#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "head2.h"

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
        printf("\nProcess P%d Arrival Time: ", i + 1);
        scanf("%f", &p[i].arrival_time);
        printf("Process P%d Burst Time  : ", i + 1);
        scanf("%f", &p[i].burst_time);
        printf("Process P%d Priority    : ", i + 1);
        scanf("%d", &p[i].priority);
    }
}

int main() {
    int n = 0;
    int choice;
    float time_quantum = 2.0f;
    bool data_entered = false;

    Process *original_proc = NULL;
    Process *work_proc = NULL;

    do {
        printf("\n=========================================\n");
        printf("         CPU SCHEDULING SIMULATOR        \n");
        printf("=========================================\n");
        printf(" 1. Enter Process Data\n");
        printf(" 2. Round Robin (RR)\n");
        printf(" 3. Priority (Non-Preemptive)\n");
        printf(" 4. Priority (Preemptive)\n");
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
                if (!data_entered) { printf("\n[!] Enter data first (Option 1).\n"); break; }
                printf("Enter Time Quantum for Round Robin: ");
                scanf("%f", &time_quantum);
                copy_processes(original_proc, work_proc, n);
                round_robin(work_proc, n, time_quantum);
                break;

            case 3:
                if (!data_entered) { printf("\n[!] Enter data first (Option 1).\n"); break; }
                copy_processes(original_proc, work_proc, n);
                priority_non_preemptive(work_proc, n);
                break;

            case 4:
                if (!data_entered) { printf("\n[!] Enter data first (Option 1).\n"); break; }
                copy_processes(original_proc, work_proc, n);
                priority_preemptive(work_proc, n);
                break;

            case 5:
                if (!data_entered) { printf("\n[!] Enter data first (Option 1).\n"); break; }
                printf("Enter Time Quantum for Round Robin: ");
                scanf("%f", &time_quantum);


                copy_processes(original_proc, work_proc, n);
                round_robin(work_proc, n, time_quantum);

                copy_processes(original_proc, work_proc, n);
                priority_non_preemptive(work_proc, n);

                copy_processes(original_proc, work_proc, n);
                priority_preemptive(work_proc, n);
                break;

            case 6:
                printf("\nExiting program. Goodbye!\n");
                break;

            default:
                printf("\nInvalid choice! Pick between 1 and 9.\n");
        }

    } while (choice != 6);

    if (original_proc) free(original_proc);
    if (work_proc) free(work_proc);

    return 0;
}


