#include "core/save.h"

#include <cassert>

//
// uint8_t
//
void load(std::istream &ips, uint8_t &obj)
{
	char vc = ips.get();
	obj = *reinterpret_cast<uint8_t *>(&vc);
}

void save(std::ostream &ops, uint8_t &obj)
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
	obj = ((uint16_t)v0)|(((uint16_t)v1)<<8);
}

void save(std::ostream &ops, uint16_t &obj)
{
	uint8_t v0 = (uint8_t)(obj);
	uint8_t v1 = (uint8_t)(obj>>8);
	save(ops, v0);
	save(ops, v1);
}

void load(std::istream &ips, uint32_t &obj)
{
	uint16_t v0 = 0;
	uint16_t v1 = 0;
	load(ips, v0);
	load(ips, v1);
	obj = ((uint32_t)v0)|(((uint32_t)v1)<<16);
}

void save(std::ostream &ops, uint32_t &obj)
{
	uint16_t v0 = (uint16_t)(obj);
	uint16_t v1 = (uint16_t)(obj>>16);
	save(ops, v0);
	save(ops, v1);
}

void load(std::istream &ips, uint64_t &obj)
{
	uint32_t v0 = 0;
	uint32_t v1 = 0;
	load(ips, v0);
	load(ips, v1);
	obj = ((uint64_t)v0)|(((uint64_t)v1)<<32ULL);
}

void save(std::ostream &ops, uint64_t &obj)
{
	uint32_t v0 = (uint32_t)(obj);
	uint32_t v1 = (uint32_t)(obj>>32ULL);
	save(ops, v0);
	save(ops, v1);
}

//
// intX_t
//

void load(std::istream &ips, int8_t &obj)
{
	load(ips, (uint8_t&)obj);
}

void save(std::ostream &ops, int8_t &obj)
{
	save(ops, (uint8_t&)obj);
}

void load(std::istream &ips, int16_t &obj)
{
	load(ips, (uint16_t&)obj);
}

void save(std::ostream &ops, int16_t &obj)
{
	save(ops, (uint16_t&)obj);
}

void load(std::istream &ips, int32_t &obj)
{
	load(ips, (uint32_t&)obj);
}

void save(std::ostream &ops, int32_t &obj)
{
	save(ops, (uint32_t&)obj);
}

void load(std::istream &ips, int64_t &obj)
{
	load(ips, (uint64_t&)obj);
}

void save(std::ostream &ops, int64_t &obj)
{
	save(ops, (uint64_t&)obj);
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

void save(std::ostream &ops, bool &obj)
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
	delete buf;
}

void save(std::ostream &ops, std::string &obj)
{
	assert(obj.size() <= 0xFFFF);
	uint16_t len = obj.size();
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

void save(std::ostream &ops, Saveable &obj)
{
	obj.save_this(ops);
}
