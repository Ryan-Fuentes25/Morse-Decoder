// morse_decoder.c

#include "morse_decoder.h"
#include <string.h>
#include <stddef.h> // Include for size_t, though string.h usually provides it

// NOTE: EduBase_LCD.h is intentionally excluded here.

// --- Global Variable Definitions ---
// These strings hold the current input sequence and the fully decoded message.
char current_morse_sequence[MAX_MORSE_LENGTH + 1] = ""; 
char decoded_message[32] = ""; 

// --- Morse Code Dictionary ---
// Defines the mapping from Morse sequence (dots/dashes) to ASCII characters.
const char* MORSE_CODE_DICT[] = {
    "",     // 0: Placeholder
    ".-",   // 1: A
    "-...", // 2: B
    "-.-.", // 3: C
    "-..",  // 4: D
    ".",    // 5: E
    "..-.", // 6: F
    "--.",  // 7: G
    "....", // 8: H
    "..",   // 9: I
    ".---", // 10: J
    "-.-",  // 11: K
    ".-..", // 12: L
    "--",   // 13: M
    "-.",   // 14: N
    "---",  // 15: O
    ".--.", // 16: P
    "--.-", // 17: Q
    ".-.",  // 18: R
    "...",  // 19: S
    "-",    // 20: T
    "..-",  // 21: U
    "...-", // 22: V
    ".--",  // 23: W
    "-..-", // 24: X
    "-.--", // 25: Y
    "--..", // 26: Z
    "-----", // 27: 0
    ".----", // 28: 1
    "..---", // 29: 2
    "...--", // 30: 3
    "....-", // 31: 4
    ".....", // 32: 5
    "-....", // 33: 6
    "--...", // 34: 7
    "---..", // 35: 8
    "----.", // 36: 9
};

// --- Decoder Logic Implementation ---

char decode_morse_char(const char *morse_sequence) {
    if (strlen(morse_sequence) == 0) return ' ';

    // A-Z lookup
    for (int i = 1; i <= 26; i++) {
        if (strcmp(morse_sequence, MORSE_CODE_DICT[i]) == 0) {
            return 'A' + (i - 1); 
        }
    }
    
    // 0-9 lookup
    for (int i = 27; i < DICT_SIZE; i++) {
        if (strcmp(morse_sequence, MORSE_CODE_DICT[i]) == 0) {
            return '0' + (i - 27);
        }
    }

    // Return '?' for unrecognized sequences
    return '?'; 
}

void handle_character_end(void) {
    // 1. Decode the current sequence
    char decoded_char = decode_morse_char(current_morse_sequence);
    
    // FIX: Change 'int len' to 'size_t len'
    size_t len = strlen(decoded_message);
    
    // 2. Append the decoded character to the message buffer
    // sizeof(decoded_message) is 32. We check for < 31 to leave room for the null terminator.
    if (len < sizeof(decoded_message) - 1) { 
        decoded_message[len] = decoded_char;
        decoded_message[len + 1] = '\0';
    }
    
    // 3. Clear the current Morse sequence for the next character input
    current_morse_sequence[0] = '\0'; 
    
    // No LCD calls here. main.c will check 'decoded_message' and update the display.
}

void handle_word_end(void) {
    // FIX: Change 'int len' to 'size_t len'
    size_t len = strlen(decoded_message);
    
    // 1. Append a space to the message, but only if the last character is NOT already a space
    if (len > 0 && decoded_message[len - 1] != ' ') { 
        if (len < sizeof(decoded_message) - 1) { 
            decoded_message[len] = ' ';
            decoded_message[len + 1] = '\0';
        }
    }
    // No LCD calls here. main.c will check 'decoded_message' and update the display.
}