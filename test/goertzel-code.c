double quietDetection(double* samples, int start, int end) {
  double amplitudesum = 0;
  int i;
  for (i = start; i < end; i++) {
    // Ensure samples are positive
    double sample = samples[i] < 0 ? -samples[i] : samples[i];
    amplitudesum += sample;
  }
  return amplitudesum;
}

struct ToneMap {
  int low_freq;
  int high_freq;
  char tone;
};

struct ToneMap TONEMAP[]
    = { { 697, 1209, '1' }, { 697, 1336, '2' }, { 697, 1477, '3' },
        { 697, 1633, 'A' }, { 770, 1209, '4' }, { 770, 1336, '5' },
        { 770, 1477, '6' }, { 770, 1633, 'B' }, { 852, 1209, '7' },
        { 852, 1336, '8' }, { 852, 1477, '9' }, { 852, 1633, 'C' },
        { 941, 1209, '*' }, { 941, 1336, '0' }, { 941, 1477, '#' },
        { 941, 1633, 'D' } };

char toneDetection(int low_freq, int high_freq) {
  int i;
  for (i = 0; i < sizeof(TONEMAP) / sizeof(struct ToneMap); i++) {
    if (TONEMAP[i].low_freq == low_freq && TONEMAP[i].high_freq == high_freq) {
      return TONEMAP[i].tone;
    }
  }
  return '\0';  // Return null character if not found
}