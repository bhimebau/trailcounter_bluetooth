/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Mon Feb  2 15:09:52 2015
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

#include "NNet.h"
#include <stdio.h>

nncfg_t ann = {3, 4, 1};

void PrintNetwork(nncfg_t *cfg){
  int i = 0;
  printf("Inputs: \n");
  for (i = 0; i < cfg->numInputs; i++){
    printf("%d\n", inputs[i]);
  }
  printf("\n");
  printf("Outputs: \n");
  for (i = 0; i < cfg->numOutput; i++){
    printf("%d\n", outputs[i]);
  }
  printf("\n");
}

int main(){
  initNetwork(&ann);

  EvaluateNet(&ann, 1);
  PrintNetwork(&ann);
  EvaluateNet(&ann, 2);
  PrintNetwork(&ann);
  EvaluateNet(&ann, 3);
  PrintNetwork(&ann);
  EvaluateNet(&ann, 4);
  PrintNetwork(&ann);
  EvaluateNet(&ann, 5);
  PrintNetwork(&ann);
  return 0;
}
/* main.c ends here */
