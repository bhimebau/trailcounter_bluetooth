// Neural Network in C
// Author: Nhan Do
// Date: 02/01/2015: initial commit

#include "NNet.h"
#include <stdio.h>

// values of hidden layers
int hiddens[MAX_HIDDEN];

// values of outputs
int outputs[MAX_OUTPUTS];

//weights of Input layer to hidden layer
int weights_ih[MAX_INPUTS][MAX_HIDDEN]; 

//weights of hidden layer to Output layer
int weights_ho[MAX_HIDDEN][MAX_OUTPUTS];

int inputIndex;

int setWeightHidden(nncfg_t *nncfg, int input, int hidden, int value) {
  if ((hidden >= nncfg->numHidden) || (input >= nncfg->numInputs)) {
    return (1);
  }
  else {
    weights_ih[input][hidden] = value;
  }
  return (0);
}

int setWeightOutput(nncfg_t *nncfg, int hidden, int output, int value) {
  if ((hidden >= nncfg->numHidden) || (output >= nncfg->numOutput)) {
    return (1);
  }
  else {
    weights_ho[hidden][output] = value;
  }
  return (0);
}

void resetOutput(nncfg_t *nncfg) {
  // reset Hidden layer and Output layer to 0;
  int i;
  for (i = 0; i < nncfg->numHidden; i++){
    hiddens[i] = 0;
  }
  for (i = 0; i < nncfg->numOutput; i++){
    outputs[i] = 0;
  }
}

void UpdateInput(int next_input, nncfg_t *nncfg){
  int i;

  for (i=(nncfg->numInputs-1); i>0 ;i--) {
    inputs[i] = inputs[i-1];
  }

  inputs[i] = next_input;
}


void initNetwork(nncfg_t *nncfg) {
  int i;

  resetOutput(nncfg);
  fifo->inputIndex = nncfg->numInputs-1;
  for (i=0;i<nncfg->numInputs;i++) fifo->inputs[i]=0;
}

void addItem(nncfg_t *nncfg, inputFifo_t *fifo, int item) {
  fifo->inputs[fifo->inputIndex] = item;
  fifo->inputIndex--;
  if (fifo->inputIndex < 0) {
    fifo->inputIndex = nncfg->numInputs-1;
  }
}

void printFifo(nncfg_t *nncfg, inputFifo_t *fifo) {
  int i; 
  
  i = fifo->inputIndex + 1;
  if (i == nncfg->numInputs) {
    i = 0;
  }
  do {
    printf("%d\n", fifo->inputs[i]);
    i++;
    if (i == nncfg->numInputs) {
      i = 0;
    }
  } while (i != fifo->inputIndex);
  printf("%d\n\n", fifo->inputs[i]);
}

void EvaluateNet(nncfg_t *nncfg, inputFifo_t *fifo, int next_input){

  int i = 0;
  int j = 0;

  // update the input FIFO
  UpdateInput(next_input, nncfg);

  /* inputs[inputIndex--] = next_input; */
  /* if (inputIndex<0) { */
  /*   inputIndex = nncfg->numInputs-1;   */
  /* } */

  resetOutput(nncfg);
  
  // propagate to the hidden layer
  for (i = 0; i < nncfg->numInputs; i++){
  
  /* i = inputIndex; */
  /* while (i != inputIndex) { */
  /*   if (i == nncfg->numInputs) { */
  /*     i = 0; */
  /*   } */
    for (j = 0; j < nncfg->numHidden; j++){
      hiddens[j] += fifo->inputs[i] * weights_ih[i][j];			
    }
    i++;
    if (i == nncfg->numInputs) {
      i = 0;
    }
  } while (i != fifo->inputIndex);
  for (j = 0; j < nncfg->numHidden; j++){
    hiddens[j] += fifo->inputs[i] * weights_ih[i][j];			
  }

  // propagate to the output layer
  for (i = 0; i < nncfg->numHidden; i++){
    for (j = 0; j < nncfg->numOutput; j++){
      outputs[j] += hiddens[i] * weights_ho[i][j];
    }
  }
}

