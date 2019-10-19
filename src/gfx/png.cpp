#include "gfx/png.h"

#include "stb_image.h"

#include <allegro.h>

#include <cassert>
#include <iostream>

void gfx::install_png_loader(void)
{
	register_bitmap_file_type("PNG", gfx::load_png, NULL);
}

BITMAP *gfx::load_png(const char *filename, RGB *pal)
{
	// Allegro 4 doesn't support PNG out of the box.
	// Therefore we need to provide it ourselves.
	// Fortunately stb_image exists...

	// No palette.
	(void)pal;

	// Load the image.
	int width = 0;
	int height = 0;
	int channels_in_file = 0;
	stbi_uc *img = stbi_load(filename,
		&width, &height, &channels_in_file,
		STBI_rgb_alpha);
	assert(img != NULL);
	if (img == NULL) {
		return NULL;
	}

	// Create a bitmap.
	BITMAP *bmp = create_bitmap_ex(32, width, height);
	assert(bmp != NULL);

	// Now fill in the strides.
	for (int y = 0; y < height; y++) {
		memcpy(bmp->line[y], img + y*width*4, width*4);

		// Strip the alpha component for pink
		// so Allegro 4 can detect it as transparent.
		for (int x = 0; x < width; x++) {
			if (bmp->line[y][x*4+0] == 0xFF
			&& bmp->line[y][x*4+1] == 0x00
			&& bmp->line[y][x*4+2] == 0xFF) {
				bmp->line[y][x*4+3] = 0;
			}
		}
	}

	// Free the stbi image.
	stbi_image_free(img);

	// We're done! Return bmp.
	return bmp;
}
