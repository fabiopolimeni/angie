// Copyright (c) 2017 Fabio Polimeni
// Creation date: 20/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/system.hpp"
#include "impl/system_impl.hpp"

namespace angie {
    namespace core {
        namespace system {

            error_e init(report::callback *cb) {
                return impl::init(cb);
            }

            void shutdown() {
                impl::shutdown();
            }

        }
    }
}
