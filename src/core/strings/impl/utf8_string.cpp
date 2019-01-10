// Copyright (c) 2018 Fabio Polimeni
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
#include "angie/core/strings/utf8_string.hpp"

#include <wchar.h>

namespace {
    // the number of characters in a multibyte string is the sum of mblen()'s
    // note: the simpler approach is mbstowcs(NULL, str, sz)
    size_t strlen_mb(const char* ptr, size_t count) {
        mbstate_t state = {0};
        size_t result = 0;
        const char* end = ptr + count;
        mbrlen(NULL, 0, &state); // reset the conversion state
        
        while(ptr < end) {
            int64_t next = mbrlen(ptr, end - ptr, &state);
            if(next == -1) {
                return 0;
            }

            ptr += next;
            ++result;
        }

        return result;
    }
}

namespace angie {
	namespace core {
		namespace strings {
            namespace utf8 {

                /**
                 * Get the length of a utf8 string.
                 *
                 * Because a UTF8 codepoint can require one
                 * or more bytes, we can't compute the length
                 * directly from the byte count, we will have
                 * to walk through the whole list of codepoints.
                 */
                types::size length(const utf8_string& str) {
                    //mbstate_t state = {0};
                    //return mbsrtowcs(nullptr, &str.data, 0, &state);
                    return strlen_mb(str, buffers::get_count(str));
                }

            }
        }
    }
}