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
      printf("Usage: %s ANN_file trainingFile [validationFile] [maxEpochs] [weightDecay] [springDecay]\n", argv[0]);
      return -1;
    }

  // Read in input parameters
  int maxEpochs = -100;
  int i, totEpochs;
  const char* annFile = argv[1];
  const char* trainFile = argv[2];
  const char* validFile = argv[3];
  struct fann_train_data* trainData;
  struct fann_train_data* validData;
  struct fann* ann;
  float trainError;
  float bestTrainError;
  float validError;
  float bestValidError;
  float weightDecay = 0.1;
  float springDecay = 1.;

  if (argc < 3)
    validFile = trainFile;
  if (argc > 4)
    maxEpochs = atoi(argv[4]);
  if (argc > 5)
    weightDecay = atoi(argv[5]);
  if (argc > 6)
    springDecay = atoi(argv[6]);

  printf("max epochs: %d  weight decay: %3.3f\n",maxEpochs,weightDecay);

  // Load the network form the file
  printf("Reading network file %s\n", annFile);
  ann = fann_create_from_file(annFile);
  if (ann == NULL)
    return -1;

  printf("Read train from file %s\n", trainFile);
  trainData = fann_read_train_from_file(trainFile);

  printf("Read valid from file %s\n", validFile);
  validData=fann_read_train_from_file(validFile);

  fann_set_quickprop_decay(ann,1);

  i = totEpochs = 1;
  while (i<=abs(maxEpochs)) {
      trainError = fann_train_epoch(ann, trainData);

      trainError = fann_test_data(ann, trainData);
      validError = fann_test_data(ann, validData);

      if (i==1) {
        bestValidError=validError;
        bestTrainError=trainError;
      }

      if ((i==1)|(validError<bestValidError)){
        bestValidError=validError;
        fann_save(ann, annFile);
        printf("*");
        if (maxEpochs<0){
            i=1;
        }
      }else{
        printf(" ");
      }
      if ((i==1)|(trainError<bestTrainError)){
          bestTrainError=trainError;
      }

      fann_set_quickprop_decay(ann, (1-MIN(1.0,MAX(1-1E-3,1+(trainError-validError)*(trainError/(bestValidError*bestTrainError))*1E-6)))*1E6 / trainData->num_data * 1000 * springDecay + weightDecay );

      if (maxEpochs>0) {
        printf("Epochs:%8d  t:%.10f  v:%.10f  v_rel:%.10f  decay:%g\n", totEpochs, trainError, validError, validError/bestValidError, fann_get_quickprop_decay(ann) );
      }else{
        printf("Epochs(%.2f):%8d t:%.10f  v:%.10f  v_rel:%.10f  decay:%g\n", (float)i/(float)abs(maxEpochs), totEpochs, trainError, validError, validError/bestValidError, fann_get_quickprop_decay(ann) );
      }
      fflush(stdout);

      i++;
      totEpochs++;
  }

  return 0;
}
