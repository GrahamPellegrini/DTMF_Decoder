/**
 * @file decode.h
 * @brief Header file containing all functions and structs required for the
 *Goertzel Algorithm.
 * @author Luca Vella, Matthew Mifsud, Graham Pellegrini
 **/

#ifndef DECODE_H
#define DECODE_H

/**
 * @struct ToneMap
 * @brief Structure representing a tone mapping between a low and high frequency
 * and the corresponding character.
 */
struct ToneMap {
  short low_freq;   // Low frequency
  short high_freq;  // High Frequency
  char tone;        // Corresponding tone character
};

/**
 * @struct Frequency
 * @brief Structure representing a frequency and cosine pair.
 *
 * This structure is used to store a frequency value and its corresponding
 * cosine of the angular frequency.
 */
struct Frequency {
  short frequency;  // Frequency value
  float cosw;       // Cosine of angular frequency
};

typedef struct Frequency Frequency_t;

/**
 * @brief Function returns the DTMF tone symbol given low and high frequencies.
 * @param low_freq The low frequency component of the DTMF tone.
 * @param high_freq The high frequency component of the DTMF tone.
 * @return The corresponding DTMF tone.
 */
char toneDetection(short low_freq, short high_freq);

/**
 * @brief Function calculates the sum of absolute sample values within the
 * specified range.
 * @param samples Pointer to the array of audio samples.
 * @param size The number of samples in the array.
 * @return The sum of absolute sample values within the specified range.
 */
float quietDetection(float* samples, unsigned short size);

/**
 * @brief Computes the magnitude of a signal segment using a filter.
 * @param samples Pointer to the array of audio samples.
 * @param size The number of samples in the array.
 * @param freq The frequency structure representing the filter.
 * @return The magnitude of the signal segment.
 */
float processSample(float* samples, unsigned short size, Frequency_t freq);

/**
 * @brief Decodes the buffer and returns the corresponding DTMF tone.
 * @param buffer Pointer to the array of audio samples.
 * @param size The number of samples in the buffer.
 * @return The corresponding DTMF tone.
 */
char decode(float* buffer, unsigned short size);

#endif  // DECODE_H
