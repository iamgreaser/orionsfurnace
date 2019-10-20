#ifndef _CORE_SAVE_H
#define _CORE_SAVE_H

#include <cstdint>
#include <iostream>

class Saveable
{
public:
	virtual void load_this(std::istream &ips) = 0;
	virtual void save_this(std::ostream &ops) = 0;
};

void load(std::istream &ips, uint8_t &obj);
void save(std::ostream &ops, uint8_t &obj);

void load(std::istream &ips, uint16_t &obj);
void save(std::ostream &ops, uint16_t &obj);
void load(std::istream &ips, uint32_t &obj);
void save(std::ostream &ops, uint32_t &obj);
void load(std::istream &ips, uint64_t &obj);
void save(std::ostream &ops, uint64_t &obj);

void load(std::istream &ips, int8_t &obj);
void save(std::ostream &ops, int8_t &obj);
void load(std::istream &ips, int16_t &obj);
void save(std::ostream &ops, int16_t &obj);
void load(std::istream &ips, int32_t &obj);
void save(std::ostream &ops, int32_t &obj);
void load(std::istream &ips, int64_t &obj);
void save(std::ostream &ops, int64_t &obj);

void load(std::istream &ips, bool &obj);
void save(std::ostream &ops, bool &obj);

void load(std::istream &ips, Saveable &obj);
void save(std::ostream &ops, Saveable &obj);

#endif /* if !defined(_CORE_SAVE_H) */
