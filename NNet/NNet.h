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

void EvaluateNet(float);
void initNetwork(void);

extern float inputs[MAX_INPUTS];
extern float hiddens[MAX_HIDDEN];
extern float outputs[MAX_OUTPUTS];

/* NNet.h ends here */
