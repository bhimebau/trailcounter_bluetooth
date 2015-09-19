/*
    ChibiOS - Copyright (C) 2006-2014 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/* Analog Input PA0 
   Analog Output PA4 
 */

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h" 
#include "chprintf.h"
#include "arm_math.h"
#include "drivers.h"
#include <NNet.h>
#include <arb.h>
#include <atoh.h>
#include <dtoa.h>
#include <chstreams.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x)
#define IDS 32
#define HDS 32
#define ODS 8

// ADCConfig structure for stm32 MCUs is empty
static ADCConfig adccfg = {0};

// Create buffer to store ADC results. This is
// one-dimensional interleaved array
#define ADC_BUF_DEPTH 1 // depth of buffer
#define ADC_CH_NUM 2    // number of used ADC channels
static adcsample_t samples_buf[ADC_BUF_DEPTH * ADC_CH_NUM]; // results array
int start_collection = 0;
int collection_buf[32];

static const ADCConversionGroup adcgrpcfg = {
  FALSE,    
  2,     
  NULL,    
  NULL,    
  0,      /* CFGR */    
  0,      /* TR1 */
  0,      /* CCR */
  /* SMPR1 */
  {ADC_SMPR1_SMP_AN1(ADC_SMPR_SMP_7P5) |    
   ADC_SMPR1_SMP_AN2(ADC_SMPR_SMP_7P5),
   
   /* SMPR2 */
   0},
  
  /* SQR1 */
  {ADC_SQR1_NUM_CH(2) |
  ADC_SQR1_SQ1_N(ADC_CHANNEL_IN1) |
  ADC_SQR1_SQ2_N(ADC_CHANNEL_IN2),
  
  /* SQR2 */
  0,
  
  /* SQR3 */
  0,

  /* SQR4 */
   0}
};

float scale_input (unsigned short a2dval) {
  float voltage;
  voltage = ((float) a2dval/4096.0) * 3.0;
  return (voltage * ARB_SCALE_FACTOR + ARB_OFFSET); // scale, offset come from arb.c[h]
}

unsigned int scale_output (float nnval) {
  return ((nnval/3.0) * 4096);
}

#define COLLECTION_THRESHOLD 0x180

static void gpt_adc_trigger(GPTDriver *gpt_ptr)  { 
  UNUSED(*gpt_ptr);
  //  static int collection_count = 0;
  //  typedef enum {DISARMED, ARMED, COLLECTING} collection_t;
  //  static collection_t cstate = DISARMED;
  EvaluateNet(scale_input(samples_buf[0]));
  dacConvertOne(&DACD1,scale_output(outputs[0]));
  adcStartConversion(&ADCD1, &adcgrpcfg, samples_buf, ADC_BUF_DEPTH);
  palTogglePad(GPIOE, GPIOE_LED4_BLUE);
}

static GPTConfig gpt_adc_config = { 
  1000000,         // timer clock: 1Mhz 
  gpt_adc_trigger, // Timer callback function 
  0,
  0
};


static THD_WORKING_AREA(waShell,2048);
static thread_t *shelltp1;

/* Thread that blinks North LED as an "alive" indicator */
static THD_WORKING_AREA(waCounterThread,128);
static THD_FUNCTION(counterThread,arg) {
  UNUSED(arg);
  while (TRUE) {
    palSetPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(500);
    palClearPad(GPIOE, GPIOE_LED3_RED);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

static void cmd_myecho(BaseSequentialStream *chp, int argc, char *argv[]) {
  int32_t i;

  (void)argv;

  for (i=0;i<argc;i++) {
    chprintf(chp, "%s\n\r", argv[i]);
  }
}

static void cmd_dac(BaseSequentialStream *chp, int argc, char *argv[]) {
  int tmp;

  (void)argv;
  if (argc!=1) {
    chprintf(chp, "Error: wrong number of arguments. %d provided\n\rExample: dac 3FF\n\r", argc);
  }
  if (!(atoh(argv[0], &tmp))) {
    if (tmp > 0xFFF) {
      chprintf(chp, "Value Error: 0 >= value <= 0xFFF: Value provided 0x%x\n\r", tmp);    
    }
    else {
      chprintf(chp, "Wrote 0x%x to dac 1\n\r", tmp);
      dacConvertOne(&DACD1,tmp);
    }
  }
  else {
    chprintf(chp, "atoh error: cannot convert %s\n\r", argv[0]);    
  }
}


static void cmd_sample(BaseSequentialStream *chp, int argc, char *argv[]) {
  UNUSED(chp);
  UNUSED(argc);
  char float_array[32];
  (void)argv;
  chprintf((BaseSequentialStream*)&SD1, "0x%04x ", samples_buf[0]);
  chprintf((BaseSequentialStream*)&SD1, "%s\n\r", convFloat(float_array, scale_input(samples_buf[0])));
}

static void cmd_testweights(BaseSequentialStream *chp, int argc, char *argv[]) {
  UNUSED(chp);
  UNUSED(argc);
  char float_array[32];
  float scaled_input;
  int i;
  (void)argv;
  for (i=0;i<32;i++) {
    scaled_input = nntest_data_sampled[i] * ARB_SCALE_FACTOR + ARB_OFFSET;
    EvaluateNet(scaled_input);
    chprintf((BaseSequentialStream*)&SD1, "%d,", i);
    chprintf((BaseSequentialStream*)&SD1, "%s ", convFloat(float_array, scaled_input));
    chprintf((BaseSequentialStream*)&SD1, "%s\n\r", convFloat(float_array, outputs[0]));
  }
}


static void cmd_collect (BaseSequentialStream *chp, int argc, char *argv[]) {
  UNUSED(chp);
  UNUSED(argc);

  char float_array[32];
  /* float scaled_input; */
  int i;
  (void)argv;

  start_collection = 1;
  while (start_collection) {
    chThdSleepMilliseconds(1);
  }
  for (i=0;i<32;i++) {
    chprintf((BaseSequentialStream*)&SD1, "0x%0x ", collection_buf[i]);
    chprintf((BaseSequentialStream*)&SD1, "%s\n\r ", convFloat(float_array, scale_input(collection_buf[i])));
  }
}

/* static void cmd_timnet(BaseSequentialStream *chp, int argc, char *argv[]) { */
/*   int hidden; */
/*   int input; */
/*   int value; */
/*   float input_array[32]; */
/*   float hidden_weights[32]; */

/*   (void)argv; */
/*   input = atoi(argv[0]); */
/*   hidden = atoi(argv[1]); */
/*   palSetPad(GPIOE, GPIOE_LED5_ORANGE); */
/*   chThdSleepMilliseconds(1); */
/*   //chprintf(chp, "input = %d hidden = %d\n\r", input, hidden); */
/*   palClearPad(GPIOE, GPIOE_LED5_ORANGE); */
/* } */

static const ShellCommand commands[] = {
  {"myecho", cmd_myecho},
  {"dac", cmd_dac},
  /* {"weighth", cmd_weighth}, */
  /* {"weighto", cmd_weighto}, */
  {"sample", cmd_sample},
  {"tw", cmd_testweights},
  {"collect", cmd_collect},
  {NULL, NULL}

};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SD1,
  commands
};

static void termination_handler(eventid_t id) {

  (void)id;
  chprintf((BaseSequentialStream*)&SD1, "Shell Died\n\r");

  if (shelltp1 && chThdTerminatedX(shelltp1)) {
    chThdWait(shelltp1);
    chprintf((BaseSequentialStream*)&SD1, "Restarting from termination handler\n\r");
    chThdSleepMilliseconds(100);
    shelltp1 = shellCreate(&shell_cfg1, sizeof(waShell), NORMALPRIO);
  }
}

static evhandler_t fhandlers[] = {
  termination_handler
};

static const DACConfig daccfg1 = {
  DAC_DHRM_12BIT_RIGHT,
  0
};


int main(void) {
  event_listener_t tel;
  
  q31_t hiddenin[IDS];
  q31_t hidden_weights[IDS];
  q31_t outin[HDS];
  q31_t out_weights[HDS];
  q63_t result;
  int i;
  
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  driversInit();
  chSysInit();
  
  for (i=0;i<IDS;i++) {
    hiddenin[i] = (float) i + 2.5;
    hidden_weights[i] = (float) i + 4.786;
  }
  for (i=0;i<HDS;i++) {
    outin[i] = (float) i + 2.5;
    out_weights[i] = (float) i + 4.786;
  }
  palSetPad(GPIOE, GPIOE_LED5_ORANGE);
  
  // Sample/Hold
  for (i=HDS-1;i>0;i--) {
    hiddenin[i] = hiddenin[i-1];
  }
  hiddenin[0] = 3.546;
  
  // Input Scaling 
  arm_dot_prod_q31(hiddenin,hidden_weights,IDS,&result);
  
  // Hidden Inputs 
  for (i=0;i<HDS;i++) {
    arm_dot_prod_q31(hiddenin,hidden_weights,IDS,&result);
    if (result > 1) result = 1;
    if (result < 0) result = 0;
  }
  for (i=0;i<ODS;i++) {
    arm_dot_prod_q31(outin,out_weights,HDS,&result);
    if (result > 1) {
      result = 1;
    }
    else if (result < 0) {
      result = 0;
    }
  }
  palClearPad(GPIOE, GPIOE_LED5_ORANGE);
  
  dacStart(&DACD1, &daccfg1);
    
    
    /*
     * Activates the serial driver 1 using the driver default configuration.
   * PC4(RX) and PC5(TX). The default baud rate is 9600.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));

  gptStart(&GPTD1, &gpt_adc_config);
  // gptStartContinuous(&GPTD1, 227);
  gptStartContinuous(&GPTD1, (int) SAMPLE_PERIOD_US);

  palSetPadMode(GPIOA, 0, PAL_MODE_INPUT_ANALOG); // this is 15th channel
  palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_ANALOG); // this is 10th channel
  adcStart(&ADCD1, &adccfg);

  // chprintf((BaseSequentialStream*)&SD1, "\n\rUp and Running %s\n\r", convFloat(afloat,max/2));
  /* print_arch(&ann); */
  /* Initialize the command shell */ 
  shellInit();

  /* 
   *  setup to listen for the shell_terminated event. This setup will be stored in the tel  * event listner structure in item 0
   */
  chEvtRegister(&shell_terminated, &tel, 0);

  shelltp1 = shellCreate(&shell_cfg1, sizeof(waShell), NORMALPRIO);
  chThdCreateStatic(waCounterThread, sizeof(waCounterThread), NORMALPRIO+1, counterThread, NULL);

  while (TRUE) {
    chEvtDispatch(fhandlers, chEvtWaitOne(ALL_EVENTS));
  }
 }


