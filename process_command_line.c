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
#include <unistd.h>  
#include <string.h>

#include "morse.h"

#define TOTAL_WORD_BITS 50
#define STANDARD_WORD_BITS 31
#define FARNSWORTH_WORD_BITS 19
#define SECONDS 60.0

#define TEN_E6 1000000L

void display_help(void)
{
    printf("Morse may be called with command line options\n\n");
    printf("      -s Sets the speed of Morse Code in word per minute WPM, from 1 - 75.\n");
    printf("      -t Sets Standard Morse coding or Farnsworth Morse coding \"S or F\".\n");
    printf("      -l (\"lower case L\") Sets the loop count, 0 loops forever anything else up to 1000\n         loops that many times.\n");
    printf("      -p Sets the port used to activate the LED light. It's preset to 8.\n");
    printf("      -f Sets the text file to be set as Morse code. File paths are allowed (expected).\n");
    printf("      -h or -H displays this text.\n\n");
    printf(" \"$ morse -s5 -tS -l1 -fexample.txt\"\n");
    printf("Sets the:\n    Speed at 5WPM\n    Standard Morse code timing\n    1 loop\n");
    printf("    Uses the \"example.txt\" to be set via the LED light as Morse code flashes\n");
    printf("\n\n");
    printf("There is a .morsecode.cfg file in the users directory that can set these options as standard so \nyou don't have to set them each time.  You can use the text editor to edit the file.\n");
    return;
}

  
void process_command_line(int argc, char *argv[], struct start_options *options)
{
    int opt;

    // put ':' in the starting of the 
    // string so that program can  
    //distinguish between '?' and ':'  
    while((opt = getopt(argc, argv, ":f:Hhl:p:s:t:p:")) != -1)  
    {  
        switch(opt)  
        {  
            case 'H':
            case 'h':
                display_help();
                exit(-2);
                break;
            case 'f':  
                options->filename = optarg;
                break;  
            case 'l':
                options->loop = atoi(optarg);
                break;
            case 's':
                options->speed = atoi(optarg);
                break;
            case 'p':
                options->port = atoi(optarg);
                break;
            case 't':
                if (*optarg == 'f' || *optarg == 'F'){
                    options->timing_type='F';
                } else if (*optarg == 's' || *optarg == 'S'){
                    options->timing_type='S';
                }                
                break;
            case ':':  
                printf("option needs a value\n");
                exit(-1);  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt);
                display_help();
                exit(-2); 
                break;  
        }  
    }  
    // optind is for the extra arguments 
    // which are not parsed 
    for(; optind < argc; optind++){
        options->filename = argv[optind];
        printf("extra arguments: %s\n", options->filename);  
    }

    /* 
     * Some final checks, file name must be set or it's an error, 
     * Farnsworth timing only used below 18 WPM
     */
    if (options->filename == NULL){
        printf("The file to open must be supplied on the command line either with -f filename or just filename\n");
        exit(-1);
    }      
    if (options->speed < 1 || options->speed > 75) {
        printf("Speed settings must be between 1 and 75 WPM, Speed requested was %d\n", options->speed);
        exit(-1);
    }
    if (!(options->timing_type =='F' || options->timing_type == 'S')){
        printf("Morse Code timing '-t' option must be S for standard or F for Farnsworth\n");
        exit(-1);
    }
    if (options->loop < 0 || options->loop > 10000){
        printf("Display loop must be set from 0 to 10,000\n");
        printf("0 = loop forever\n1 - 10,000 display the message that many times\n");
        exit(-1);
    }
    if (options->port < 0 || options->port >53){
        printf("GPIO port must be between 0 and 53\n");
        exit(-1);
    }

    if (options->timing_type=='F' && options->speed >=18){
        options->timing_type='S';
        printf("Farnsworth timing asked for but speed set to >= 18, Timing set to Standard instead\n");
        printf("Farnsworth timing is only applicable below 18 Words Per Minute (WPM)\n");
    }

    if (options->timing_type == 'S'){
        // dot time could be longer then 1 second which takes special handeling 
        // because the timing fuctions do seconds and microseconds, but microseconds
        // are less then 1 seconds worth, can't do 2 seconds in microseconds!!
        options->dot_time = SECONDS / (float)(options->speed * TOTAL_WORD_BITS);
        // Cast the float to int and drop the float part
        options->dot_time_seconds = (unsigned int) options->dot_time;
        // Not subtract the int from the float and drop the whole number 
        options->dot_time -= options->dot_time_seconds;

        options->inter_word_dot_time = SECONDS / (float)(options->speed * TOTAL_WORD_BITS);
        options->inter_word_dot_time_seconds = (unsigned int) options->inter_word_dot_time;
        options->inter_word_dot_time -= options->inter_word_dot_time_seconds;
        // Calculate the microseconds for the utimer function
        options->dot_time_micro = options->dot_time * TEN_E6;
        options->inter_word_dot_time_micro = options->inter_word_dot_time * TEN_E6;
         
    } else {
        options->dot_time = SECONDS / (float)(18 * TOTAL_WORD_BITS); // dot's are transmitted at 18 WPM
        options->dot_time_seconds = (unsigned int) options->dot_time;
        options->dot_time -= options->dot_time_seconds;
        options->dot_time_micro = options->dot_time * TEN_E6;

        options->inter_word_dot_time = (SECONDS-(options->dot_time*(float)(STANDARD_WORD_BITS*options->speed)))/(options->speed*FARNSWORTH_WORD_BITS);
        options->inter_word_dot_time_seconds = (unsigned int) options->inter_word_dot_time;
        options->inter_word_dot_time -= options->inter_word_dot_time_seconds;
        options->inter_word_dot_time_micro = options->inter_word_dot_time * TEN_E6;
    }
    return;
}
