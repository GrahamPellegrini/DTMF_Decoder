/**
 * @mainpage DTMF Decoder
 * @section intro Project Description
 * This project is a Dual Tone Multi-Frequency (DTMF) decoder which samples a
 * signal, decodes it and displays it. The project uses the LPC4088
 * microcontroller, an LCD, an LED and a switch. The project uses the ADC to
 * sample the signal, the LCD to display the decoded signal, the LED to indicate
 * the status of the program and the switch to toggle AutoScroll, Clear Display
 * and Enter Debugging Mode (Read ADC Value). The project also uses the
 * EEPROM to store the auto scroll status.
 * @subsection team Team
 * The project was developed by Matthew Mifsud, Luca Vella, Graham Pellegrini.
 * @file main.c
 * @brief Main file which samples a signal, decodes it and displays it. (Using
 * the LCD having with an AutoScroll mechanism and an LED)
 * @author Matthew Mifsud, Luca Vella, Graham Pellegrini
 **/

#include <platform.h>

#include <stdio.h>
#include <stdlib.h>

#include <adc.h>
#include <delay.h>
#include <eeprom.h>
#include <lcd.h>
#include <leds.h>
#include <switches.h>
#include <timer.h>

#include "decode.h"

#define SCREEN_SIZE 16

// Configurations
#define SAMPLE_RATE 8000
#define BASELINE    2048
#define BUFFER_SIZE 240

// EEPROM Configurations
#define EE_AUTOSCROLL_OFFSET  16
#define EE_AUTOSCROLL_ADDRESS 16

// Creating two buffers to store the ADC values
static float buffer1[BUFFER_SIZE] = { 0 };
static float buffer2[BUFFER_SIZE] = { 0 };

// Creating a buffer to store the output in the second row of the LCD
static char secondRowOutput[SCREEN_SIZE + 1] = { 0 };

// Creating a pointer to switch between the two buffers
static float* currentBuffer = buffer1;

// Creating a variable to keep track of the current buffer index
static unsigned short currentBufferIndex = 0;

// Creating a variable to keep track of the cursor position
static unsigned short cursorPosition = 0;

// Creating variables to keep track of the previous switch states
static unsigned char prev_SW_LT = 0;
static unsigned char prev_SW_DN = 0;
static unsigned char prev_SW_UP = 0;
static unsigned char prev_SW_CR = 0;
static unsigned char prev_SW_RT = 0;

// Creating a variable to keep track of the buffer status
static unsigned char isBufferFull = 0;

// Creating a variable to keep track of the auto scroll status
static unsigned char autoScroll = 0;

// Creating a variable to keep track if we're in ADC value mode (debugging mode)
static unsigned char adcValue = 0;

// Creating a variable to add a slight delay so that printed ADC values are
// visible
static unsigned int adcDelay = 0;

/**
 * @brief Function to initialize all components i.e. ADC, LCD, LED, Switch and
 * EEPROM.
 * @param None
 */
void initializeComponents(void);

/**
 * @brief Function to print a start message on the LCD.
 * @param message The message to be printed.
 */
void printStartMessage(char* message);

/**
 * @brief Function to setup the sampling timer and start it.
 * @param None
 */
void startSamplingTimer(void);

/**
 * @brief Function to which will be invoked by the timer interrupt to sample
 * using the ADC.
 * @param None
 */
void samplingInterrupt(void);

/**
 * @brief Function to detect switch input and act accordingly.
 * @param None
 */
void detectSwitchInput(void);

int main(void) {
  // Variables to store the current and previous decoded results
  char decodedResult = '\0';
  char prevdecodedResult = '\0';
  // Variable to store the ADC value as a string
  char str[16] = { 0 };

  initializeComponents();             // Initialize all components
  printStartMessage("DTMF Decoder");  // Print start message on LCD
  startSamplingTimer();               // Start the sampling timer

  // Infinite main loop
  while (1) {
    __WFI();              // Wait for interrupt to save power
    detectSwitchInput();  // Detecting any switch input

    if (!isBufferFull)
      continue;  // Do nothing till buffer full

    isBufferFull = 0;  // Reset buffer status

    // If ADC value is enabled
    if (adcValue) {
      // Set delay to a value between 0 and 24
      adcDelay = adcDelay % 25;

      // If delay is 0
      if (adcDelay == 0) {
        // Convert ADC value to string
        sprintf(str, "%+0.5f", (currentBuffer[0]));

        lcd_clear();
        lcd_print("ADC Value: ");  // Print ADC value on LCD
        lcd_set_cursor(0, 1);      // Set cursor to second row
        lcd_print(str);            // Print ADC value on LCD in second row
      }

      adcDelay++;  // Increment delay
      continue;    // Skip decoding
    }

    // Save previous decoded result
    prevdecodedResult = decodedResult;
    // Decode the current full buffer
    decodedResult = decode(currentBuffer, BUFFER_SIZE);

    // If two frequencies were detected at the same time
    if (decodedResult == '!') {
      leds_set(1, 0, 0);  // Set LED to red
    }

    // If no frequency was detected
    if (decodedResult == '\0' || prevdecodedResult == decodedResult) {
      continue;  // Skip printing
    }

    // Saving contents of second row in preparation for shifting up LCD.

    // If cursor is equal or greater to the last position in row 1
    if (cursorPosition >= SCREEN_SIZE) {
      // If the first row is filled, switch to the second row.
      if (cursorPosition == SCREEN_SIZE) {
        lcd_set_cursor(0, 1);  // Set cursor to the second row
      }

      // Save decoded result in second row
      secondRowOutput[cursorPosition - SCREEN_SIZE] = decodedResult;
    }

    // Shifting up when end of LCD is reached.

    // If cursor is equal or greater tothe last position in row 2
    if (cursorPosition >= SCREEN_SIZE * 2) {
      if (autoScroll) {
        // If auto scroll is enabled

        lcd_clear();                 // Clear LCD
        lcd_print(secondRowOutput);  // Print second row output in the first row
        lcd_set_cursor(0, 1);        // Set cursor to the second row

        // Set cursor position to the last position in the first row
        cursorPosition = SCREEN_SIZE;
      }
      // If auto scroll is disabled
      else {
        lcd_clear();         // Clear LCD
        cursorPosition = 0;  // Set cursor position to 0 (first position in LCD)
      }
    }

    // Finally print decoded result on LCD
    lcd_put_char(decodedResult);
    // Setting LED to white if debugging mode is enabled
    leds_set(adcValue, adcValue, (adcValue || autoScroll));
    // Increment cursor position
    cursorPosition++;
  }
}

void initializeComponents(void) {
  adc_init();       // Initialize ADC
  lcd_init();       // Initialize LCD
  leds_init();      // Initialize LED
  switches_init();  // Initialize Switch
  eeprom_init();    // Initialize EEPROM

  // Read auto scroll status from EEPROM
  eeprom_read(EE_AUTOSCROLL_OFFSET, EE_AUTOSCROLL_ADDRESS, &autoScroll, 1);
  // Set LED to blue if auto scroll is enabled
  leds_set(0, 0, autoScroll);
}

void printStartMessage(char* message) {
  lcd_clear();         // Clear LCD
  lcd_print(message);  // Print message on LCD
  delay_ms(1000);      // Delay for 1 second to allow user to read message
  lcd_clear();         // Clear LCD
}

void startSamplingTimer(void) {
  // Initialize timer with the sampling rate
  timer_init(CLK_FREQ / SAMPLE_RATE);
  // Set callback function for timer interrupt
  timer_set_callback(&samplingInterrupt);
  // Enable timer
  timer_enable();
}

void samplingInterrupt(void) {
  // Read ADC value and subtract baseline
  volatile const short res = adc_read() - BASELINE;
  // Convert ADC value to voltage
  volatile const float vadc = res * (3.3 / ADC_MASK);

  // If buffer is full
  if (currentBufferIndex >= BUFFER_SIZE) {
    isBufferFull = 1;        // Set buffer status to full
    currentBufferIndex = 0;  // Reset buffer index

    // Switch buffer
    currentBuffer = (currentBuffer == buffer1) ? buffer2 : buffer1;
  }

  // If current buffer is buffer 1
  if (currentBuffer == buffer1) {
    // Add ADC value to buffer 1
    buffer1[currentBufferIndex++] = vadc;
  }
  // If current buffer is buffer 2
  else {
    // Add ADC value to buffer 2
    buffer2[currentBufferIndex++] = vadc;
  }
}

void detectSwitchInput(void) {
  // Variables to store switch states
  unsigned char SW_UP, SW_DN, SW_CR, SW_LT, SW_RT;

  /* Toggle auto scroll */

  // Get up switch's state
  SW_UP = switch_get(P_SW_UP);

  // If switch is in rising state
  if (!prev_SW_UP && SW_UP) {
    // Set previous switch state to pressed
    prev_SW_UP = 1;

    // Toggle auto scroll
    autoScroll = !autoScroll;
    // Erase EEPROM
    eeprom_erase(EE_AUTOSCROLL_ADDRESS);
    // Write auto scroll status to EEPROM
    eeprom_write(EE_AUTOSCROLL_OFFSET, EE_AUTOSCROLL_ADDRESS, &autoScroll, 1);
    // Set LED to white if debugging mode is enabled or blue if auto scroll is
    // enabled
    leds_set(adcValue, adcValue, (adcValue || autoScroll));

    return;
  }
  // If switch is in falling state
  else if (prev_SW_UP && !SW_UP) {
    // Set previous switch state to not pressed
    prev_SW_UP = 0;
    return;
  }

  /* Clearing LCD */

  // Get centre switch's state
  SW_CR = switch_get(P_SW_CR);

  // If switch is in rising state
  if (!prev_SW_CR && SW_CR) {
    // Set previous switch state to pressed
    prev_SW_CR = 1;

    leds_set(0, 1, 0);   // Set LED to green
    lcd_clear();         // Clear LCD
    cursorPosition = 0;  // Reset cursor position

    return;
  }
  // If switch is in falling state
  else if (prev_SW_CR && !SW_CR) {
    // Set previous switch state to not pressed
    prev_SW_CR = 0;
    // Set LED to white if debugging mode is enabled or blue if auto scroll is
    // enabled
    leds_set(adcValue, adcValue, (adcValue || autoScroll));

    return;
  }

  /* Exiting Program */

  // Get down switch's state
  SW_DN = switch_get(P_SW_DN);

  // If switch is in rising state
  if (!prev_SW_DN && SW_DN) {
    // Set previous switch state to pressed
    prev_SW_DN = 1;

    lcd_clear();           // Clear LCD
    lcd_print("Goodbye");  // Print goodbye message on LCD
    exit(0);               // Exit program

    return;
  }
  // If switch is in falling state
  else if (prev_SW_DN && !SW_DN) {
    // Set previous switch state to not pressed
    prev_SW_DN = 0;

    return;
  }

  /* ADC Debug Mode */

  // Get left switch's state
  SW_LT = switch_get(P_SW_LT);

  // If switch is in rising state
  if (!prev_SW_LT && SW_LT) {
    // Set previous switch state to pressed
    prev_SW_LT = 1;

    // Toggle ADC value mode
    adcValue = !adcValue;
    // Reset cursor position
    cursorPosition = 0;
    // Set LED to white if debugging mode is enabled or blue if auto scroll is
    // enabled
    leds_set(adcValue, adcValue, (adcValue || autoScroll));

    return;
  }
  // If switch is in falling state
  else if (prev_SW_LT && !SW_LT) {
    // Set previous switch state to not pressed
    prev_SW_LT = 0;
    // Reset delay counter
    adcDelay = 0;
    // Clear LCD
    lcd_clear();

    return;
  }

  /* Exiting Program */

  // Get right switch's state
  SW_RT = switch_get(P_SW_RT);

  // If switch is in rising state
  if (!prev_SW_RT && SW_RT) {
    // Set previous switch state to pressed
    prev_SW_RT = 1;

    lcd_clear();           // Clear LCD
    lcd_print("Goodbye");  // Print goodbye message on LCD
    exit(0);               // Exit program

    return;
  }
  // If switch is in falling state
  else if (prev_SW_RT && !SW_RT) {
    // Set previous switch state to not pressed
    prev_SW_RT = 0;

    return;
  }
}
