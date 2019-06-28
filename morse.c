#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

#include "morse.h"

#ifdef RASPBERRY_PI
#include <pigpiod_if2.h>
#endif

int main(int argc, char* argv[]) {
    struct start_options options;
    // Set some sane values to the options struct.
    memset(&options, 0, sizeof(options));
    options.timing_type = 'S';
    options.speed = 1;
    options.dot_time = 60.0 / (options.speed * 50.0); // A word is defined as 50 dots
    options.inter_word_dot_time = options.dot_time;
    options.loop=0;
    options.port = 24;
    
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
