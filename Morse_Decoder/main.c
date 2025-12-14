/*
 * @file main.c
 *
 * @brief Main source code for the Morse Code Decoder.
 *
 * Decodes button presses (PD0/SW5) into Morse code and outputs to UART0/VCP.
 *
 * @author Ryan Fuentes
 */

#include "TM4C123GH6PM.h"
#include "GPIO.h"
#include "SysTick_Delay.h"
#include "Morse.h" // Includes constants and decoding function
#include <string.h> 

// --- External Declarations ---
extern void UART0_Init(void);
extern void UART0_OutChar(char data);
extern uint32_t us_elapsed_counter; // The continuous counter from SysTick_Delay.c
// -----------------------------

// Configuration constants
#define BUTTON_PIN 0x01         // SW5 is on PD0

// Global variables for the Morse state machine
static char morse_sequence[MAX_MORSE_LENGTH + 1]; // Buffer for current letter
static uint8_t press_is_active = 0;
static uint32_t press_start_time_us = 0; 
static uint32_t last_release_time_us = 0; 

/**
 * @brief Appends a dot ('.') or dash ('-') to the current sequence buffer.
 */
void Append_Morse_Element(char element) {
    size_t len = strlen(morse_sequence);
    if (len < MAX_MORSE_LENGTH) {
        morse_sequence[len] = element;
        morse_sequence[len + 1] = '\0';
    }
}

/**
 * @brief Decodes the current sequence, outputs the character via UART, and clears the buffer.
 */
void Decode_And_Output(void) {
    if (strlen(morse_sequence) > 0) {
        char decoded_char = Decode_Morse_Sequence(morse_sequence);
        UART0_OutChar(decoded_char);
        
        // Clear the sequence buffer for the next character
        morse_sequence[0] = '\0'; 
    }
}

/**
 * @brief State machine to detect presses, releases, dots, dashes, and gaps.
 */
void Check_Button_And_Gaps(void) {
    uint8_t current_status = Get_EduBase_Button_Status(); // Reads PD0-PD3
    uint8_t button_pressed = (current_status & BUTTON_PIN);
    uint32_t current_time_us = us_elapsed_counter; // Continuous time base

    // --- State 1: Press Detection (Rising Edge) ---
    if (button_pressed && !press_is_active) {
        press_is_active = 1;
        press_start_time_us = current_time_us;
        last_release_time_us = 0; // Stop monitoring the gap

    // --- State 2: Release Detection & Dot/Dash Classification (Falling Edge) ---
    } else if (!button_pressed && press_is_active) {
        press_is_active = 0;
        uint32_t press_duration_us = current_time_us - press_start_time_us;
        uint32_t press_duration_ms = press_duration_us / 1000;

        // Morse Code Timing Thresholds (based on T = 100ms)
        if (press_duration_ms >= (MORSE_UNIT_MS * 0.5) && press_duration_ms < (MORSE_UNIT_MS * 2.5)) {
            // Approx 0.5T to 2.5T: DOT
            Append_Morse_Element('.');
        } else if (press_duration_ms >= (MORSE_UNIT_MS * 2.5) && press_duration_ms < (MORSE_UNIT_MS * 6)) {
            // Approx 2.5T to 6T: DASH
            Append_Morse_Element('-');
        } 
        
        last_release_time_us = current_time_us; // Start monitoring for gap

    // --- State 3: Gap Detection (Idle) ---
    } else if (!button_pressed && !press_is_active && last_release_time_us != 0) {
        // Only run if a key was recently released
        uint32_t gap_duration_us = current_time_us - last_release_time_us;
        uint32_t gap_duration_ms = gap_duration_us / 1000;

        // Gap Timing Thresholds
        
        // Inter-Word Gap (>= 7T): Send character, then a SPACE
        if (gap_duration_ms >= (MORSE_UNIT_MS * 7)) {
            Decode_And_Output();
            UART0_OutChar(' '); 
            // Reset gap timer to avoid continuous output of spaces
            last_release_time_us = current_time_us; 
            
        // Inter-Character Gap (>= 3T): Send character
        } else if (gap_duration_ms >= (MORSE_UNIT_MS * 3)) {
            Decode_And_Output();
            // Reset gap timer to wait for the next key-up event
            last_release_time_us = current_time_us; 
        }
    }
}

int main(void)
{
    // 1. Initialization
    SysTick_Delay_Init();       // Initializes SysTick for continuous 1us timing
    EduBase_Button_Init();      // Initializes PD0 (SW5) as input
    UART0_Init();               // Initializes UART for serial output
    
    // Set initial state
    morse_sequence[0] = '\0';
    
    // 2. Main Execution Loop
    while(1)
    {
        // Continuously check button state and timing logic
        Check_Button_And_Gaps();
        
        // Since SysTick is running independently, the loop can run at full speed.
        // The speed of this loop determines the resolution of the button status check,
        // but the timing measurement is handled by the high-resolution SysTick counter.
    }
}