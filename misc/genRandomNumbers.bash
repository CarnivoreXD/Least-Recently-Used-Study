#!/usr/bin/env bash
# gen_pairs.sh — generate N lines: "key id"
# Usage: ./gen_pairs.sh [N=100] [MAX_KEY=1000000] [SEED]
# Example: ./gen_pairs.sh 100 1000000 42 > input.txt

N=${1:-100}
MAX=${2:-10000}
SEED=${3:-}

awk -v N="$N" -v MAX="$MAX" -v SEED="$SEED" 'BEGIN {
  if (SEED=="") SEED = 1200
  srand(SEED);
  for (i = 0; i < N; i++) {
    key = int(rand() * MAX);
    printf "%d %d\n", key, i;         # "key id" per line
  }
}'
