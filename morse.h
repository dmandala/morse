#ifndef MORSE_C
#define MORSE_C

#define RASPBERRY_PI
#define CONSOLE_MORSE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

struct start_options {
    char timing_type;			// Standard or Farnsworth
    int loop;				// 0 loop forever 1 -10,000 otherwise
    int speed;				// Words Per Minute
    float dot_time;			// Calculated at 60 / (50 dots per word * words)
    unsigned int dot_time_micro;	// how long is the dot in micro seconds?
    float inter_word_dot_time;		// Different then dot time if Farnsworth applied
    unsigned int inter_word_dot_time_micro;// how long in micro seconds
    int fd;
    struct stat fileInfo;
    char *filename;			// Text file to open
    char * message;			// Pointer to the text to send
    int gpio_pi;
    int port;
    };

extern void display_message(struct start_options options);
extern void process_config_file(struct start_options *options);
extern void process_command_line(int argc, char *argv[], struct start_options *options);
extern void open_text_file(struct start_options *options);
extern void close_text_file(struct start_options options);

#define DOT_FILE_NAME ".morsecode.cfg"
#define ETC_FILE_PATH_AND_NAME "/etc/morsecode.cfg"

#endif