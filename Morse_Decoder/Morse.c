/**
 * @file Morse.c
 *
 * @brief Source code for the Morse Decoder logic.
 *
 * Implements the Morse code lookup table and decoding function.
 *
 * @author Ryan Fuentes
 */

#include "Morse.h"
#include <string.h> // Required for strcmp

// --- The Global Morse Code Map ---
const MorseCodeMap MORSE_MAP[] = {
    // Letters
    {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'},
    {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'},
    {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'},
    {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'},
    {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'},
    {"--..", 'Z'},
    // Numbers
    {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'}, {"....-", '4'},
    {".....", '5'}, {"-....", '6'}, {"--...", '7'}, {"---..", '8'}, {"----.", '9'},
    // Sentinel to mark the end of the array
    {NULL, 0} 
};

char Decode_Morse_Sequence(const char* sequence) {
    if (sequence == NULL || strlen(sequence) == 0) {
        return 0; // Return null if sequence is empty or invalid
    }
    
    // Iterate through the lookup table until the sentinel is found
    for (int i = 0; MORSE_MAP[i].morse != NULL; i++) {
        // Use strcmp to compare the input sequence with the stored Morse string
        if (strcmp(sequence, MORSE_MAP[i].morse) == 0) {
            return MORSE_MAP[i].character;
        }
    }
    
    // Return '?' if the sequence does not match any entry
    return '?'; 
}