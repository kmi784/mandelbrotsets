import time
from subprocess import run, DEVNULL

def benchmark(func):
    def wrapper(num_threads, warm_up, repeat):
        for _ in range(warm_up):
            func(num_threads)

        dt = []
        for _ in range(repeat):
            t_start = time.perf_counter()
            func(num_threads)
            t_end = time.perf_counter()
            dt.append(t_end - t_start)

        return dt
    
    return wrapper

@benchmark
def run_c(num_threads):
    return run(["../bin/benchmark", f"{num_threads}"], check=True, stdout=DEVNULL)

@benchmark
def run_py(num_processes):
    return run(["./py_benchmark.py", f"{num_processes}"], check=True, stdout=DEVNULL)

@benchmark
def run_cpy(num_processes):
    return run(["./cpy_benchmark.py", f"{num_processes}"], check=True, stdout=DEVNULL)





