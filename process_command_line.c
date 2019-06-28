// Program to illustrate the getopt() 
// function in C 
  
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
  
void process_command_line(int argc, char *argv[], struct start_options *options)
{
    int opt;

    // put ':' in the starting of the 
    // string so that program can  
    //distinguish between '?' and ':'  
    while((opt = getopt(argc, argv, ":l:s:t:f:p:")) != -1)  
    {  
        switch(opt)  
        {  
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
                exit(-1); 
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
    if (options->speed < 1 || options->speed > 50) {
        printf("Speed settings must be between 1 and 50 WPM, Speed requested was %d\n", options->speed);
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
        options->dot_time = SECONDS / (float)(options->speed * TOTAL_WORD_BITS);
        options->inter_word_dot_time = options->dot_time;
        options->dot_time_micro = options->inter_word_dot_time_micro = options->dot_time * TEN_E6; 
    } else {
        options->dot_time = SECONDS / (float)(18 * TOTAL_WORD_BITS); // dot's are transmitted at 18 WPM
        options->dot_time_micro = options->dot_time * TEN_E6;
        options->inter_word_dot_time = (SECONDS-(options->dot_time*(float)(STANDARD_WORD_BITS*options->speed)))/(options->speed*FARNSWORTH_WORD_BITS);
        options->inter_word_dot_time_micro = options->inter_word_dot_time * TEN_E6;
    }
    return;
}
