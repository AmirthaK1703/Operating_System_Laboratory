#ifndef HEAD_H
#define HEAD_H

typedef struct {
    int pid;
    float arrival_time;
    float burst_time;
    int priority;
    float remaining_time;
    float completion_time;
    float turnaround_time;
    float waiting_time;
} Process;

void copy_processes(Process src[], Process dest[], int n);
void input_processes(Process p[], int n);

void round_robin(Process p[], int n, float time_quantum);
void priority_non_preemptive(Process p[], int n);
void priority_preemptive(Process p[], int n);

void calculate_metrics(Process p[], int n);
void print_results(Process p[], int n, const char *title);

#endif
