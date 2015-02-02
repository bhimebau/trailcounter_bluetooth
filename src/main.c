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

#include "ch.h"
#include "hal.h"
#include "test.h"
#include "shell.h" 
#include "chprintf.h"
#include "drivers.h"
#include <NNet.h>
#include <atoh.h>
#include <chstreams.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define UNUSED(x) (void)(x)

// ADCConfig structure for stm32 MCUs is empty
static ADCConfig adccfg = {0};

// Create buffer to store ADC results. This is
// one-dimensional interleaved array
#define ADC_BUF_DEPTH 1 // depth of buffer
#define ADC_CH_NUM 2    // number of used ADC channels
static adcsample_t samples_buf[ADC_BUF_DEPTH * ADC_CH_NUM]; // results array

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


static void gpt_adc_trigger(GPTDriver *gpt_ptr)  { 
  UNUSED(*gpt_ptr);
  EvaluateNet(samples_buf[0]);
  dacConvertOne(&DACD1,outputs[0]);
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


static const ShellCommand commands[] = {
  {"myecho", cmd_myecho},
  {"dac", cmd_dac},
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

/*
 * Application entry point.
 */

int main(void) {
  event_listener_t tel;
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

  dacStart(&DACD1, &daccfg1);

  /*
   * Activates the serial driver 1 using the driver default configuration.
   * PC4(RX) and PC5(TX). The default baud rate is 9600.
   */
  sdStart(&SD1, NULL);
  palSetPadMode(GPIOC, 4, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOC, 5, PAL_MODE_ALTERNATE(7));

  gptStart(&GPTD1, &gpt_adc_config); 
  gptStartContinuous(&GPTD1, 10);

  palSetPadMode(GPIOA, 0, PAL_MODE_INPUT_ANALOG); // this is 15th channel
  palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_ANALOG); // this is 10th channel

  // Following 3 functions use previously created configuration
  // to initialize ADC block, start ADC block and start conversion.
  // &ADCD1 is pointer to ADC driver structure, defined in the depths of HAL.
  // Other arguments defined ourself earlier.
  //  adcInit();
  adcStart(&ADCD1, &adccfg);
  // adcStartConversion(&ADCD1, &adcgrpcfg, samples_buf, ADC_BUF_DEPTH);

  chprintf((BaseSequentialStream*)&SD1, "\n\rUp and Running\n\r");

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


