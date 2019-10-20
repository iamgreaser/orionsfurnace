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
