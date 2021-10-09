#include <loop.h>
#include <duktape.h>
#include <duk_module.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/file.h>
#include <cwalk.h>

char *glb_path;

char *load_file(const char *address, size_t *size)
{
    size_t local_size;
    if (size == NULL)
    {
        size = &local_size;
    }

    *size = 0;
    int fd = open(address, O_RDONLY);
    if (fd < 0)
    {
        return NULL;
    }
    int to_read = 1;
    char *buffer = calloc(sizeof(char), to_read + 1);
    while (1)
    {
        ssize_t is_read = read(fd, buffer + *size, to_read);
        *size += is_read;
        if (is_read < to_read)
        {
            break;
        }
        else
        {
            to_read *= 2;
            buffer = realloc(buffer, *size + to_read + 1);
        }
    }
    buffer[*size] = '\0';
    close(fd);
    return buffer;
}

duk_ret_t gprint(duk_context *ctx)
{
    printf("%s\n", duk_to_string(ctx, 0));
    return 0;
}

duk_ret_t load_module(duk_context *ctx)
{
    char path[1024];
    const char *rel_path = duk_to_string(ctx, 0);
    cwk_path_join(glb_path, rel_path, path, 1024);
    char *src = load_file(path, NULL);
    duk_push_string(ctx, src);
    free(src);
    return 1;
}

void fatal_handler(void *_, const char *msg)
{
    fprintf(stderr, "*** FATAL ERROR: %s\n", (msg ? msg : "no message"));
    fflush(stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        return 1;
    }
    glb_path = argv[1];

    duk_context *ctx = duk_create_heap(NULL, NULL, NULL, NULL, fatal_handler);
    duk_module_duktape_init(ctx);
    char *script = load_file(glb_path, NULL);

    size_t len;
    cwk_path_get_dirname(glb_path, &len);
    glb_path[len] = '\0';

    duk_get_global_string(ctx, "Duktape");
    duk_push_c_function(ctx, load_module, 4);
    duk_put_prop_string(ctx, -2, "modSearch");
    duk_pop(ctx);

    duk_push_c_function(ctx, gprint, 1);
    duk_put_global_string(ctx, "print");
    duk_eval_string_noresult(ctx, script);
    duk_destroy_heap(ctx);
    return 0;
}