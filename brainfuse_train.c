/*
Adapted from https://code.google.com/p/fann-excel/source/browse/trunk/standAloneFANN/standAlone.cpp?r=20
*/

#include <fann.h>
#include <stdio.h>
#include <unistd.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main(int argc, char *argv[])
{
  // Check input consistency
  if (argc < 3)
    {
      printf("Usage: %s ANN_file training_file [validation_file [maxEpochs]]\n", argv[0]);
      return -1;
    }

  // Read in input parameters
  unsigned int maxEpochs = 100;
  int i;
  const char* annFile = argv[1];
  const char* trainFile = argv[2];
  const char* validFile = argv[3];
  struct fann_train_data* trainData;
  struct fann_train_data* validData;
  struct fann* ann;
  float trainError;
  float validError;
  float bestValidError;

  if (argc < 3)
    validFile = trainFile;
  if (argc > 4)
    maxEpochs = atoi(argv[4]);

  // Load the network form the file
  printf("Reading network file %s\n", annFile);
  ann = fann_create_from_file(annFile);
  if (ann == NULL)
    return -1;

  printf("Read train from file %s\n", trainFile);
  trainData = fann_read_train_from_file(trainFile);

  printf("Read valid from file %s\n", validFile);
  validData=fann_read_train_from_file(validFile);

  for(i = 1; i <= maxEpochs; i++)
  {
      trainError = fann_train_epoch(ann, trainData);

      trainError = fann_test_data(ann, trainData);
      validError = fann_test_data(ann, validData);

      if ((i==1) | (validError<bestValidError)){
        bestValidError=validError;
        fann_save(ann, annFile);
        printf("*");
      }else{
        printf(" ");
      }

      printf("Epochs     %8d. E_train: %.10f     E_valid: %.10f\n", i, trainError, validError);
  }
        
  return 0;
}
