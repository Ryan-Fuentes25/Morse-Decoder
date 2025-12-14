/**
 * @file Morse.h
 *
 * @brief Header file for the Morse Decoder logic.
 *
 * Defines the Morse code map and timing constants.
 *
 * @author Ryan Fuentes
 */

#include <stdint.h>
#include <stddef.h> // For size_t

// Timing constants for decoder logic
#define MORSE_UNIT_MS 100       // T = 100 milliseconds
#define MAX_MORSE_LENGTH 6      // Max dots/dashes for a single character (e.g., 'C' is 4, numbers are 5)

/**
 * @brief Structure to map a Morse code sequence string to a character.
 */
typedef struct {
    const char* morse;
    char character;
} MorseCodeMap;

/**
 * @brief Global Morse code map lookup table.
 */
extern const MorseCodeMap MORSE_MAP[];

/**
 * @brief Decodes a sequence of dots and dashes into its corresponding character.
 *
 * @param sequence The null-terminated string of Morse code (e.g., ".-.").
 *
 * @return The decoded character, or '?' if the sequence is unknown.
 */
char Decode_Morse_Sequence(const char* sequence);