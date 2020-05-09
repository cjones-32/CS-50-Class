#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    typedef uint8_t BYTE;
    
    //declare ints and buffer
    int file_count = 0;
    char file_name[8];
    bool jpg_found = false;
    FILE *new_jpg = NULL;
    //just cause
    BYTE *buffer = malloc(512);
    
    //if there is not exactly one argument, end
    if (argc != 2)
    {
        printf("Usage: file to recover");
        return 1;
    }
    //open the file
    FILE *raw_file = fopen(argv[1], "r");
    //if file does not exist, end
    if (raw_file == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    
    //file exists, read it till end
    while (fread(buffer, 512, 1, raw_file) == 1)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] | 0x0f) == 0xef)
        {
            //file found, first jpg
            if (!jpg_found)
            {
                jpg_found = true;
                sprintf(file_name, "%03i.jpg", file_count);
                new_jpg = fopen(file_name, "w");
                fwrite(buffer, 512, 1, new_jpg);
                file_count++;
            }
            //file found, not first jpg.
            else
            {
                fclose(new_jpg);
                sprintf(file_name, "%03i.jpg", file_count);
                FILE *img = fopen(file_name, "w");
                fwrite(buffer, 512, 1, new_jpg);
                file_count++;
            }
        }
        //if was found but not in this block, keep writing
        else if (jpg_found)
            fwrite(buffer, 512, 1, new_jpg);
    }
    //close everything
    if (jpg_found)
        fclose(new_jpg);
    fclose(raw_file);
    free(buffer);
}
