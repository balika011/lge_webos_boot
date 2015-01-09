#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER 256

char header[] = 
"/*\n \
* (C) Copyright 2010\n \
* MediaTek, Inc.\n \
*\n \
* See file CREDITS for list of people who contributed to this\n \
* project.\n \
*\n \
* This program is free software; you can redistribute it and/or\n \
* modify it under the terms of the GNU General Public License as\n \
* published by the Free Software Foundation; either version 2 of\n \
* the License, or (at your option) any later version.\n \
*\n \
* This program is distributed in the hope that it will be useful,\n \
* but WITHOUT ANY WARRANTY; without even the implied warranty of\n \
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the\n \
* GNU General Public License for more details.\n \
*\n \
* You should have received a copy of the GNU General Public License\n \
* along with this program; if not, write to the Free Software\n \
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,\n \
* MA 02111-1307 USA\n \
*/";


int main(int argc, char *argv[])
{
    unsigned int current_name = 1;
    char buffer[BUFFER], previous;
    char buffer2[BUFFER];

    do
    {
        int current_file = -1, current_tmp_file = -1;
        int not_first = 0, line_comment = 0, multi_comment = 0, quote = 0,
            escape = 0, to_print = 0, I = 0;
        ssize_t buffered_size;

        // open each file 
        if (argc < 2) 
        {
            current_file = STDIN_FILENO;
        } 
        else 
        {
            current_file = open(argv[current_name], O_RDONLY);
            if (current_file < 0)
            {
                fprintf(stderr, "%s: error opening file '%s': %s\n", argv[0],
                  argv[current_name++], strerror(errno));
                continue;
            }

            
            sprintf(buffer, "%s_tmp", argv[current_name]);
            current_tmp_file = open(buffer, O_WRONLY | O_CREAT | O_TRUNC);
            if (current_tmp_file < 0)
            {
                fprintf(stderr, "%s: error opening file '%s': %s\n", argv[0],
                  buffer, strerror(errno));
                continue;
            }
        
        }          

        sprintf(buffer, "\n");
        write(current_tmp_file, header, sizeof(header) - 1);
        write(current_tmp_file, buffer, 1);

        while ((buffered_size = read(current_file, buffer, BUFFER)) != 0)
        {
            if (buffered_size == (ssize_t) -1)
            {
                if (errno == EINTR) 
                {
                    continue;
                } 
                else
                {
                    break;
                } 
            }


            for (I = 0; I < (signed) buffered_size; I++)
            {
                if (escape)
                {
                    if (not_first && to_print)
                    {
                        if(previous != '\r')
                        {
                            write(current_tmp_file, &previous, 1);
                        }
                    } 
                    previous = buffer[I];
                    to_print = !line_comment && !multi_comment;
                    escape = 0;
                    if (++I >= buffered_size) continue;
                }

                if (buffer[I] == '\\')
                {
                    if (not_first && to_print)
                    {
                        if(previous != '\r')
                        {
                            write(current_tmp_file, &previous, 1);
                        }
                    } 
                    previous = buffer[I];
                    to_print = !line_comment && !multi_comment;
                    escape = 1;
                    continue;
                }
                else
                {
                    escape = 0;
                } 


                if (quote)
                {
                    if(previous != '\r')
                    {
                        write(current_tmp_file, &previous, 1);
                    }
                    quote = !(buffer[I] == '\"');
                    to_print = 1;
                }
                else if (line_comment) 
                {
                    line_comment = !(to_print = (buffer[I] == '\n'));
                }
                else if (multi_comment)
                {
                    to_print = 0;
                    if (buffer[I] == '/' && previous == '*') 
                    {
                        multi_comment = 0;
                    } 
                }
                else switch (buffer[I])
                {
                case '/': 
                    if (not_first && previous == '/')
                    {
                        to_print     = 0;
                        line_comment = 1;
                    }
                    else
                    {
                        if (not_first && to_print)
                        {
                            if(previous != '\r')
                            {
                                write(current_tmp_file, &previous, 1);
                            }
                        } 
                        to_print = 1;
                    }
                    break;

                case '*': 
                    if (not_first && previous == '/')
                    {
                        to_print      = 0;
                        multi_comment = 1;
                    }
                    else
                    {
                        if (not_first && to_print)
                        {
                            if(previous != '\r')
                            {
                                write(current_tmp_file, &previous, 1);
                            }
                        } 
                        to_print = 1;
                    }
                    break;

                case '\"': 
                    quote = 1;
                    if (not_first && to_print)
                    {
                        if(previous != '\r')
                        {
                            write(current_tmp_file, &previous, 1);
                        }
                    }
                    to_print = 1;
                    break;

                default: 
                    if (not_first && to_print) 
                    {
                        if(previous != '\r')
                        {
                            write(current_tmp_file, &previous, 1);
                        }
                    }
                    to_print = 1;
                    break;
                }

                not_first = 1;
                previous  = buffer[I];
            }
        }

        if (not_first) 
        {
            if(previous != '\r')
            {
                write(current_tmp_file, &previous, 1);
            }
        }

        if(current_file >= 0) 
        {
            close(current_file);
        }
        if(current_tmp_file >= 0) 
        {
            close(current_tmp_file);
        }        

        sprintf(buffer2, "rm -f %s", argv[current_name]);
        system(buffer2);

        sprintf(buffer, "%s_tmp", argv[current_name]);
        sprintf(buffer2, "mv -f %s %s", buffer, argv[current_name]);
        system(buffer2);
        
        sprintf(buffer2, "chmod 666 %s", argv[current_name]);
        system(buffer2);
        
        current_name++;
    }
    while (current_name < argc);

    return 0;
}

