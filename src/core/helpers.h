/*
This file is part of Orion's Furnace.

Orion's Furnace is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Orion's Furnace is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with Orion's Furnace.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CORE_HELPERS_H
#define CORE_HELPERS_H

#include <cstdlib>
#include <cstdio>

// TODO: Work out how to shut up one specific GCC warning
#if 0
#define PANIC(fmt, ...) { fprintf(stderr, "%s:%d:%s() | Fatal error! " fmt "\n", __FILE__, __LINE__, __func__, ## __VA_ARGS__); abort(); }
#else
#define PANIC(fmt) { fprintf(stderr, "%s:%d:%s() | Fatal error! %s\n", __FILE__, __LINE__, __func__, fmt); abort(); }
#endif
#endif /* if !defined(CORE_HELPERS_H) */
