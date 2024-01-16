# Questions

1. _Run `process-run.py` with the following flags: `-l 5:100,5:100`.  What should the CPU utilization be (e.g., the 
    percent of time the CPU is in use?).  Why do you know this?  Use the `-c` and `-p` flags to see if you were right._
    
    CPU utilization should be 100% because of the `:100` in each of the process suggests all instructions are CPU.
    ```
    ➜  python3 process-run.py -l 5:100,5:100 -cp
    Time        PID: 0        PID: 1           CPU           IOs
      1        RUN:cpu         READY             1          
      2        RUN:cpu         READY             1          
      3        RUN:cpu         READY             1          
      4        RUN:cpu         READY             1          
      5        RUN:cpu         READY             1          
      6           DONE       RUN:cpu             1          
      7           DONE       RUN:cpu             1          
      8           DONE       RUN:cpu             1          
      9           DONE       RUN:cpu             1          
     10           DONE       RUN:cpu             1          
    
    Stats: Total Time 10
    Stats: CPU Busy 10 (100.00%)
    Stats: IO Busy  0 (0.00%)
    ``` 
   
2. _Now run with these flags: `./process-run.py -l 4:100,1:0`.  These flags specify one process with 4 instructions (all
    to use the CPU), and one that simply issues an I/O and waits for it to be done.  How long does it take to complete
    both processes?  Use `-c` and `-p` to see if you were right._

    I'd expect the `4:100` process to run without issues but would think that the CPU could be idle if the I/O takes
    longer to finish than the `4:100` process.
    
    ```
    ➜ python3 process-run.py -l 4:100,1:0 -cp
    Time        PID: 0        PID: 1           CPU           IOs
      1        RUN:cpu         READY             1          
      2        RUN:cpu         READY             1          
      3        RUN:cpu         READY             1          
      4        RUN:cpu         READY             1          
      5           DONE        RUN:io             1          
      6           DONE       BLOCKED                           1
      7           DONE       BLOCKED                           1
      8           DONE       BLOCKED                           1
      9           DONE       BLOCKED                           1
     10           DONE       BLOCKED                           1
     11*          DONE   RUN:io_done             1          
    
    Stats: Total Time 11
    Stats: CPU Busy 6 (54.55%)
    Stats: IO Busy  5 (45.45%)
    ```
   
3. _Switch the order of the processes: `-l 1:0,4:100`.  What happens now?  Does switching the order matter?  Why?_

    Indeed, if we start the I/O bound process first, we get a more efficient use of the CPU  but still imperfect.
    ```
    ➜  python3 process-run.py -l 1:0,4:100 -cp
    Time        PID: 0        PID: 1           CPU           IOs
      1         RUN:io         READY             1          
      2        BLOCKED       RUN:cpu             1             1
      3        BLOCKED       RUN:cpu             1             1
      4        BLOCKED       RUN:cpu             1             1
      5        BLOCKED       RUN:cpu             1             1
      6        BLOCKED          DONE                           1
      7*   RUN:io_done          DONE             1          
    
    Stats: Total Time 7
    Stats: CPU Busy 6 (85.71%)
    Stats: IO Busy  5 (71.43%)
    ```
    if our OS is not going to have some sort of policy for which to run first, then the order does matter: starting the
    I/O-bound process first allows the CPU to run the `4:100` process while its waiting for I/O to complete.

4. _We'll now explore some other flags.  One important flag is `-S`, which determines how the system reacts when a process issues an I/O.  With the flag set to SWITCH_ON_END, the system will NOT switch to another process while one is doing I/O, instead waiting until the process is completely finished.  What happens when you run the following two processes (`-l 1:0,4:100 -c -S SWITCH_ON_END`), one doing I/O and the other doing CPU work?_

    I'd expect the CPU to be wasting time waiting for the I/O to complete rather than switching to the `4:100` process.  Indeed, this is what the output suggests:
    ```
   ➜  cpu-intro git:(master) ✗ python3 process-run.py -l 1:0,4:100 -c -S SWITCH_ON_END 
    Time        PID: 0        PID: 1           CPU           IOs
      1         RUN:io         READY             1          
      2        BLOCKED         READY                           1
      3        BLOCKED         READY                           1
      4        BLOCKED         READY                           1
      5        BLOCKED         READY                           1
      6        BLOCKED         READY                           1
      7*   RUN:io_done         READY             1          
      8           DONE       RUN:cpu             1          
      9           DONE       RUN:cpu             1          
     10           DONE       RUN:cpu             1          
     11           DONE       RUN:cpu             1  
    ```

5. _Now, run the same processes, but with the switching behavior set to switch to another process whenever one is WAITING for I/O (`-l 1:0,4:100 -c -S SWITCH_ON_IO`)  What happens now?_
    
    This should behave similar to question (3).
    ```
    ➜  cpu-intro git:(master) ✗ python3 process-run.py -l 1:0,4:100 -cp -S SWITCH_ON_IO
    Time        PID: 0        PID: 1           CPU           IOs
      1         RUN:io         READY             1          
      2        BLOCKED       RUN:cpu             1             1
      3        BLOCKED       RUN:cpu             1             1
      4        BLOCKED       RUN:cpu             1             1
      5        BLOCKED       RUN:cpu             1             1
      6        BLOCKED          DONE                           1
      7*   RUN:io_done          DONE             1          
    
    Stats: Total Time 7
    Stats: CPU Busy 6 (85.71%)
    Stats: IO Busy  5 (71.43%)
    ```
6. _One other important behavior is what to do when an I/O completes.  With `-I IO_RUN_LATER`, when an I/O cpmletes, the proces that issued it is not necessarily run right away; rather, whatever was runing at the time keeps running.  What happens when you run this combination of processes? (`./process-run.py -cp -l 3:0,5:100,5:100,5:100 -S SWITCH_ON_IO -I IO_RUN_LATER`)_

    The first process will continue running either after the second, or at the very end (don't know if the scheduler is going to choose a waiting I/O process over a ready process).  From the results it looks like the first process gets resumed after each of the other processes, so most of its IO blockage isn't affecting the CPU.  Once the CPU only can work on the I/O-bound process, then we start getting inefficient use of the CPU.
    ```
    Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
      1         RUN:io         READY         READY         READY             1          
      2        BLOCKED       RUN:cpu         READY         READY             1             1
      3        BLOCKED       RUN:cpu         READY         READY             1             1
      4        BLOCKED       RUN:cpu         READY         READY             1             1
      5        BLOCKED       RUN:cpu         READY         READY             1             1
      6        BLOCKED       RUN:cpu         READY         READY             1             1
      7*         READY          DONE       RUN:cpu         READY             1          
      8          READY          DONE       RUN:cpu         READY             1          
      9          READY          DONE       RUN:cpu         READY             1          
     10          READY          DONE       RUN:cpu         READY             1          
     11          READY          DONE       RUN:cpu         READY             1          
     12          READY          DONE          DONE       RUN:cpu             1          
     13          READY          DONE          DONE       RUN:cpu             1          
     14          READY          DONE          DONE       RUN:cpu             1          
     15          READY          DONE          DONE       RUN:cpu             1          
     16          READY          DONE          DONE       RUN:cpu             1          
     17    RUN:io_done          DONE          DONE          DONE             1          
     18         RUN:io          DONE          DONE          DONE             1          
     19        BLOCKED          DONE          DONE          DONE                           1
     20        BLOCKED          DONE          DONE          DONE                           1
     21        BLOCKED          DONE          DONE          DONE                           1
     22        BLOCKED          DONE          DONE          DONE                           1
     23        BLOCKED          DONE          DONE          DONE                           1
     24*   RUN:io_done          DONE          DONE          DONE             1          
     25         RUN:io          DONE          DONE          DONE             1          
     26        BLOCKED          DONE          DONE          DONE                           1
     27        BLOCKED          DONE          DONE          DONE                           1
     28        BLOCKED          DONE          DONE          DONE                           1
     29        BLOCKED          DONE          DONE          DONE                           1
     30        BLOCKED          DONE          DONE          DONE                           1
     31*   RUN:io_done          DONE          DONE          DONE             1          
    
    Stats: Total Time 31
    Stats: CPU Busy 21 (67.74%)
    Stats: IO Busy  15 (48.39%)
    ```

7. _Now run the same processes, but with `-I IO_RUN_IMMEDIATE` set, which immediately runs the process that issued the I/O.  How does this behavior differ?  Why might running a process that just completed an I/O again be a good idea?_

    This will get the `3:0` process started on its next I/O bound instruction earlier than before which should reduce the idling at the end of the CPU runtime seen in question (6).  So running a process that just completed an I/O can be a good idea if it has many I/O-bound instructions.
    ```
    Time        PID: 0        PID: 1        PID: 2        PID: 3           CPU           IOs
      1         RUN:io         READY         READY         READY             1          
      2        BLOCKED       RUN:cpu         READY         READY             1             1
      3        BLOCKED       RUN:cpu         READY         READY             1             1
      4        BLOCKED       RUN:cpu         READY         READY             1             1
      5        BLOCKED       RUN:cpu         READY         READY             1             1
      6        BLOCKED       RUN:cpu         READY         READY             1             1
      7*   RUN:io_done          DONE         READY         READY             1          
      8         RUN:io          DONE         READY         READY             1          
      9        BLOCKED          DONE       RUN:cpu         READY             1             1
     10        BLOCKED          DONE       RUN:cpu         READY             1             1
     11        BLOCKED          DONE       RUN:cpu         READY             1             1
     12        BLOCKED          DONE       RUN:cpu         READY             1             1
     13        BLOCKED          DONE       RUN:cpu         READY             1             1
     14*   RUN:io_done          DONE          DONE         READY             1          
     15         RUN:io          DONE          DONE         READY             1          
     16        BLOCKED          DONE          DONE       RUN:cpu             1             1
     17        BLOCKED          DONE          DONE       RUN:cpu             1             1
     18        BLOCKED          DONE          DONE       RUN:cpu             1             1
     19        BLOCKED          DONE          DONE       RUN:cpu             1             1
     20        BLOCKED          DONE          DONE       RUN:cpu             1             1
     21*   RUN:io_done          DONE          DONE          DONE             1          
    
    Stats: Total Time 21
    Stats: CPU Busy 21 (100.00%)
    Stats: IO Busy  15 (71.43%)
    ```

8. Now run with some randomly generated processes using flags `-s 1 -l 3:50,3:50` or `-s 2 -l 3:50,3:50` or `-s 3 -l 3:50,3:50`.  See if you can predict how the trace will turn out.  What happens when you use `-I IO_RUN_IMMEDIATE` versus that flag `-I IO_RUN_LATER`?  What happens when you use the flag `-S SWITCH_ON_IO` versus `-S SWITCH_ON_END`?
    
    Given that each process has a mix of I/O and CPU instructions I'd expect a less efficient result than the previous examples.  Looks like IO_RUN_IMMEDIATE is generally more efficient, and SWITCH_ON_IO is also more efficient.