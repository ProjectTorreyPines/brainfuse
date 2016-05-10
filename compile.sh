export FANN_ROOT=../fann
gcc -o brainfuse_run.exe brainfuse_run.c -O3 -L $FANN_ROOT/src -I $FANN_ROOT/src/include/ -lfloatfann -lm
gcc -o brainfuse_train.exe brainfuse_train.c -O3 -L $FANN_ROOT/src -I $FANN_ROOT/src/include/ -lfloatfann -lm
ln -sf brainfuse_run.exe run.exe
ln -sf brainfuse_train.exe train.exe
cp brainfuse_run.* ../neural/
