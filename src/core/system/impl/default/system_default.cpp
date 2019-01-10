// Copyright (c) 2017 Fabio Polimeni
// Created on: 18/03/2017
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "angie/core/system/system.hpp"
#include <locale.h>

namespace angie {
    namespace core {
        namespace system {

            error init() {
                if (!setlocale(LC_ALL, "en_GB.utf8")) {
                    return error::locale_error;
                }
                
                if (!setlocale(LC_NUMERIC, "en_GB.utf8")) {
                    return error::locale_error;
                }

                if (!setlocale(LC_TIME, "en_GB.utf8")) {
                    return error::locale_error;
                }
                
                return error::ok;
            }

            void shutdown() {
            }

        }
    }
}