#ifndef DTMF_H
#define DTMF_H

#define PI (3.1415927) // Defining estimate of PI

// Structure representing a tone mapping between frequencies and corresponding
// characters
struct ToneMap
{
    int low_freq;  // Frequency 1 (Low)
    int high_freq; // Frequency 2 (High)
    char tone;     // Corresponding tone character
};

// Definition of the MyObject structure representing frequency and cosine/sine
// values
typedef struct Frequency
{
    int frequency; // Frequency value
    double cosw;   // Cosine of angular frequency
    double sinw;   // Sine of angular frequency
} Frequency_t;

// Array of ToneMap structures representing the mapping between frequencies and
// characters
static struct ToneMap TONEMAP[] = {{697, 1209, '1'}, {697, 1336, '2'}, {697, 1477, '3'}, {697, 1633, 'A'}, {770, 1209, '4'}, {770, 1336, '5'}, {770, 1477, '6'}, {770, 1633, 'B'}, {852, 1209, '7'}, {852, 1336, '8'}, {852, 1477, '9'}, {852, 1633, 'C'}, {941, 1209, '*'}, {941, 1336, '0'}, {941, 1477, '#'}, {941, 1633, 'D'}};

static Frequency_t low[] = {
    {
        697,                // FREQ
        0.8538689046744509, // COS
        0.5204881301528917, // SIN
    },
    {
        770,
        0.8226405180208598,
        0.5685618507342639,
    },
    {
        852,
        0.78434349193341,
        0.620326757976556,
    },
    {
        941,
        0.7391022841200879,
        0.6735932107796729,
    },
};

static Frequency_t high[] = {
    {
        1209,               // FREQ
        0.5820520115136658, // COS
        0.8131515577633086, // SIN
    },
    {
        1336,
        0.49818510533949084,
        0.86707070116449,
    },
    {
        1477,
        0.3993091946133181,
        0.9168163213519179,
    },
    {
        1633,
        0.284266353387601,
        0.9587453469674393,
    },
};

// Function declarations
char toneDetection(int low_freq, int high_freq);
double quietDetection(int *samples, int size);
void init(Frequency_t *self, double frequency);
double processSample(int *samples, int size, Frequency_t freq);

#endif // DTMF_H
