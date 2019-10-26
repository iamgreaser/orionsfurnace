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

#ifndef CORE_SAVE_H
#define CORE_SAVE_H

#include <cstdint>
#include <iostream>

class Saveable
{
public:
  virtual ~Saveable(void) {}
  virtual void load_this(std::istream &ips) = 0;
  virtual void save_this(std::ostream &ops) const = 0;
};

void load(std::istream &ips, uint8_t &obj);
void save(std::ostream &ops, uint8_t obj);

void load(std::istream &ips, uint16_t &obj);
void save(std::ostream &ops, uint16_t obj);
void load(std::istream &ips, uint32_t &obj);
void save(std::ostream &ops, uint32_t obj);
void load(std::istream &ips, uint64_t &obj);
void save(std::ostream &ops, uint64_t obj);

void load(std::istream &ips, int8_t &obj);
void save(std::ostream &ops, int8_t obj);
void load(std::istream &ips, int16_t &obj);
void save(std::ostream &ops, int16_t obj);
void load(std::istream &ips, int32_t &obj);
void save(std::ostream &ops, int32_t obj);
void load(std::istream &ips, int64_t &obj);
void save(std::ostream &ops, int64_t obj);

void load(std::istream &ips, bool &obj);
void save(std::ostream &ops, bool obj);

void load(std::istream &ips, std::string &obj);
void save(std::ostream &ops, const std::string &obj);

void load(std::istream &ips, Saveable &obj);
void save(std::ostream &ops, const Saveable &obj);

#endif /* if !defined(CORE_SAVE_H) */
