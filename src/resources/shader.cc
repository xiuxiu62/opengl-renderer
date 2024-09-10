#include "resources/shader.h"
#include "logger.h"

#include <cstdio>
#include <cstdlib>

FILE *try_open(const char *path, const char *modes);

ShaderSource shader_source_load(const char *path, ShaderKind kind) {
#define finish()                                                                                                       \
    fclose(file);                                                                                                      \
    return shader;

    ShaderSource shader = {.kind = kind, .path = path, .code = nullptr, .size = 0};
    FILE *file = try_open(path, "rb");
    if (!file) {
        info("Failed to open file: %s", path);
        finish();
    }

    fseek(file, 0, SEEK_END);
    shader.size = ftell(file);
    fseek(file, 0, SEEK_SET);

    shader.code = static_cast<char *>(malloc(shader.size + 1)); // +1 for null termination
    if (!shader.code) {
        info("Failed to allocate memory for shader code\n");
        finish();
    }

    usize bytes_read = fread(shader.code, 1, shader.size, file);
    if (bytes_read != shader.size) {
        info("Failed to read shader file\n");
    }
    shader.code[shader.size] = '\0'; // null terminate
    finish();
}

void shader_source_free(ShaderSource *src) {
    if (src->code) free(src->code);
    src->code = nullptr;
    src->size = 0;
}

FILE *try_open(const char *path, const char *modes) {
    FILE *file;
#ifdef _WIN32
    errno_t err = fopen_s(&file, path, modes);
    if (err) {
        if (file) fclose(file);
        file = nullptr;
    }
#else
    file = fopen(path, modes);
#endif /* ifdef WIN32 */
    return file;
}
