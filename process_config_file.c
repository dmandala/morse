#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

#include <confuse.h>

#include "morse.h"

void process_config_options(long int loop, long int speed, long int port, char * filename, char * type, struct start_options *options)
{
    if ( loop >= 0){
        options->loop = (int) loop;
    }
    if ( speed >= 0){
        options->speed = (int) speed;
    }
    if ( port >= 0){
        options->port = (int) port;
    }
    if (filename != NULL){
        if (options->filename != NULL){
            free(options->filename);
        }
        options->filename = strdup(filename);
        free(filename);
    }
    if (type != NULL){
        if (*type == 'f' || *type == 'F'){
            options->timing_type='F';
        } else if (*type == 's' || *type == 'S'){
            options->timing_type='S';
        }else{
            printf("Morse Code timing '-t' option must be S for standard or F for Farnsworth\n");
            exit(-1);
        }
        free(type);
    }
    return;
}

void load_config_file(char * file_to_load, struct start_options *options)
{
    long int loop;
    long int speed;
    long int port;
    char * filename;
    char *type;

    cfg_opt_t opts[] = {
        CFG_SIMPLE_STR("type", &type), 
        CFG_SIMPLE_STR("filename", &filename), 
        CFG_SIMPLE_INT("loop", &loop),
        CFG_SIMPLE_INT("speed", &speed),
        CFG_SIMPLE_INT("port", &port),
        CFG_END()
    }; cfg_t *cfg;

#ifdef LC_MESSAGES
    setlocale(LC_MESSAGES, "");
    setlocale(LC_CTYPE, "");
#endif

    /* 
     * Preset the values here so we can tell if they have changed, ints to
     * -1 strings to NULL
     */
     
    loop = -1;
    speed = -1;
    port = -1;
    filename = NULL;
    type = NULL;
    
    // Now setup to load a config file
    cfg = cfg_init(opts, 0);
    if (cfg_parse(cfg, file_to_load) != CFG_PARSE_ERROR){
        // Config file loaded, now process what was in it.
        process_config_options(loop, speed, port, filename, type, options);
    }
    cfg_free(cfg);
    return;
}

void process_config_file(struct start_options *options)
{
    // First load config if present
    load_config_file(ETC_FILE_PATH_AND_NAME, options);
    // Next load .config if present to override /etc settings
    {
        uid_t uid = getuid();
        struct passwd *pw = getpwuid(uid);

        if (pw == NULL) {
            printf("Failed\n");
            exit(EXIT_FAILURE);
        }
        {
            char *dot_file_name = DOT_FILE_NAME;
            char *dot_file_to_open = malloc(strlen(pw->pw_dir)+strlen(dot_file_name) + 2);
            strcpy(dot_file_to_open, pw->pw_dir);
            strcat(dot_file_to_open, "/");
            strcat(dot_file_to_open,dot_file_name);
            load_config_file(dot_file_to_open, options);
            free(dot_file_to_open);
        }
    }
return;
}
