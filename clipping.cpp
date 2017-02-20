#include "FrameBuffer.h"

int main() {
	FrameBuffer *fb = new FrameBuffer;
	fb->vyoffset = 300;
    fb->vxoffset = 300;
    fb->vxsize = 160;
    fb->vysize = 90;

    fb->draw_view();
    fb->wyoffset = 300;
    fb->wxoffset = 300;
    fb->wxsize = 160;
    fb->wysize = 90;

    fb->draw_line_clip(400,250,250,350, 0,0,255);

    delete fb;
	return 0;
}
