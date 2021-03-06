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

#include "core/save.h"

#include <cassert>

//
// uint8_t
//
void load(std::istream &ips, uint8_t &obj)
{
  int v = ips.get();
  assert(v >= 0); // TODO: throw an exception on EOF instead
  assert(v <= 0xFF); // this on the other hand should just crash on failure
  uint8_t vc = static_cast<uint8_t>(v);
  obj = *reinterpret_cast<uint8_t *>(&vc);
}

void save(std::ostream &ops, uint8_t obj)
{
  ops.put(*reinterpret_cast<char *>(&obj));
}

//
// uintX_t
//
void load(std::istream &ips, uint16_t &obj)
{
  uint8_t v0 = 0;
  uint8_t v1 = 0;
  load(ips, v0);
  load(ips, v1);
  obj = static_cast<uint16_t>((static_cast<uint32_t>(v0))|(static_cast<uint32_t>(v1)<<8));
}

void save(std::ostream &ops, uint16_t obj)
{
  uint8_t v0 = static_cast<uint8_t>(obj);
  uint8_t v1 = static_cast<uint8_t>(obj>>8);
  save(ops, v0);
  save(ops, v1);
}

void load(std::istream &ips, uint32_t &obj)
{
  uint16_t v0 = 0;
  uint16_t v1 = 0;
  load(ips, v0);
  load(ips, v1);
  obj = static_cast<uint32_t>(v0)|(static_cast<uint32_t>(v1)<<16);
}

void save(std::ostream &ops, uint32_t obj)
{
  uint16_t v0 = static_cast<uint16_t>(obj);
  uint16_t v1 = static_cast<uint16_t>(obj>>16);
  save(ops, v0);
  save(ops, v1);
}

void load(std::istream &ips, uint64_t &obj)
{
  uint32_t v0 = 0;
  uint32_t v1 = 0;
  load(ips, v0);
  load(ips, v1);
  obj = static_cast<uint64_t>(v0)|(static_cast<uint64_t>(v1)<<32ULL);
}

void save(std::ostream &ops, uint64_t obj)
{
  uint32_t v0 = static_cast<uint32_t>(obj);
  uint32_t v1 = static_cast<uint32_t>(obj>>32ULL);
  save(ops, v0);
  save(ops, v1);
}

//
// intX_t
//

void load(std::istream &ips, int8_t &obj)
{
  load(ips, reinterpret_cast<uint8_t &>(obj));
}

void save(std::ostream &ops, int8_t obj)
{
  save(ops, static_cast<uint8_t>(obj));
}

void load(std::istream &ips, int16_t &obj)
{
  load(ips, reinterpret_cast<uint16_t &>(obj));
}

void save(std::ostream &ops, int16_t obj)
{
  save(ops, static_cast<uint16_t>(obj));
}

void load(std::istream &ips, int32_t &obj)
{
  load(ips, reinterpret_cast<uint32_t &>(obj));
}

void save(std::ostream &ops, int32_t obj)
{
  save(ops, static_cast<uint32_t>(obj));
}

void load(std::istream &ips, int64_t &obj)
{
  load(ips, reinterpret_cast<uint64_t &>(obj));
}

void save(std::ostream &ops, int64_t obj)
{
  save(ops, static_cast<uint64_t>(obj));
}

//
// bool
//
void load(std::istream &ips, bool &obj)
{
  uint8_t vc = 0;
  load(ips, vc);
  assert(vc == 0 || vc == 1);
  obj = (vc != 0);
}

void save(std::ostream &ops, bool obj)
{
  uint8_t vc = (obj ? 1 : 0);
  save(ops, vc);
}

//
// pstring16
//
void load(std::istream &ips, std::string &obj)
{
  uint16_t len = 0;
  load(ips, len);
  char *buf = new char[len]; // I'd use a VLA, but Microsoft refuse to implement VLAs which is why they're optional in C11
  ips.read(buf, len);
  obj = std::string(buf, len);
  delete[] buf;
}

void save(std::ostream &ops, const std::string &obj)
{
  size_t raw_len = obj.size();
  assert(raw_len <= 0xFFFF);
  uint16_t len = static_cast<uint16_t>(raw_len);
  save(ops, len);
  ops.write(obj.c_str(), len);
}

//
// Saveable
//
void load(std::istream &ips, Saveable &obj)
{
  obj.load_this(ips);
}

void save(std::ostream &ops, const Saveable &obj)
{
  obj.save_this(ops);
}
