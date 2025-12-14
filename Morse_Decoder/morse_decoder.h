// morse_decoder.h

#ifndef MORSE_DECODER_H
#define MORSE_DECODER_H

#include <stdint.h>
#include <stdbool.h>
#include "SysTick_Delay.h" // For access to ms_elapsed_since_input
#include "GPIO.h"          // For button function prototypes

// NOTE: EduBase_LCD.h is intentionally excluded here.

// --- Global Constants and Definitions ---

#define MAX_MORSE_LENGTH    5       
#define CHAR_END_TIMEOUT_MS 500     // Pause to decode one character
#define WORD_END_TIMEOUT_MS 1000    // Pause to insert a space
#define DICT_SIZE           37      

// --- Global Variables (Extern) ---

// Current sequence being entered (e.g., ".-.")
extern char current_morse_sequence[MAX_MORSE_LENGTH + 1]; 
// Decoded message to be displayed on the LCD. main.c will read this.
extern char decoded_message[32]; 

// --- Function Prototypes ---

/**
 * @brief Decodes a single Morse code string into a character.
 * @param morse_sequence The string of dots and dashes (e.g., ".-.").
 * @return The decoded character (uppercase/digit), or '?' if not found.
 */
char decode_morse_char(const char *morse_sequence);

/**
 * @brief Decodes the current sequence and appends the character to the message.
 * DOES NOT call LCD functions.
 */
void handle_character_end(void);

/**
 * @brief Inserts a space (' ') into the decoded message.
 * DOES NOT call LCD functions.
 */
void handle_word_end(void);

#endif // MORSE_DECODER_H