*This project has been created as part of the 42 curriculum by knomura.*

# philo — Dining Philosophers

## Description

Philosophers sit around a table. Each philosopher eats, sleeps, and thinks in a loop. A fork is placed between each pair of philosophers, and eating requires both forks.

The goal is to simulate this using threads and mutexes — no deadlock, no data race, no starvation.

- Each philosopher is a thread. Each fork is a mutex.
- A monitor thread checks if any philosopher has starved.
- Philosophers take forks in different order based on their ID (even/odd) to avoid deadlock.
- The simulation stops when a philosopher dies or all philosophers have eaten enough times.

## Instructions

### Compilation

```sh
cd philo
make
```

### Execution

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

All time values are in milliseconds.

```sh
./philo 5 800 200 200
./philo 5 800 200 200 7
```

### Other targets

```sh
make clean   # Remove object files
make fclean  # Remove object files and binary
make re      # Full rebuild
make san     # Build with ThreadSanitizer
```

## Resources

- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [POSIX Threads — LLNL Tutorial](https://hpc-tutorials.llnl.gov/posix/)
- [pthread_mutex_lock(3) — Linux man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)

### AI usage

ChatGPT was used for:

- Understanding and confirming how standard library functions (e.g. `pthread_mutex_lock`, `usleep`, `gettimeofday`) work.

All implementation was written and verified by the author.
