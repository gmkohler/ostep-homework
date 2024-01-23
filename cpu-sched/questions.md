# Homework (Simulation)

This program, `scheduler.py`, allows you to see how different schedulers perform under scheduling metrics such as response time, turnaround time, and total wait time.  See the README for details.

## Questions

### 1. Compute the response and turnaround time when running three jobs of length 200 with the SJF and FIFO schedules.
Given the jobs have the same length I would expect the schedulers to behave similarly

#### SJF
Job 0: Turnaround time 200 (it starts first), wait time 0 (it starts first), response time 0 (it starts first)

Job 1: Turnaround time 400 (it starts second), wait time 200 (it waits for job 0), response time 200 (once it starts)

Job 2: Turnaround time 600 (starts last), wait time 400 (it waits for jobs 0 and 1), response time 400 (once it starts)

#### FIFO

Same results (confirmed with `diff` of the `-c` output)

### 2. Now do the same but with jobs of different lengths: 100, 200, and 300.

#### SJF

Job 0: Turnaround time 100 (it starts first), wait time 0 (it starts first), response time 0 (it starts first)

Job 1: Turnaround time 300 (it starts after job 0), wait time 100 (it waits for job 0), response time 100 (it starts after job 0)

Job 2: Turnaround time 600 (starts after jobs 0,1), wait time 300 (waits for jobs 0,1) response time 300 (it starts after jobs 0,1)

#### FIFO

same results given the jobs were inserted in shortest-job order (confirmed with `diff` of the `-c` output).

### 3. Now do the same, but also with the RR scheduler and a time-slice of 1.

#### response times
Response times will be 0,1,2 because each job gets one instruction executed to start.


#### wait times
Job 0 will wait for ~200 instructions of the other jobs to be run before its last instruction runs

Job 1 will need to wait 200 instructions while all 3 jobs receive work, then 100 more while it competes with job 2, so ~300 instructions wait time

Job 2 will need to wait 200 instructions while all 3 jobs are being scheduled, then 100 more while it competes with job 1, so ~300 instructions wait time

#### turnaround times

Job 0 turnaround will increase by ~3x compared to its length because of the other two jobs being run (~300 instructions).

Job 1 turnaround will be its length plus the wait time of 300, so ~500 instructions of time.

Job 2 turnaround will be its length plus the wait time of ~300, so ~600 instructions of time.

### 4. For what types of workloads does SJF deliver the same turnaround times as FIFO?

If the jobs are equal length, or they arrive in "sorted order" (shortest first), then the two schedulers will behave the same with respect to turnaround time.

### 5. For what types of workloads and quantum lengths does SJF delivery the same response times as RR?

Round Robin can be configured to behave like a SJF scheduler if the jobs are of equal length and the RR quantum length is equal to that job length.  Under these conditions, RR won't actually be interrupting the jobs.

### 6. What happens to the response time with SJF as job lengths increase?  Can you use the simulator  to demonstrate the trend?

Average response times will increase as job lengths increase because they're all waiting.

### 7. What happens to response time with RR as quantum lengths increase?  Can you write an equation that gives the worst-case response times, given N jobs?

Response times will increase because the waiting jobs have to wait longer to start.
