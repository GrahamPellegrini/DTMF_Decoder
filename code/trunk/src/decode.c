/**
 * @file decode.c
 * @brief File which contains an implementation of the Goertzel Algorithm
 * @author Luca Vella, Matthew Mifsud, Graham Pellegrini
 **/

#include "decode.h"

#define QUIET_THRESHOLD    1.5
#define GOERZTEL_THRESHOLD 125

static struct ToneMap TONEMAP[16]
    = { { 697, 1209, '1' }, { 697, 1336, '2' }, { 697, 1477, '3' },
        { 697, 1633, 'A' }, { 770, 1209, '4' }, { 770, 1336, '5' },
        { 770, 1477, '6' }, { 770, 1633, 'B' }, { 852, 1209, '7' },
        { 852, 1336, '8' }, { 852, 1477, '9' }, { 852, 1633, 'C' },
        { 941, 1209, '*' }, { 941, 1336, '0' }, { 941, 1477, '#' },
        { 941, 1633, 'D' } };

static Frequency_t low[4] = {
  { 697, 0.8538689047 * 2 },
  { 770, 0.8226405180 * 2 },
  { 852, 0.7843434919 * 2 },
  { 941, 0.7391022841 * 2 },
};

static Frequency_t high[4] = {
  { 1209, 0.5820520115 * 2 },
  { 1336, 0.4981851053 * 2 },
  { 1477, 0.3993091946 * 2 },
  { 1633, 0.2842663534 * 2 },
};

char toneDetection(short low_freq, short high_freq) {
  // Loop variable
  int i = 0;

  // Loop through the array of ToneMaps
  for (i = 0; i < 16; ++i) {
    // Check if the low and high frequencies match the tone
    if (TONEMAP[i].low_freq == low_freq && TONEMAP[i].high_freq == high_freq) {
      // If they do, return the tone char
      return TONEMAP[i].tone;
    }
  }

  // Return null character if not matched tone
  return '\0';
}

float quietDetection(float* samples, unsigned short size) {
  // Loop variable
  int i = 0;
  // Stores the sum of absolute sample values
  float amplitudesum = 0;

  // Iterate over the samples within the specified range
  for (i = 0; i < size; i++) {
    // Add the absolute value of the sample to the sum
    amplitudesum += (samples[i] < 0) ? -samples[i] : samples[i];
  }

  // Return the sum of absolute sample values
  return amplitudesum;
}

float processSample(float* samples, unsigned short size, Frequency_t freq) {
  // Loop variable
  int i = 0;
  // Initialize state variables q0, q1, and q2
  float q0 = 0, q1 = 0, q2 = 0;

  // Iterate over the signal segment
  for (i = 0; i < size; ++i) {
    // Compute the new value of q0 using the filter equation
    q0 = samples[i] + (freq.cosw * q1) - q2;
    // freq.cosw is actually 2 * the cos

    // Update q2 and q1 for the next iteration
    q2 = q1;
    q1 = q0;
  }

  return (q1 * q1) + (q2 * q2) - (freq.cosw * q1 * q2); // Return the magnitude
}

char decode(float* buffer, unsigned short size) {
  int i = 0; // Loop variable
  unsigned char low_found = 0, high_found = 0; // Flags for if low and high freqs are found
  float low_freq_mag = 0, high_freq_mag = 0; // Magnitudes of low and high frequencies
  float low_freq_highest = 0, high_freq_highest = 0; // Low and high frequencies, highest frequency
  short lowest_freq = 0, highest_freq = 0; // Lowest and highest frequencies

  // Start of decoded algorithm
  if (quietDetection(buffer, size) < (float) QUIET_THRESHOLD) { // Check if quiet
    return '\0'; // Return null character
  }

  for (i = 0; i < 4; ++i) { // Loop through the low and high frequencies
    // Low freq
    low_freq_mag = processSample(buffer, size, low[i]); // Compute the magnitude of the low frequency
    if (low_freq_mag > GOERZTEL_THRESHOLD) { // Check if the magnitude is above the threshold
      if (low_found) { // Check if the low frequency has already been found
        return '!'; // Return an error character
      }

      if (low_freq_mag < low_freq_highest) { // Check if the magnitude is higher than the highest
        continue; // Skip to the next iteration
      }

      low_found = 1; // Set the low frequency found flag
      low_freq_highest = low_freq_mag; // Set the highest magnitude
      lowest_freq = low[i].frequency; // Set the lowest frequency
    }

    // High freq
    high_freq_mag = processSample(buffer, size, high[i]); // Compute the magnitude of the high frequency
    if (high_freq_mag > GOERZTEL_THRESHOLD) { // Check if the magnitude is above the threshold
      if (high_found) { // Check if the high frequency has already been found
        return '!'; // Return an error character
      }

      if (high_freq_mag < high_freq_highest) { // Check if the magnitude is higher than the highest
        continue; // Skip to the next iteration
      }

      high_found = 1; // Set the high frequency found flag
      high_freq_highest = high_freq_mag; // Set the highest magnitude
      highest_freq = high[i].frequency; // Set the highest frequency
    }
  }

  return toneDetection(lowest_freq, highest_freq); // Return the corresponding tone
}
