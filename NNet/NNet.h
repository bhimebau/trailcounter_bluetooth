/* NNet.h --- 
 * 
 * Filename: NNet.h
 * Description: 
 * Author: Bryce Himebaugh
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

/* #define MAX_INPUTS 32 */
/* #define MAX_HIDDEN 32 */
/* #define MAX_OUTPUTS 8 */

// extern int inputs[MAX_INPUTS];
extern float hiddens[MAX_HIDDEN];
extern float outputs[MAX_OUTPUTS];
extern float weights_ih[MAX_INPUTS][MAX_HIDDEN];
extern float weights_ho[MAX_HIDDEN][MAX_OUTPUTS];

typedef struct {
  int numInputs;
  int numHidden;
  int numOutput;
} nncfg_t;

typedef struct {
  int inputIndex;
  float inputs[MAX_INPUTS];
} inputFifo_t;

// extern inputFifo_t iFifo;

void EvaluateNet(nncfg_t *, inputFifo_t *, int);
void initNetwork(nncfg_t *, inputFifo_t *);
int setWeightHidden(nncfg_t *, int, int, int);
int setWeightOutput(nncfg_t *, int, int, int);

void addItem(nncfg_t *, inputFifo_t *, int item);
void printFifo(nncfg_t *, inputFifo_t *);

float scale_input(int);  
int scale_output(float);  

/* NNet.h ends here */
