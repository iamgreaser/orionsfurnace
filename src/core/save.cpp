#include "core/save.h"

void load(std::istream &ips, uint8_t &obj)
{
	char vc = ips.get();
	obj = *reinterpret_cast<uint8_t *>(&vc);
}

void save(std::ostream &ops, uint8_t &obj)
{
	ops.put(*reinterpret_cast<char *>(&obj));
}

void load(std::istream &ips, Saveable &obj)
{
	obj.load_this(ips);
}

void save(std::ostream &ops, Saveable &obj)
{
	obj.save_this(ops);
}
