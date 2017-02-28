#include "FrameBuffer.h"

int main() {
	FrameBuffer *fb = new FrameBuffer;
	fb->wxsize = IMG_X_SIZE;
	fb->wysize = IMG_Y_SIZE;
	fb->wxoffset = 50;
	fb->wyoffset = 50;
	fb->vxsize = 532;
	fb->vysize = 624;
	fb->vxoffset = 160;
	fb->vyoffset = 90;
	fb->draw_view();
    fb->draw_bangunan_clip(0,0,255);
	fb->draw_street_clip(128,128,128);
	fb->draw_minimap(700,100,242,284);
    // fb->draw_bangunan();
	// fb->draw_street();
    delete fb;
	return 0;
}
