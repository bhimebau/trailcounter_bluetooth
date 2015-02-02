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
#define NUM_INPUT_NODE 3
#define NUM_HIDDEN_NODE 2
#define NUM_OUTPUT_NODE 3

extern int inputs[NUM_INPUT_NODE];
extern int hiddens[NUM_HIDDEN_NODE];
extern int outputs[NUM_OUTPUT_NODE];
extern int weights_ih[NUM_INPUT_NODE][NUM_HIDDEN_NODE];
extern int weights_ho[NUM_HIDDEN_NODE][NUM_OUTPUT_NODE];

void EvaluateNet(int);

/* NNet.h ends here */
