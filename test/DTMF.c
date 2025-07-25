#include "DTMF.h"

// Function that will take the low and high frequencies and return the corresponding DTMF tone symbol 
char toneDetection(int low_freq, int high_freq) { 
    // Loop through the array of ToneMaps
    for (int i = 0; i < sizeof(TONEMAP) / sizeof(struct ToneMap); i++) {

        // Check if the low and high frequencies match the tone
        if (TONEMAP[i].frequency1 == low_freq && TONEMAP[i].frequency2 == high_freq){
            // If they do return the tone char
            return TONEMAP[i].tone; 
        }
    }
    // Return null character if not matched tone
    return '\0'; 
}

// Function that will calculate the sum of absolute sample values within the specified range.
double quietDetection(double *samples, int start, int end) {
    // Initialize a variable to store the sum of absolute sample values
    double amplitudesum = 0;

    // Iterate over the samples within the specified range
    for (int i = start; i < end; i++) {
        // Take the absolute value of each sample
        double sample = samples[i] < 0 ? -samples[i] : samples[i];
        // Add the absolute value of the sample to the sum
        amplitudesum += sample;
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

    // Iterate over terms of the Taylor series up to 10 terms
    for (int i = start; i <= 10; i += 2) {
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
void init(MyObject *self, double frequency, double sample_rate) {
    // Assign the frequency value to the frequency member of MyObject
    self->frequency = frequency;
    // Calculate the angular frequency (w) 
    double w = (2 * PI) * frequency / sample_rate;

    // Calculate and assign the cosine of w to the cosw member of MyObject
    self->cosw = my_trig(w,COSINE_BIT);

    // Calculate and assign the sine of w to the sinw member of MyObject
    self->sinw = my_trig(w,SINE_BIT);
}

//Compute the magnitude of a signal segment using a filter represented by MyObject.
double processSample(double *samples, int start, int end, MyObject freq) {
    // Initialize state variables q0, q1, and q2
    double q0 = 0, q1 = 0, q2 = 0;
    
    // Iterate over the signal segment
    for (int i = start; i < end; i++) {
        // Compute the new value of q0 using the filter equation
        q0 = samples[i] + ((2 * freq.cosw) * q1) - q2;
        
        // Update q2 and q1 for the next iteration
        q2 = q1;
        q1 = q0;
    }

    // Compute the real and imaginary parts of the filtered signal
    double real_part = q1 - q2 * freq.cosw;
    double imag_part = q2 * freq.sinw;
    
    // Computing the magnitude of the signal 
    double magnitude = real_part * real_part + imag_part * imag_part;
    // Note: we are ignoring the fact the power is really being calculated here and that a sqrt is mathematically required. But this does not effect our logical values.

    // Return the computed magnitude
    return magnitude;
}





