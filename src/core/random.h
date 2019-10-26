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

#ifndef CORE_RANDOM_H
#define CORE_RANDOM_H

#include <cstdint>

class Random
{
protected:
  uint32_t m_seed;
public:
  Random(void);
  Random(uint32_t seed);

  uint32_t get_seed(void) { return m_seed; }
  void set_seed(uint32_t seed) { m_seed = seed; }
  uint32_t next_int(uint32_t open_upper_bound);
protected:
  uint32_t next_raw(void);
};

#endif /* if !defined(CORE_RANDOM_H) */
