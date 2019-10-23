#ifndef _CORE_RANDOM_H
#define _CORE_RANDOM_H

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

#endif /* if !defined(_CORE_RANDOM_H) */
