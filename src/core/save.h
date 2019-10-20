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
void load(std::istream &ips, Saveable &obj);
void save(std::ostream &ops, Saveable &obj);

#endif /* if !defined(_CORE_SAVE_H) */
