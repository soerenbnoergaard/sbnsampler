#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include "file_helpers.h"

#define MAX_PATH_LENGTH 256

static char fh_path[MAX_PATH_LENGTH] = {'\0'};

int32_t fh_set_path(const char *path)
{
    if (strncpy(fh_path, path, MAX_PATH_LENGTH) == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}

int32_t fh_free(binary_data_t *buffer)
{
    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
    return 0;
}

//Returns a binary_data_t structure if reading the file was OK.
//In case of an error it always returns NULL.
binary_data_t *fh_read_file(const char *filename) {

    //Allocated our binary data structure
    binary_data_t *binary_data = malloc(sizeof(binary_data_t));
    if (binary_data != NULL) {

        binary_data->size = 0;
        void *buffer = NULL;
        long position;
        char path_and_file[MAX_PATH_LENGTH] = {'\0'};
        
        snprintf(path_and_file, MAX_PATH_LENGTH, "%s/%s", fh_path, filename);

        //Open the file for reading in binary mode
        FILE *fIn = fopen(path_and_file, "rb");

        if (fIn != NULL) {
            //Go to the end of the file
            const int fseek_end_value = fseek(fIn, 0, SEEK_END);
            if (fseek_end_value != -1) {

                //Get the current position in the file (in bytes)
                position = ftell(fIn);
                if (position != -1) {

                    //Go back to the beginning of the file
                    const int fseek_set_value = fseek(fIn, 0, SEEK_SET);
                    if (fseek_set_value != -1) {

                        //Allocate enough space to read the whole file
                        buffer = malloc(position);
                        if (buffer != NULL) {

                            //Read the whole file to buffer
                            const long size = fread(buffer, 1, position, fIn);

                            if (size == position) {
                                binary_data->size = position;
                                binary_data->data = buffer;

                                fclose(fIn);
                                return binary_data;
                            }
                            free(buffer);
                        }
                    }
                }
            }
            fclose(fIn);
        }
        free(binary_data);
    }
    return NULL;
}
