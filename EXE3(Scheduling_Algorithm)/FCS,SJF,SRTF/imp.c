#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include "head.h"

void calculate_metrics(Process p[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
    }
}

void print_results(Process p[], int n, const char *algorithm_name) {
    int i;
    float total_tat = 0.0f, total_wt = 0.0f;

    printf("\n========================================================================\n");
    printf(" %s SCHEDULING RESULTS \n", algorithm_name);
    printf("========================================================================\n");
    printf("%-8s | %-12s | %-12s | %-12s | %-12s | %-12s\n",
           "PID", "Arrival", "Burst", "Completion", "Turnaround", "Waiting");
    printf("------------------------------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        printf("P%-7d | %-12.2f | %-12.2f | %-12.2f | %-12.2f | %-12.2f\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);

        total_tat += p[i].turnaround_time;
        total_wt += p[i].waiting_time;
    }

    printf("------------------------------------------------------------------------\n");
    printf("Average Turnaround Time : %.2f\n", total_tat / (float)n);
    printf("Average Waiting Time    : %.2f\n", total_wt / (float)n);
    printf("========================================================================\n");
}

void fcfs(Process p[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    float current_time = 0.0f;
    for (i = 0; i < n; i++) {
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time;
        }
        current_time += p[i].burst_time;
        p[i].completion_time = current_time;
    }

    calculate_metrics(p, n);
    print_results(p, n, "FIRST-COME, FIRST-SERVED (FCFS)");
}

void sjf(Process p[], int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    bool is_completed[n];
    for (i = 0; i < n; i++) is_completed[i] = false;

    float current_time = 0.0f;
    int completed_count = 0;

    while (completed_count < n) {
        int idx = -1;
        float min_burst = FLT_MAX;

        for (i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !is_completed[i]) {
                if (p[i].burst_time < min_burst) {
                    min_burst = p[i].burst_time;
                    idx = i;
                }
                else if (p[i].burst_time == min_burst) {
                    if (idx != -1 && p[i].arrival_time < p[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            current_time += p[idx].burst_time;
            p[idx].completion_time = current_time;
            is_completed[idx] = true;
            completed_count++;
        } else {
            float next_arrival = FLT_MAX;
            for (i = 0; i < n; i++) {
                if (!is_completed[i] && p[i].arrival_time < next_arrival) {
                    next_arrival = p[i].arrival_time;
                }
            }
            current_time = next_arrival;
        }
    }

    calculate_metrics(p, n);
    print_results(p, n, "SHORTEST JOB FIRST (SJF)");
}

void srtf(Process p[], int n) {
    int i, j;


    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }


    for (i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
    }

    float current_time = 0.0f;
    int completed_count = 0;
    float time_step = 0.1f;

    while (completed_count < n) {
        int idx = -1;
        float min_remaining = FLT_MAX;

        for (i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0.0001f) {
                if (p[i].remaining_time < min_remaining) {
                    min_remaining = p[i].remaining_time;
                    idx = i;
                } else if (p[i].remaining_time == min_remaining) {
                    if (idx == -1 || p[i].arrival_time < p[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            p[idx].remaining_time -= time_step;
            current_time += time_step;

            if (p[idx].remaining_time <= 0.0001f) {
                p[idx].remaining_time = 0.0f;
                p[idx].completion_time = current_time;
                completed_count++;
            }
        } else {
            float next_arrival = FLT_MAX;
            for (i = 0; i < n; i++) {
                if (p[i].remaining_time > 0.0001f && p[i].arrival_time < next_arrival) {
                    next_arrival = p[i].arrival_time;
                }
            }
            current_time = next_arrival;
        }
    }

    calculate_metrics(p, n);
    print_results(p, n, "SHORTEST REMAINING TIME FIRST (SRTF)");
}
