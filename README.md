# Weighted Round-Robin (WRR) Scheduler

## Build Commands
### Kernel
```shell
sudo ./build-rpi3.sh
sudo ./setup-images.sh
```

### Test Code
```shell
test/compile-mount-and-copy.sh
```

## WRR Scheduler Class


## Load Balancing


## Turnaround Time Test

- We take prime number `300000007` to prime factorization target number 
- All test repeated 5 times and we measured average of the execution time.

First, we executed prime factorization `test` alone. As a result, because there's rare interrupt to test, execution time was around 1.9 secs regardless of task weight

So, we decided to run spinning `dummy` tasks at background to all `CPU`s. Then run `test` and measured execution time. we did 3 trials with different `dummy` weight, 1, 10 and 20.

The graph below shows excution time based on task weight.
- The color of graph is differentiated by weight of dummy task.

<img width="789" alt="KakaoTalk_Photo_2023-05-27-19-01-36" src="https://github.com/swsnu/project-3-hello-scheduler-team-6/assets/69378301/787d09d7-8d37-44c7-8548-32e1e0a77521">
