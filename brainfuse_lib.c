#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "doublefann.h"

fann_type torben(fann_type m[], int n)
{
    int         i, less, greater, equal;
    fann_type  min, max, guess, maxltguess, mingtguess;

    min = max = m[0] ;
    for (i=1 ; i<n ; i++) {
        if (m[i]<min) min=m[i];
        if (m[i]>max) max=m[i];
    }

    while (1) {
        guess = (min+max)/2;
        less = 0; greater = 0; equal = 0;
        maxltguess = min ;
        mingtguess = max ;
        for (i=0; i<n; i++) {
            if (m[i]<guess) {
                less++;
                if (m[i]>maxltguess) maxltguess = m[i] ;
            } else if (m[i]>guess) {
                greater++;
                if (m[i]<mingtguess) mingtguess = m[i] ;
            } else equal++;
        }
        if (less <= (n+1)/2 && greater <= (n+1)/2) break ; 
        else if (less>greater) max = maxltguess ;
        else min = mingtguess;
    }
    if (less >= (n+1)/2) return maxltguess;
    else if (less+equal >= (n+1)/2) return guess;
    else return mingtguess;
}

int run_net_on_data_(int * num_data, int * num_input, int * num_output, double * input, double * output, char *basepath, int *readInfo, char *names, int *debug){
  struct fann **ann;
  fann_type *calc_in, *calc_out, *calc_tmp, *calc_med, *ihs_in_lmbda, *ihs_in_shift, *ihs_out_lmbda, *ihs_out_shift;
  float tmp;
  int i, j, n, num_net;
  char path[256],dummy[100000];
  char *result = NULL;
  FILE *fp;

  //automatic detection of available multi-networks
  num_net=0;
  while(1){
    if (*debug)
      sprintf(path,"%s_%d",basepath,num_net);
    if (access(path,R_OK)==0)
      num_net+=1;
    else
      break;
  }
  if (num_net==0) num_net=1;

  //load the networks
  ann  = (struct fann **)malloc(num_net  * sizeof(struct fann*));
  for(n=0; n < num_net; n++){

    //if there is a single network, use basename
    if (*debug){
      if (num_net>1)
	sprintf(path,"%s_%d",basepath,n);
      else
	sprintf(path,"%s",basepath);
    }

    if (n==0){
      //////////////////////////////
      if (*debug)
	if (*readInfo!=0)
	  printf(" BRAINFUSE info from %s\n",path);
      //////////////////////////////
      
      //skip additional information
      fp = fopen(path,"r");
      for(i = 0; i < 44; i++){
	fgets(dummy,100000,fp);
      }

      //input_names
      fgets(dummy,100000,fp);
      *(dummy+strlen(dummy)-1)=(char)NULL;
      strcpy(dummy,dummy+12);
      strcpy(names,dummy);
      result=strtok(dummy," ");
      for(i = 0; result!=NULL; i++){
	result=strtok(NULL," ");
      }
      *num_input=i;
      
      //output_names
      fgets(dummy,100000,fp);
      *(dummy+strlen(dummy)-1)=(char)NULL;
      strcpy(dummy,dummy+13);
      strcat(names," ");
      strcat(names,dummy);
      strcat(names," ");
      result=strtok(dummy," ");
      for(i = 0; result!=NULL; i++){
	result=strtok(NULL," ");
      }
      *num_output=i;

      //stop if only reading of names
      if (*readInfo==1)
	return 0;

      //////////////////////////////
      if (*debug){
	printf("- DataI/O: n=%d in=%d out=%d precision=%ld\n",*num_data,*num_input,*num_output,sizeof(fann_type));
	printf("- Initialize\n");
      }
      //////////////////////////////

      //allocate and init
      calc_in  = (fann_type *)malloc(*num_input * sizeof(fann_type));
      calc_tmp = (fann_type *)malloc(num_net * *num_output  * sizeof(fann_type));
      calc_med = (fann_type *)malloc(num_net * sizeof(fann_type));
      ihs_in_lmbda = (fann_type *)malloc(*num_input * sizeof(fann_type));
      ihs_in_shift = (fann_type *)malloc(*num_input * sizeof(fann_type));
      ihs_out_lmbda = (fann_type *)malloc(*num_output * sizeof(fann_type));
      ihs_out_shift = (fann_type *)malloc(*num_output * sizeof(fann_type));
      for(i = 0; i < *num_data * *num_output; i++)
	output[i]=0.;
      
      //ihs_in_lmbda
      fgets(dummy,100000,fp);
      *(dummy+strlen(dummy)-1)=(char)NULL;
      strcpy(dummy,dummy+13);
      result=strtok(dummy," ");
      for(i = 0; result!=NULL; i++){
	sscanf(result,"%f",&tmp);
	ihs_in_lmbda[i]=tmp;
	//printf("%f \n",ihs_in_lmbda[i]);
	result=strtok(NULL," ");
      }

      //ihs_in_shift
      fgets(dummy,100000,fp);
      *(dummy+strlen(dummy)-1)=(char)NULL;
      strcpy(dummy,dummy+13);
      result=strtok(dummy," ");
      for(i = 0; result!=NULL; i++){
	sscanf(result,"%f",&tmp);
	ihs_in_shift[i]=tmp;
	//printf("%f \n",ihs_in_shift[i]);
	result=strtok(NULL," ");
      }

      //ihs_out_lmbda
      fgets(dummy,100000,fp);
      *(dummy+strlen(dummy)-1)=(char)NULL;
      strcpy(dummy,dummy+14);
      result=strtok(dummy," ");
      for(i = 0; result!=NULL; i++){
	sscanf(result,"%f",&tmp);
	ihs_out_lmbda[i]=tmp;
	//printf("%f \n",ihs_out_lmbda[i]);
	result=strtok(NULL," ");
      }

      //ihs_out_shift
      fgets(dummy,100000,fp);
      *(dummy+strlen(dummy)-1)=(char)NULL;
      strcpy(dummy,dummy+14);
      result=strtok(dummy," ");
      for(i = 0; result!=NULL; i++){
	sscanf(result,"%f",&tmp);
	ihs_out_shift[i]=tmp;
	//printf("%f \n",ihs_out_shift[i]);
	result=strtok(NULL," ");
      }

      fclose(fp);

    }

    //////////////////////////////
    if (*debug) printf("- Network from %s\n",path);
    //////////////////////////////

    ann[n] = fann_create_from_file(path);
    if(!ann[n]){
      printf("Error creating ann --- ABORTING.\n");
      return -1;
    }

  }

  //////////////////////////////
  if (*debug) printf("- Calculate\n");
  //////////////////////////////
  //for each data point
  for(i = 0; i < *num_data; i++){

    //for each network
    for(n=0; n < num_net; n++){
      //load unscaled input data
      for(j = 0; j < *num_input; j++)
	calc_in[j]=(fann_type)input[i + j * *num_data];
      //ihs
      for(j = 0; j < *num_input; j++)
	if (ihs_in_lmbda[j]==0)
	  calc_in[j]=(fann_type)(calc_in[j]-ihs_in_shift[j]);
	else
	  calc_in[j]=(fann_type)(asinh(ihs_in_lmbda[j]*(calc_in[j]-ihs_in_shift[j]))/ihs_in_lmbda[j]);
      //scale in
      fann_scale_input(ann[n],calc_in);
      //compute
      calc_out = fann_run(ann[n],calc_in);
      //scale out
      fann_descale_output(ann[n],calc_out);
      //iihs
      for(j = 0; j < *num_output; j++){
	if (ihs_out_lmbda[j]==0)
	  calc_out[j]=(fann_type)(calc_out[j]+ihs_out_shift[j]);
	else
	  calc_out[j]=(fann_type)(sinh(ihs_out_lmbda[j]*calc_out[j])/ihs_out_lmbda[j]+ihs_out_shift[j]);
      }
      //store result for all ann
      for(j = 0; j < *num_output; j++){
	calc_tmp[n + j * num_net]=(double)calc_out[j];
      }
    }

    //calculate the median or the mean
    for(j = 0; j < *num_output; j++){
      calc_out[j]=0.;
    }
    for(j = 0; j < *num_output; j++){
      for(n=0; n < num_net; n++){
	calc_med[n]=calc_tmp[n + j * num_net];
	//printf("%f\n",calc_med[n]);
      }
      if (0){
	//median
	calc_out[j]=torben(calc_med,num_net);
      }
      else{
	//mean
	for(n=0; n < num_net; n++){
	  calc_out[j]+=calc_med[n]/num_net;
	}
      }
      //if (*debug) printf("%f,%f,%f,%f\n",calc_out[0],calc_out[1],calc_out[2],calc_out[3]);
    }
    //if (*debug) printf("\n");

    //save
    for(j = 0; j < *num_output; j++){
      output[i + j * *num_data]=(double)calc_out[j];
    }
  }
    
  //////////////////////////////
  if (*debug) printf("- Cleanup\n");
  //////////////////////////////
  for(n=0; n < num_net; n++){
    fann_destroy(ann[n]);
  }

  free(calc_in);
  free(calc_tmp);
  free(calc_med);

  //////////////////////////////
  if (*debug>1) {
    for(i = 0; i < *num_data; i++){
      for( j = 0; j < *num_output; j++){
	printf("%f ",output[i + j * *num_data]);
      }
      printf("\n");
    }
  }
  //////////////////////////////

  return 0;
}

