# for thread in 1 2 4 8
# do
# #   for size in 131 395 662 984 1376
#   for size in 1376
#   do
#     export RESULT=$(gcc -std=c11 -fopenmp -Ofast -march=native main.c -I. && ./a.out 16 $thread $size < ../data/large/$size.tsp && rm a.out)
#     export FILENAME="${thread}_${size}.out"
#     echo $RESULT > ./results/thread_vs_time_size/$FILENAME
#   done
# done

for tries in 1 2 4 8 16 32 64
do
  for size in 1376
  do
    export RESULT=$(gcc -std=c11 -fopenmp -Ofast -march=native main.c -I. && ./a.out $tries 8 $size < ../data/large/$size.tsp && rm a.out)
    export FILENAME="${tries}_${size}.out"
    echo $RESULT > ./results/tries_vs_precision_size/$FILENAME
  done
done
