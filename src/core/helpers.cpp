#include "core/helpers.h"

#include <cstdint>
#include <iostream>

void get_u8(std::istream &ips, uint8_t &v)
{
	char vc = ips.get();
	v = *reinterpret_cast<uint8_t *>(&vc);
}

void put_u8(std::ostream &ops, uint8_t v)
{
	ops.put(*reinterpret_cast<char *>(&v));
}
