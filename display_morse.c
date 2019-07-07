/*
 * morse, it will display text files via Morse Code
 *
 * Copyright (C) 2019  David I. S. Mandala
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; version 2 of the License.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * David I. S. Mandala davidm@them.com
 * 120 E. FM 544
 * Suite 72, BX 107
 * Murphy, TX 75094
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "morse.h"
#include "morse_code.h"

#ifdef RASPBERRY_PI
#include <pigpiod_if2.h>
#define ON 1
#define OFF 0
#endif

void timer_wait(int cnt, unsigned int seconds, unsigned int micros)
{
    int loop;
    for (loop=0;loop<cnt;loop++){
        // Set a timer and sleep for a 7 dots
        if(seconds){
            sleep(seconds);
        }
        usleep((useconds_t)micros);
    }

return;
} 

void intra_character_wait(struct start_options options)
{
#ifdef CONSOLE_MORSE
    printf("~"); // Equal a single dot timing, not effected by Farnsworth timing.
#endif
    // Set a timer and sleep for a single dot
    timer_wait(1, options.dot_time_seconds, options.dot_time_micro);
}

void inter_character_wait(struct start_options options)
{
#ifdef CONSOLE_MORSE
    if (options.timing_type == 'S'){
        printf("[~]"); // Equal a standard 3 dot timing
    } else { // It's Farnsworth timing
        printf("[+]"); // Equal a farnsworth 3 dot timing
    }
#endif
    // Set a timer and sleep for a 3 dots
    timer_wait(3, options.inter_word_dot_time_seconds, options.inter_word_dot_time_micro);
    return;
}

void word_wait(struct start_options options)
{
#ifdef CONSOLE_MORSE
    if (options.timing_type == 'S'){
        printf("[~~~~~]"); // Equal a standard 7 dot timing
    } else { // It's Farnsworth timing
        printf("[+++++]"); // Equal a Farnsworth 7 dot timing
    }
    fflush(stdout);
#endif
    // Set a timer and sleep for a 7 dots
    timer_wait(7, options.inter_word_dot_time_seconds, options.inter_word_dot_time_micro);
    return;
}

void display_dash(struct start_options options)
{
#ifdef RASPBERRY_PI
    // Turn LED ON NOW
    gpio_write(options.gpio_pi, (unsigned)options.port, ON);
#endif
    // Set a timer and sleep for a three dots
    timer_wait(3, options.dot_time_seconds, options.dot_time_micro);
#ifdef RASPBERRY_PI
    // Turn LED OFF NOW
    gpio_write(options.gpio_pi, (unsigned)options.port, OFF);
#endif
    return;
}

void display_dot(struct start_options options)
{
#ifdef RASPBERRY_PI
    // Turn LED ON NOW
    gpio_write(options.gpio_pi,(unsigned)options.port, ON);
#endif
    // Set a timer and sleep for a single dot
    timer_wait(1, options.dot_time_seconds, options.dot_time_micro);
    usleep((useconds_t)options.dot_time_micro);
#ifdef RASPBERRY_PI
    // Turn LED OFF NOW
    gpio_write(options.gpio_pi, (unsigned)options.port, OFF);
#endif
    return;
}

void flash_morse_char(char letter, struct start_options options)
{
    char *morse_code_string;
    
    morse_code_string = morse_code[(int)letter];
#ifdef CONSOLE_MORSE
        printf("(%c)", letter);
#endif    
    while (*morse_code_string){
#ifdef CONSOLE_MORSE
        printf("%c", *morse_code_string);
#endif
        if (*morse_code_string == '.'){
            display_dot(options);
        } else { // it's a "-"
            display_dash(options);
        }
        if (*++morse_code_string){
            intra_character_wait(options);
        }
    }
    return;
}

/*
 * This function displays the message in morse code, the start_options
 * struct stores the timing info needed to display the code depending on
 * options selected at the start.
 */
 
void display_morse(struct start_options options) {
    long loop;
    char *pstring;

    for (loop = 0, pstring = options.message; loop < options.fileInfo.st_size; loop++){
        // If it's a space or a \n then do a word wait, not a inter_char wait
        if (*pstring == ' ' || *pstring == '\n'){
            word_wait(options);
            pstring++;
        } else {
            // Send a letter and advance string to next letter
            flash_morse_char(*pstring++, options);
            // If it's not a space or a \n then wait inter_char time
            if (!(*pstring == ' ' || *pstring =='\n')){
                inter_character_wait(options);
            }
        }
    }
    return;
}

void display_message(struct start_options options) {
    int loop;

    /* The normal use of this software is to loop forever so if the 
     * options.loop variable is set to 0 loop forever, otherwise loop
     * options.loop times, whatever it's set to.
     */
    if (options.loop == 0){
        while(1){
            display_morse(options);
#ifdef CONSOLE_MORSE
            printf("\n");
#endif
        }
    } else if (options.loop > 0){
        for (loop=0;options.loop>loop;loop++){
            display_morse(options);
#ifdef CONSOLE_MORSE
            printf("\n");
#endif
        }
    }
    return;
}

