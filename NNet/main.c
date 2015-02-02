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

void PrintNetwork(){
  int i = 0;
  printf("Inputs: \n");
  for (i = 0; i < NUM_INPUT_NODE; i++){
    printf("%d\n", inputs[i]);
  }
  printf("\n");
  printf("Outputs: \n");
  for (i = 0; i < NUM_OUTPUT_NODE; i++){
    printf("%d\n", outputs[i]);
  }
  printf("\n");
}

/* void PrintInput() { */
/*   int i = 0; */
/*   printf("Print input values: \n"); */
/*   for (i = 0; i < NUM_INPUT_NODE; i++){ */
/*     printf("%d %d\n", i, inputs[i]); */
/*   } */
/*   printf("\n"); */
/* } */


int main(){

  EvaluateNet(1);
  PrintNetwork();
  EvaluateNet(2);
  PrintNetwork();
  EvaluateNet(3);
  PrintNetwork();
  EvaluateNet(4);
  PrintNetwork();
  EvaluateNet(5);
  PrintNetwork();

  /* PrintOutput(); */

  /* EvaluateNet(20); */
  /* PrintOutput(); */

  /* EvaluateNet(30); */
  /* PrintOutput(); */
  return 0;
}

/* main.c ends here */
