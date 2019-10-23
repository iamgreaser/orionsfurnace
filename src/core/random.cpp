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
	m_seed = (uint32_t)(duration_cast<milliseconds>(now.time_since_epoch()).count());

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

