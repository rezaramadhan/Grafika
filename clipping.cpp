#include "FrameBuffer.h"

int main() {
	FrameBuffer *fb = new FrameBuffer;
	fb->vyoffset = 300;
    fb->vxoffset = 300;
    fb->vxsize = 160;
    fb->vysize = 90;
    
    fb->draw_line_clip(200,200,550,200,255,255,255);
	fb->draw_line_clip(550,200,550,550,255,255,255);
	fb->draw_line_clip(550,550,200,550,255,255,255);
	fb->draw_line_clip(200,550,200,200,255,255,255);
	
	fb->flood_fill_clip(215,215,255,255,255);
	
    fb->draw_view();
    fb->wyoffset = 300;
    fb->wxoffset = 300;
    fb->wxsize = 160;
    fb->wysize = 90;
	
    delete fb;
	return 0;
}
