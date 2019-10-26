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

#include "core/random.h"

#include <chrono>
#include <cstdint>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;
using std::chrono::time_point;

Random::Random(uint32_t seed)
  : m_seed(seed)
{

}

Random::Random(void)
{
  // Grab it from the system time in milliseconds.
  time_point<system_clock> now = system_clock::now();
  m_seed = static_cast<uint32_t>((duration_cast<milliseconds>(now.time_since_epoch()).count()));

  // Seed it a few times for good luck.
  for (int i = 0; i < 13; i++) {
    this->next_raw();
  }
}

uint32_t Random::next_raw(void)
{
  // Classic glibc rand() coefficients
  constexpr uint32_t SEED_MUL = 1103515245;
  constexpr uint32_t SEED_ADD = 12345;

  m_seed = (m_seed * SEED_MUL) + SEED_ADD;

  return m_seed;
}

uint32_t Random::next_int(uint32_t open_upper_bound)
{
  uint64_t result = this->next_raw();
  result *= open_upper_bound;
  result >>= 32;
  return (uint32_t)result;
}
