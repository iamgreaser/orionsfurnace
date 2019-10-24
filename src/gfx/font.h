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

#ifndef _GFX_FONT_H
#define _GFX_FONT_H

#include "gfx/gfx.h"

#include <cstddef>
#include <string>

namespace gfx
{
	using std::string;

	class Font
	{
	private:
		const string m_filename;
		const int m_ptsize;
		void *m_loaded_font = NULL;
	public:
		Font(const string filename, const int ptsize);
		~Font();

		void ensure_loaded(void);

		void draw(int px, int py,
			int r, int g, int b,
			const std::string text);
	};

	extern Font font_base;
}

using gfx::Font;

#endif /* if !defined(_GFX_FONT_H) */
