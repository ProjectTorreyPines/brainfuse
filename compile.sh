export FANN_ROOT=/Users/meneghini/Coding/fann
gcc -c brainfuse_lib.c -O3 -L $FANN_ROOT/src -I $FANN_ROOT/src/include/ -lfloatfann -lm
gcc -o run.exe brainfuse_run.c brainfuse_lib.o -O3 -L $FANN_ROOT/src -I $FANN_ROOT/src/include/ -lfloatfann -lm
gcc -o train.exe brainfuse_train.c -O3 -L $FANN_ROOT/src -I $FANN_ROOT/src/include/ -lfloatfann -lm
