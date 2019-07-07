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
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include<signal.h>

#include "morse.h"

#ifdef RASPBERRY_PI
#include <pigpiod_if2.h>
#endif

static struct start_options options;

#define OFF 0

void sig_handler(int signo)
{
    if (signo == SIGINT){
        close_text_file(options);
#ifdef RASPBERRY_PI
    // All done, close the gpio code
        gpio_write(options.gpio_pi, (unsigned)options.port, OFF);
        pigpio_stop(options.gpio_pi);
#endif
        exit(0);
  }
}

int main(int argc, char* argv[]) {
//    struct start_options options;
    // Set some sane values to the options struct.
    memset(&options, 0, sizeof(options));
    options.timing_type = 'S';
    options.speed = 1;
    options.dot_time = 60.0 / (options.speed * 50.0); // A word is defined as 50 dots
    options.inter_word_dot_time = options.dot_time;
    options.loop=0;
    options.port = 24;

    if (signal(SIGINT, sig_handler) == SIG_ERR){
        printf("\ncan't catch SIGINT, exiting\n");
        exit(-3);
    }
    // get any and all config file options
    process_config_file(&options);
    // now process any command line arguments
    process_command_line(argc, argv, &options);

#ifdef CONSOLE_MORSE
    printf("Options: filename = %s, timing type %c, WPM = %d, loop %d, port %d\n", 
                   options.filename, options.timing_type, options.speed, options.loop, options.port);
#endif

#ifdef RASPBERRY_PI
    // We are on a RASPBERRY PI so setup to use the GPIO to display morsecode
    if ((options.gpio_pi = pigpio_start(NULL, NULL)) < 0){
        printf("Could not initilize Raspberry Pi GPIO library\n");
        exit(-1);
    }
    // The hardware I build uses GPIO24 but you could use anyport that you are hooked to
    set_mode(options.gpio_pi, options.port, PI_OUTPUT); // Set the gpio as output.
    set_pull_up_down(options.gpio_pi, options.port, PI_PUD_UP); // Set a pull-up on the gpio.
#endif

    open_text_file(&options);
    display_message(options);
    close_text_file(options);

#ifdef RASPBERRY_PI
    // All done, close the gpio code
    pigpio_stop(options.gpio_pi);
#endif
    return(0);
}
