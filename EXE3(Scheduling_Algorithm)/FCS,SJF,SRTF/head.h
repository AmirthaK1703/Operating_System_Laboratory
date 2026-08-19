#ifndef HEAD_H
#define HEAD_H

#include <stdbool.h>

typedef struct {
    int pid;
    float arrival_time;
    float burst_time;
    float completion_time;
    float turnaround_time;
    float waiting_time;
    float remaining_time;
} Process;

void fcfs(Process p[], int n);
void sjf(Process p[], int n);
void srtf(Process p[], int n);

void calculate_metrics(Process p[], int n);
void print_results(Process p[], int n, const char *algorithm_name);

#endif
