# Questions

1. _Run `./fork.py -s 10` and see which actions are taken.  Can you predict what the process tree looks like at each step?  Use the `-c` flag to check your answers.  Try some different random seeds (`-s`) or add more actions (`-a`) to get the hang of it._
    ```
    ARG seed 10
    ARG fork_percentage 0.7
    ARG actions 5
    ARG action_list
    ARG show_tree False
    ARG just_final False
    ARG leaf_only False
    ARG local_reparent False
    ARG print_style fancy
    ARG solve False

                               Process Tree:
                                   a

    Action: a forks b
    Process Tree?
         a
         └── b
    Action: a forks c
    Process Tree?
         a
         ├── b
         └── c
    Action: c EXITS
    Process Tree?
         a
         └── b
    Action: a forks d
         a
         ├── b
         └── d
    Process Tree?
    Action: a forks e
         a
         ├── b
         ├── d
         └── e
    Process Tree?
    ```
   ```
   ARG seed 142
   ARG fork_percentage 0.7
   ARG actions 5
   ARG action_list
   ARG show_tree False
   ARG just_final False
   ARG leaf_only False
   ARG local_reparent False
   ARG print_style fancy
   ARG solve False

                              Process Tree:
                                  a

   Action: a forks b
   Process Tree?
                                  a
                                  └── b
   Action: a forks c
   Process Tree?
                                  a
                                  ├── b
                                  └── c
   Action: c forks d
                                  a
                                  ├── b
                                  └── c
                                      └── d
   Process Tree?
   Action: a forks e
                                  a
                                  ├── b
                                  └── c
                                  │   └── d
                                  └── e
   Process Tree?
   Action: c forks f
                                  a
                                  ├── b
                                  └── c
                                  │  ├── d
                                  │  └── f
                                  └── e
   Process Tree?
   ```
2. _One control the simulator gives you is the `fork_percentage`, controlled by the `-f` flag.  The higher it is, the more likely the next action is a fork; the lower it is, the more likely the action is an exit.  Run the simulator with a large number of actions (e.g., `-a 100`) and vary the `fork_percentage` from `0.1` to `0.9`.  What do you think the resulting final process trees will look like as the percentage changes?_
I predict higher fork percentage will result in larger trees.  Not going to print the output but visually my prediction is confirmed.

3. Now, switch the output by using the `-t` flag.  Given a set of process trees, can you tell which actions were taken?
   ```
   ARG seed 20
   ARG fork_percentage 0.7
   ARG actions 5
   ARG action_list
   ARG show_tree True
   ARG just_final False
   ARG leaf_only False
   ARG local_reparent False
   ARG print_style fancy
   ARG solve False

                              Process Tree:
                                  a

   Action?
   a forks b
                                  a
                                  └── b
   Action?
   b EXITS
                                  a
   Action?
   a forks c
                                  a
                                  └── c
   Action?
   c forks d
                                  a
                                  └── c
                                      └── d
   Action?
   a forks e
                                  a
                                  ├── c
                                  │   └── d
                                  └── e
   ```

4. _One interesting thing to note is what happens when a child exits; what happens to its children in the process tree?  To study, let's create a specific example: `./fork.py -A a+b,b+c,c+d,c+e,c-`.  What do you think the process tree should look like after the exit? What if you use the -R flag?  Learn more about what happens to orphaned processes on your own to add more context._
I expect the children to die without the -R flag.  In reality, they are adopted by the main process (`a`).  I would suspect the `-R` flag would give the children to `b` (confirmed).

5. One last flag to explore is the `-F` flag, which skips intermediate steps and only asks to fill in the final process tree.  Run `./fork.py -F` and see if you can write down the final tree by looking at the series of actions generated.  Use different random seeds to try this a few times.
   ```
   ARG seed 1
   ARG fork_percentage 0.7
   ARG actions 5
   ARG action_list
   ARG show_tree False
   ARG just_final True
   ARG leaf_only False
   ARG local_reparent False
   ARG print_style fancy
   ARG solve False

                              Process Tree:
                                  a

   Action: a forks b
   Action: a forks c
   Action: c forks d
   Action: a forks e
   Action: c EXITS

                           Final Process Tree?
                                     a
                                     ├── b
                                     ├── d
                                     └── e
   ```

   ```
   ARG seed 122
   ARG fork_percentage 0.7
   ARG actions 5
   ARG action_list
   ARG show_tree False
   ARG just_final True
   ARG leaf_only False
   ARG local_reparent False
   ARG print_style fancy
   ARG solve False

                              Process Tree:
                                  a

   Action: a forks b
   Action: b forks c
   Action: c EXITS
   Action: b forks d
   Action: b EXITS

                           Final Process Tree?
                                  a
                                  └── d
   ```
6. Finally, use both -t and -F together. This shows the final process
tree, but then asks you to fill in the actions that took place. By looking at the tree, can you determine the exact actions that took place?  In which cases can you tell? In which can’t you tell? Try some different random seeds to delve into this question

I don't think it's possible to tell the actions from the tree because exiting a process removes knowledge of where its children come from.