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
inputFifo_t iFifo;

int main(){
  initNetwork(&ann,&iFifo);
  EvaluateNet(&ann,&iFifo,1);
  printFifo(&ann,&iFifo);
  EvaluateNet(&ann,&iFifo,2);
  printFifo(&ann,&iFifo);
  EvaluateNet(&ann,&iFifo,3);
  printFifo(&ann,&iFifo);
  EvaluateNet(&ann,&iFifo,4);
  printFifo(&ann,&iFifo);
  EvaluateNet(&ann,&iFifo,5);
  printFifo(&ann,&iFifo);
  EvaluateNet(&ann,&iFifo,6);
  printFifo(&ann,&iFifo);
  EvaluateNet(&ann,&iFifo,7);
  printFifo(&ann,&iFifo);
  EvaluateNet(&ann,&iFifo,8);
  printFifo(&ann,&iFifo);
  return 0; 
}
/* main.c ends here */
