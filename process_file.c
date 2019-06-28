#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include "morse.h"

void close_text_file(struct start_options options)
{
    // Don't forget to free the mmapped memory
    if (munmap(options.message, options.fileInfo.st_size) == -1)
    {
        close(options.fd);
        perror("Error un-mmapping the file");
        exit(EXIT_FAILURE);
    }

    // Un-mmaping doesn't close the file, so we still need to do that.
    close(options.fd);
    return;
}


void open_text_file(struct start_options *options)
{    
    options->fd = open(options->filename, O_RDONLY, (mode_t)0600);
    
    if (options->fd == -1)
    {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }        
    
    if (fstat(options->fd, &options->fileInfo) == -1)
    {
        perror("Error getting the file size");
        exit(EXIT_FAILURE);
    }
    
    if (options->fileInfo.st_size == 0)
    {
        fprintf(stderr, "Error: File is empty, nothing to do\n");
        exit(EXIT_FAILURE);
    }
    
    options->message = mmap(0, options->fileInfo.st_size, PROT_READ, MAP_SHARED, options->fd, 0);
    if (options->message == MAP_FAILED)
    {
        close(options->fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    return;
}

