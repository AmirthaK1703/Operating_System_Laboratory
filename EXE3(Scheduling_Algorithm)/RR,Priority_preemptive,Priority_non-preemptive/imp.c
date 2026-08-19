#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include <limits.h>
#include "head2.h"
void calculate_metrics(Process p[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        // Safety bound adjustment for floating-point calculation precision
        if (p[i].waiting_time < 0) {
            p[i].waiting_time = 0.0f;
        }
    }
}

void print_results(Process p[], int n, const char *title) {
    float total_tat = 0.0f, total_wt = 0.0f;
    int i;

    printf("\n------------------------------------------------------------------------\n");
    printf(" %s \n", title);
    printf("------------------------------------------------------------------------\n");
    printf(" PID | Arrival | Burst | Priority | Completion | Turnaround | Waiting \n");
    printf("------------------------------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        printf("  %-2d |  %-6.1f | %-5.1f |    %-5d |   %-8.1f |    %-7.1f | %-7.1f \n",
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].completion_time, p[i].turnaround_time, p[i].waiting_time);

        total_tat += p[i].turnaround_time;
        total_wt += p[i].waiting_time;
    }

    printf("------------------------------------------------------------------------\n");
    printf("Average Turnaround Time : %.2f\n", total_tat / n);
    printf("Average Waiting Time    : %.2f\n", total_wt / n);
    printf("------------------------------------------------------------------------\n");
}
void round_robin(Process p[], int n, float time_quantum) {
    int i, j;

    // Initial sort by arrival time
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                Process temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int completed_count = 0;
    for (i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        if (p[i].burst_time <= 0.0001f) {
            p[i].remaining_time = 0.0f;
            p[i].completion_time = p[i].arrival_time;
            completed_count++;
        }
    }

    // Queue implementation for ready process tracking
    int queue[100];
    int front = 0, rear = 0;
    bool visited[100] = {false};

    float current_time = 0.0f;

    // Fast forward time to first process arrival if needed
    if (completed_count < n) {
        current_time = p[0].arrival_time;
        queue[rear++] = 0;
        visited[0] = true;
    }

    while (completed_count < n) {
        if (front == rear) { // Queue is empty (CPU idle)
            float next_arrival = FLT_MAX;
            int next_idx = -1;
            for (i = 0; i < n; i++) {
                if (p[i].remaining_time > 0.0001f && !visited[i] && p[i].arrival_time < next_arrival) {
                    next_arrival = p[i].arrival_time;
                    next_idx = i;
                }
            }
            if (next_idx != -1) {
                current_time = next_arrival;
                queue[rear++] = next_idx;
                visited[next_idx] = true;
            } else {
                break;
            }
        }

        int curr_idx = queue[front++]; // Dequeue

        float execute_time = (p[curr_idx].remaining_time < time_quantum) ? p[curr_idx].remaining_time : time_quantum;
        p[curr_idx].remaining_time -= execute_time;
        current_time += execute_time;

        // Enqueue processes that arrived during this execution window
        for (i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0.0001f && !visited[i]) {
                queue[rear++] = i;
                visited[i] = true;
            }
        }

        // Re-enqueue current process if not completed
        if (p[curr_idx].remaining_time > 0.0001f) {
            queue[rear++] = curr_idx;
        } else {
            p[curr_idx].remaining_time = 0.0f;
            p[curr_idx].completion_time = current_time;
            completed_count++;
        }
    }

    calculate_metrics(p, n);

    char title[100];
    snprintf(title, sizeof(title), "ROUND ROBIN (Time Quantum = %.2f)", time_quantum);
    print_results(p, n, title);
}

/* ==========================================================
   2. PRIORITY SCHEDULING (Non-Preemptive)
   Lower value = Higher priority
   ========================================================== */
void priority_non_preemptive(Process p[], int n) {
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
    int completed_count = 0;
    for (i = 0; i < n; i++) {
        is_completed[i] = false;
        if (p[i].burst_time <= 0.0001f) {
            is_completed[i] = true;
            p[i].completion_time = p[i].arrival_time;
            completed_count++;
        }
    }

    float current_time = 0.0f;

    while (completed_count < n) {
        int idx = -1;
        int highest_priority = INT_MAX;

        for (i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && !is_completed[i]) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    idx = i;
                } else if (p[i].priority == highest_priority) {
                    // Tie breaker: FCFS (earlier arrival time)
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
            if (next_arrival != FLT_MAX && current_time < next_arrival) {
                current_time = next_arrival;
            }
        }
    }

    calculate_metrics(p, n);
    print_results(p, n, "PRIORITY SCHEDULING (Non-Preemptive)");
}

/* ==========================================================
   3. PRIORITY SCHEDULING (Preemptive)
   Lower value = Higher priority
   ========================================================== */
void priority_preemptive(Process p[], int n) {
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

    int completed_count = 0;
    for (i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        if (p[i].burst_time <= 0.0001f) {
            p[i].remaining_time = 0.0f;
            p[i].completion_time = p[i].arrival_time;
            completed_count++;
        }
    }

    float current_time = 0.0f;
    float time_step = 0.1f;

    while (completed_count < n) {
        int idx = -1;
        int highest_priority = INT_MAX;

        for (i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && p[i].remaining_time > 0.0001f) {
                if (p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    idx = i;
                } else if (p[i].priority == highest_priority) {
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
            if (next_arrival != FLT_MAX && current_time < next_arrival) {
                current_time = next_arrival;
            } else {
                break;
            }
        }
    }

    calculate_metrics(p, n);
    print_results(p, n, "PRIORITY SCHEDULING (Preemptive)");
}
