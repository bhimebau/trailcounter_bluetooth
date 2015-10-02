/* atoh.c --- 
 * 
 * Filename: atoh.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Mon Feb  2 10:38:00 2015
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

/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
/* #include "ch.h" */
/* #include "chprintf.h" */

int atoh(char *p, int *value) {
  int tmp = 0;
  int ret_val = 0;
  int loop_count = 0;
  char *t; 
  t = p;
  while (*t != 0) {
    if ((*t == 'x') || (*t == 'X')) {
      t++;
      p = t;
      break;
    }
    t++;
  }
  while (*p != 0) {
    tmp = tmp << 4;
    if ((*p>='0') && (*p<='9')) {
      tmp += (*p-'0');
    }
    else if ((*p>='a') && (*p<='f')) {
      tmp += (*p-'a') + 10;
    }
    else if ((*p>='A') && (*p<='F')) {
      tmp += (*p-'A') + 10;
    }      
    else {
      ret_val = 1;
      return(ret_val);
    }
    if (loop_count++>=8) {
      ret_val = 1;
      return(ret_val);
    }     
    p++;
  }
  *value = tmp;
  return(ret_val);
}


/* atoh.c ends here */
