// https://bytefreaks.net/programming-2/c/cc-full-example-of-reading-a-whole-binary-file-to-buffer

#ifndef GM_S_LITTLE_HELPERS_FILE_HELPERS_H
#define GM_S_LITTLE_HELPERS_FILE_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        long size;
        void *data;
    } binary_data_t;

    int32_t fh_set_path(const char *path);
    binary_data_t *fh_read_file(const char *filename);

#ifdef __cplusplus
}
#endif

#endif //GM_S_LITTLE_HELPERS_FILE_HELPERS_H
