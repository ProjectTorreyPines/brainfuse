#export FANN_ROOT=../../fann
gcc -o brainfuse_run.exe brainfuse_run.c -O3 -L $FANN_ROOT/lib -I $FANN_ROOT/include/ -lfloatfann -lm -Wl,--rpath=$FANN_ROOT/lib
gcc -o brainfuse_train.exe brainfuse_train.c -O3 -L $FANN_ROOT/lib -I $FANN_ROOT/include/ -lfloatfann -lm -Wl,--rpath=$FANN_ROOT/lib
ln -sf brainfuse_run.exe run.exe
ln -sf brainfuse_train.exe train.exe
cp brainfuse_run.* ../../neural/
cp brainfuse_train.* ../../neural/
