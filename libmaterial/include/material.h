#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

// A raw buffer of bytes from rust
struct Buffer {
  uint8_t *data;
  uintptr_t len;
};

extern "C" {

/// Update a material file
/// # Safety
/// - Input pointer and length are valid
/// - You free the output later
int update_file(uintptr_t input_len, const uint8_t *input_data, Buffer *out_buffer);

/// Free the output
void free_buf(Buffer buf);

} // extern "C"
