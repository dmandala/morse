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

