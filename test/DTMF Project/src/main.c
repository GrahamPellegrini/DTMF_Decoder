#include <adc.h>
#include <platform.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <timer.h>
#include "DTMF.h"
#include "delay.h"
#include "lcd.h"
#include "leds.h"
#include "switches.h"

#include <stdlib.h>  // Required for rand() and srand()

#define BUFFER_SIZE     1024
#define SAMPLE_RATE     8000
#define QUIET_THRESHOLD 10

int buffer1[BUFFER_SIZE] = { 0 };

int buffer2[BUFFER_SIZE] = { 0 };

char secondRowOutput[17];

int prev_SW_UP = 0;
int prev_SW_CR = 0;

int secondRowPosition = 0;

int* currentBuffer = buffer1;

unsigned int currentBufferPosition = 0;

unsigned int cursorPosition = 0;

bool isBufferFull = false;

bool autoScroll = false;

char decodedResult = '\0';

void samplingInterrupt(void);

void detectSwitchInput(void);

char getMockDecodedResult(void);

char str[128];

char alphabet[]
    = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1111111111111111111111111111";
int alphabetIndex = 0;

int num = 0;

int main(void) {
  char prevdecodedResult = '\0';
  int lowest = 0, highest = 0;
  int lowest_freq = 0, highest_freq = 0;
  double temp = 0;
  int i = 0;
  secondRowOutput[16] = '\0';
  adc_init();
  lcd_init();
  leds_init();
  switches_init();
  lcd_clear();
  lcd_print("DTMF Decoder");
  delay_ms(1000);
  lcd_clear();

  timer_init(CLK_FREQ / SAMPLE_RATE);
  timer_set_callback(&samplingInterrupt);
  timer_enable();

  while (1) {
    detectSwitchInput();

    if (isBufferFull) {
      isBufferFull = false;
      if (currentBuffer == buffer1) {
        currentBuffer = buffer2;
      } else {
        currentBuffer = buffer1;
      }
      // Start of decoded algorithm
      if (quietDetection(currentBuffer, BUFFER_SIZE) < QUIET_THRESHOLD) {
        decodedResult = '\0';
        prevdecodedResult = '\0';
        continue;
      }

      // Low freq
      for (i = 0; i < 4; ++i) {
        temp = processSample(currentBuffer, BUFFER_SIZE, low[i]);
        // sprintf(str, "%d", temp);
        // lcd_clear();
        // lcd_print(str);
        if (temp > lowest) {
          lowest = temp;
          lowest_freq = low[i].frequency;
        }
      }

      // High freq
      for (i = 0; i < 4; ++i) {
        temp = processSample(currentBuffer, BUFFER_SIZE, high[i]);
        if (temp > highest) {
          highest = temp;
          highest_freq = high[i].frequency;
        }
      }

      prevdecodedResult = decodedResult;
      decodedResult = toneDetection(lowest_freq, highest_freq);

      // End of decode
      if (prevdecodedResult == decodedResult) {
        continue;
      }

      // Saving contents of second row in preparation for shifting up LCD.
      if (cursorPosition >= 16 && cursorPosition < 32) {
        secondRowOutput[secondRowPosition] = decodedResult;
        secondRowPosition++;
      }

      // Shifting up when end of LCD is reached.
      if (cursorPosition == 32) {
        if (autoScroll) {
          secondRowPosition = 0;
          cursorPosition = 17;
          lcd_clear();
          lcd_print(secondRowOutput);
          lcd_set_cursor(0, 1);
          lcd_put_char(decodedResult);
          secondRowOutput[secondRowPosition] = decodedResult;
          secondRowPosition++;
        }
      } else  // Otherwise we print normally
      {
        if (cursorPosition == 16)  // Row 1 is filled, switch to row 2.
        {
          lcd_set_cursor(0, 1);
        }
        lcd_put_char(decodedResult);
        cursorPosition++;
      }
    }
  }
}

void samplingInterrupt() {
  const int res = adc_read();

  if (currentBufferPosition >= BUFFER_SIZE) {
    isBufferFull = true;
    currentBufferPosition = 0;
    return;
  }

  if (currentBuffer == buffer1) {
    buffer1[currentBufferPosition] = res;
  } else {
    buffer2[currentBufferPosition] = res;
  }

  currentBufferPosition++;
}

void detectSwitchInput() {
  int SW_UP = switch_get(P_SW_UP);
  int SW_CR = switch_get(P_SW_CR);

  if (!prev_SW_UP && SW_UP) {
    if (!autoScroll) {
      leds_set(0, 0, 1);
      autoScroll = true;
    } else {
      leds_set(0, 0, 0);
      autoScroll = false;
    }
    prev_SW_UP = SW_UP;
  } else if (prev_SW_UP && !SW_UP) {
    prev_SW_UP = SW_UP;
  } else if (!prev_SW_CR && SW_CR) {
    leds_set(0, 1, 0);
    lcd_clear();
    cursorPosition = 0;
    secondRowPosition = 0;
    prev_SW_CR = SW_CR;
  } else if (prev_SW_CR && !SW_CR) {
    prev_SW_CR = SW_CR;
    if (autoScroll) {
      leds_set(0, 0, 1);
    } else {
      leds_set(0, 0, 0);
    }
  } else if (switch_get(P_SW_DN)) {
    exit(0);
  } else if (switch_get(P_SW_LT)) {
    exit(0);
  } else if (switch_get(P_SW_RT)) {
    exit(0);
  }
}