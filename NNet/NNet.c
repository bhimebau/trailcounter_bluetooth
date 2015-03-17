/* NNet.h --- 
 * 
 * Filename: NNet.h
 * Description: 
 * Author: Nhan Do, Bryce Himebaugh
 * Maintainer: 
 * Created: Mon Feb  2 15:02:44 2015
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* Copyright (c) 2014-2015 Analog Computing Solutions  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */

#include <weights.h>
#include "NNet.h"
#include <stdio.h>

float inputs[MAX_INPUTS];
float hiddens[MAX_HIDDEN];
float outputs[MAX_OUTPUTS];

void EvaluateNet(float next_input){

  int i = 0;
  int j = 0;

  for (i=(MAX_INPUTS-1); i>0 ;i--) {
    inputs[i] = inputs[i-1];
  }
  inputs[i] = next_input;

  for (i = 0; i < MAX_HIDDEN; i++){
    hiddens[i] = 0;
  }
  for (i = 0; i < MAX_OUTPUTS; i++){
    outputs[i] = 0;
  }

  for (i = 0; i < MAX_INPUTS; i++){
    for (j = 0; j < MAX_HIDDEN; j++){
      hiddens[j] += inputs[i] * weights_ih[i][j];			
      if (hiddens[j] > 1) {
      	hiddens[j] = 1;
      }
      else if (hiddens[j] < 0) {
      	hiddens[j] = 0;
      }
    }
  }

  for (i = 0; i < MAX_HIDDEN; i++){
    for (j = 0; j < MAX_OUTPUTS; j++){
      outputs[j] += hiddens[i] * weights_ho[i][j];
      if (outputs[j] > 1) {
      	outputs[j] = 1;
      }
      else if (outputs[j] < 0) {
      	outputs[j] = 0;
      }
    }
  }
}



