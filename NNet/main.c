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
#include <nntest.h>
#include <stdio.h>

inputFifo_t iFifo;

int main(){
  int i;
  float a2d_resolution; 
  float a2d_counts;
  initNetwork(&ann,&iFifo);
  
  a2d_resolution = 3.0/4095.0;

  for (i=0;i<32;i++) {
    a2d_counts = nntest_data_sampled[i]/a2d_resolution;
    EvaluateNet(&ann,&iFifo,scale_input((int) a2d_counts));
    printf("%d %f 0x%03x %f %f\n", i, nntest_data[i], (unsigned int) a2d_counts, scale_input((int)a2d_counts), outputs[0]);
  }
  for (i=32;i<42;i++) {
    EvaluateNet(&ann,&iFifo,scale_input((int) 0x000));
    printf("%d %f 0x%03x %f %f\n",i, nntest_data[i], (unsigned int) a2d_counts, scale_input((int)0x000), outputs[0]);
  }
    
  /* EvaluateNet(&ann,&iFifo,2); */
  /* EvaluateNet(&ann,&iFifo,3); */
  /* EvaluateNet(&ann,&iFifo,4); */
  /* EvaluateNet(&ann,&iFifo,5); */
  /* EvaluateNet(&ann,&iFifo,6); */
  /* EvaluateNet(&ann,&iFifo,7); */
  /* EvaluateNet(&ann,&iFifo,8); */
  return 0; 
}
/* main.c ends here */
