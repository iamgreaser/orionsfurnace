#ifndef _CORE_PLAYER_H
#define _CORE_PLAYER_H

#include "core/core.h"

#include "core/save.h"

#include <cassert>
#include <cstdint>
#include <iostream>

class PlayerInput : public Saveable
{
private:
	bool m_input_move[4] = {};
public:
	PlayerInput(void);
	PlayerInput(std::istream &ips);

	bool get_input_move(Direction dir) {
		assert(dir >= 0 && dir < 4);
		return m_input_move[dir];
	}
	void set_input_move(Direction dir, bool v) {
		assert(dir >= 0 && dir < 4);
		m_input_move[dir] = v;
	}

	void load_this(std::istream &ips);
	void save_this(std::ostream &ops);
};

class Player : public Saveable
{
	friend Game; // m_game MUST point to the correct game!
private:
	Game *m_game;
	int32_t m_cx;
	int32_t m_cy;
	Direction m_dir;

	int32_t m_pos_interp_x0;
	int32_t m_pos_interp_y0;
	int32_t m_pos_interp_x1;
	int32_t m_pos_interp_y1;
	int32_t m_pos_interp_remain = 0;
	int32_t m_pos_interp_len = 0;

	PlayerInput m_input;
	int32_t m_input_cooldown = 0;

public:
	Player(Game *game, int cx, int cy, Direction dir);
	Player(Game *game, std::istream &ips);

	int get_x(void) { return m_cx; }
	int get_y(void) { return m_cy; }
	Direction get_dir(void) { return m_dir; }
	bool get_input_move(Direction dir) {
		return m_input.get_input_move(dir);
	}
	void set_input_move(Direction dir, bool v) {
		m_input.set_input_move(dir, v);
	}
	void set_all_inputs(PlayerInput player_input);

	void tick(void);

	void draw(void);

	void load_this(std::istream &ips);
	void save_this(std::ostream &ops);

	bool attempt_move_by(int dx, int dy);
	bool attempt_move_to(int cx, int cy);

protected:
	void calc_interp_pos(int *px, int *py);
};

#endif /* if !defined(_CORE_PLAYER_H) */
