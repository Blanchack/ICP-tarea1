#!/bin/bash

g++ -O3 -DNDEBUG -fopenmp profiling.cpp -o profiler

N=2048
B=64

echo "algo,threads,cycles,instruction,cache_references,cache_misses" > perf_results.csv

run_perf() {
	local algo=$1
	local threads=$(nproc)
	local extra_args=$3

	echo "Running $algo with $threads thread(s)..."
	export OMP_NUM_THREADS=$threads

	perf stat -e cycles,instructions,cache-references,cache-misses -x , -o temp_perf.csv ./profiler $algo $N $extra_args

	CYCLES=$(grep "cycles" temp_perf.csv | awk -F, '{print $1}')
	INSTRUCTIONS=$(grep "instructions" temp_perf.csv | awk -F, '{print $1}')
	CACHEREF=$(grep "cache-references" temp_perf.csv | awk -F, '{print $1}')
	CACHEMISS=$(grep "cache-misses" temp_perf.csv | awk -F, '{print $1}')

	echo "$algo,$threads,$CYCLES,$INSTRUCTIONS,$CACHEREF,$CACHEMISS" >> perf_results.csv
	rm temp_perf.csv
}

run_perf "classic" 1 ""
run_perf "seq_tiling" 1 "$B"
run_perf "seq_strassen" 1
run_perf "par_tiling" 2 "$B"
run_perf "par_tiling" 4 "$B"
run_perf "par_tiling" 8 "$B"
run_perf "par_tiling" 16 "$B"
run_perf "par_strassen" 16
run_perf "par_hybrid" 16

echo "Results saved in perf_results.csv"
