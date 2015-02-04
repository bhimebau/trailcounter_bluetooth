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

#define MAX_INPUTS 32
#define MAX_HIDDEN 32
#define MAX_OUTPUTS 8

extern int inputs[MAX_INPUTS];
extern int hiddens[MAX_HIDDEN];
extern int outputs[MAX_OUTPUTS];
extern int weights_ih[MAX_INPUTS][MAX_HIDDEN];
extern int weights_ho[MAX_HIDDEN][MAX_OUTPUTS];

typedef struct {
  int numInputs;
  int numHidden;
  int numOutput;
} nncfg_t;

void EvaluateNet(nncfg_t *, int);
int setWeightHidden(nncfg_t *, int, int, int);
int setWeightOutput(nncfg_t *, int, int, int);

/* NNet.h ends here */
