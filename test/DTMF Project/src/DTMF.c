#include "DTMF.h"

/**
 * Function that will take the low and high frequencies
 * and return the corresponding DTMF tone symbol */
char toneDetection(int low_freq, int high_freq) {
  int i = 0;
  // Loop through the array of ToneMaps
  for (i = 0; i < 16; i++) {
    // Check if the low and high frequencies match the tone
    if (TONEMAP[i].low_freq == low_freq && TONEMAP[i].high_freq == high_freq) {
      // If they do return the tone char
      return TONEMAP[i].tone;
    }
  }
  // Return null character if not matched tone
  return 'Z';
}

// Function that will calculate the sum of absolute sample values within the
// specified range.
double quietDetection(int* samples, int size) {
  // Initialize a variable to store the sum of absolute sample values
  int i = 0;
  double amplitudesum = 0;

  // Iterate over the samples within the specified range
  for (i = 0; i < size; i++) {
    // Add the absolute value of the sample to the sum
    amplitudesum += (samples[i] < 0) ? -samples[i] : samples[i];
  }
  // Return the sum of absolute sample values
  return amplitudesum;
}

// Compute the sine or cosine function using a Taylor series expansion.

double my_trig(double x, int is_sin) {
  // Initialize the result to 0
  double result = 0.0;
  // Initialize the first term of the series based on the control bit
  double term = is_sin ? x : 1.0;
  // Initialize the sign of the term to positive
  double sign = 1.0;
  // Initialize the starting index for the loop based on the control bit
  int start = is_sin ? 1 : 0;
  // Initialize for loop
  int i = 0;

  // Iterate over terms of the Taylor series up to 10 terms
  for (i = start; i <= 10; i += 2) {
    // Add the current term to the result, multiplied by the current sign
    result += sign * term;

    // Change the sign for the next term
    sign = -sign;

    // Update the term using the recurrence relation of the series
    term *= (x * x) / ((i + 1) * (i + 2));
  }
  // Return the computed sine or cosine value
  return result;
}

// Initialize the MyObject struct with the given frequency and sample rate.
#define COSINE_BIT  0
#define SINE_BIT    0
#define SAMPLE_RATE 8000

void init(Frequency_t* self, double frequency) {
  // Calculate the angular frequency (w)
  double w = (2 * PI) * frequency / SAMPLE_RATE;

  // Assign the frequency value to the frequency member of MyObject
  self->frequency = frequency;

  // Calculate and assign the cosine of w to the cosw member of MyObject
  self->cosw = my_trig(w, COSINE_BIT);

  // Calculate and assign the sine of w to the sinw member of MyObject
  self->sinw = my_trig(w, SINE_BIT);
}

// Compute the magnitude of a signal segment using a filter represented by
// MyObject.
double processSample(int* samples, int size, Frequency_t freq) {
  // Initialize state variables q0, q1, and q2
  double q0 = 0, q1 = 0, q2 = 0;
  double real_part = 0, imag_part = 0;
  int i = 0;
  // Iterate over the signal segment
  for (i = 0; i < size; i++) {
    // Compute the new value of q0 using the filter equation
    q0 = samples[i] + ((2 * freq.cosw) * q1) - q2;

    // Update q2 and q1 for the next iteration
    q2 = q1;
    q1 = q0;
  }

	return (q1*q1) + (q2 * q2) - (2 * freq.cosw * q1 * q2);
	
  // Compute the real and imaginary parts of the filtered signal
  real_part = q1 - q2 * freq.cosw;
  imag_part = q2 * freq.sinw;

  // Note: we are ignoring the fact the power is really being calculated here
  // and that a sqrt is mathematically required. But this does not effect our
  // logical values.

  // Return the computed magnitude
  return real_part * real_part + imag_part * imag_part;
}
