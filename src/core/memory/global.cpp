// Copyright (c) 2017 Fabio Polimeni
// Creation date: 20/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/memory/global.hpp"
#include "impl/global_impl.hpp"

namespace angie {
    namespace core {
        namespace memory {

            void* allocate(types::size size, types::size align) {
                return impl::allocate(size, align);
            }

            void deallocate(void* ptr) {
                impl::deallocate(ptr);
            }

            void* reallocate(void* ptr, types::size size, types::size align) {
                return impl::reallocate(ptr, size, align);
            }

            void flush() {
                impl::flush();
            }

        }
    }
}
