#include "FrameBuffer.h"

int main() {

	FrameBuffer *fb = new FrameBuffer;
//pesawat
	int deltay = 200 ;
	int deltax = 900;

	while (1)
	{
	// frame clipping
	    fb->vyoffset = 0;
	    fb->vxoffset = 900;
	    fb->vxsize = 100;
	    fb->vysize = 300;

	    fb->draw_view();
	    fb->wyoffset = 0;
	    fb->wxoffset = 900;
	    fb->wxsize = 100;
	    fb->wysize = 300;

		fb->draw_line_clip(deltax+0,3+deltay,deltax+16, 11+deltay,0,0,150);
		fb->draw_line_clip(deltax+16,11+deltay,deltax+81,11+deltay,0,0,150);
		fb->draw_line_clip(deltax+81,11+deltay,deltax+125,57+deltay,0,0,150);
		fb->draw_line_clip(deltax+125,57+deltay,deltax+134,57+deltay,0,0,150);
		fb->draw_line_clip(deltax+134,57+deltay,deltax+110,21+deltay,0,0,150);
		fb->draw_line_clip(deltax+110,21+deltay,deltax+112,11+deltay,0,0,150);
		fb->draw_line_clip(deltax+112,11+deltay,deltax+154,9+deltay,0,0,150);
		fb->draw_line_clip(deltax+154,9+deltay,deltax+167,28+deltay,0,0,150);
		fb->draw_line_clip(deltax+167,28+deltay,deltax+176,28+deltay,0,0,150);
		fb->draw_line_clip(deltax+176,28+deltay,deltax+169,11+deltay,0,0,150);
		fb->draw_line_clip(deltax+169,11+deltay,deltax+169,-11+deltay,0,0,150);
		fb->draw_line_clip(deltax+169,-11+deltay,deltax+176,-28+deltay,0,0,150);
		fb->draw_line_clip(deltax+176,-28+deltay,deltax+167,-28+deltay,0,0,150);
		fb->draw_line_clip(deltax+167,-28+deltay,deltax+154,-9+deltay,0,0,150);
		fb->draw_line_clip(deltax+154,-9+deltay,deltax+112,-11+deltay,0,0,150);
		fb->draw_line_clip(deltax+112,-11+deltay,deltax+110,-21+deltay,0,0,150);
		fb->draw_line_clip(deltax+110,-21+deltay,deltax+134,-57+deltay,0,0,150);
		fb->draw_line_clip(deltax+134,-57+deltay,deltax+125,-57+deltay,0,0,150);
		fb->draw_line_clip(deltax+125,-57+deltay,deltax+81,-11+deltay,0,0,150);
		fb->draw_line_clip(deltax+81,-11+deltay,deltax+16,-11+deltay,0,0,150);
		fb->draw_line_clip(deltax+16,-11+deltay,deltax+0,-3+deltay,0,0,150);
		fb->draw_line_clip(deltax+0,3+deltay,deltax+0,0+deltay,0,0,150);
		fb->draw_line_clip(deltax+0,-3+deltay,deltax+0,0+deltay,0,0,150);

		// fb->flood_fill_clip(deltax+90,deltay,0,0,150);

		usleep(100000);

		fb->draw_line_clip(deltax+0,3+deltay,deltax+16, 11+deltay,0,0,0);
		fb->draw_line_clip(deltax+16,11+deltay,deltax+81,11+deltay,0,0,0);
		fb->draw_line_clip(deltax+81,11+deltay,deltax+125,57+deltay,0,0,0);
		fb->draw_line_clip(deltax+125,57+deltay,deltax+134,57+deltay,0,0,0);
		fb->draw_line_clip(deltax+134,57+deltay,deltax+110,21+deltay,0,0,0);
		fb->draw_line_clip(deltax+110,21+deltay,deltax+112,11+deltay,0,0,0);
		fb->draw_line_clip(deltax+112,11+deltay,deltax+154,9+deltay,0,0,0);
		fb->draw_line_clip(deltax+154,9+deltay,deltax+167,28+deltay,0,0,0);
		fb->draw_line_clip(deltax+167,28+deltay,deltax+176,28+deltay,0,0,0);
		fb->draw_line_clip(deltax+176,28+deltay,deltax+169,11+deltay,0,0,0);
		fb->draw_line_clip(deltax+169,11+deltay,deltax+169,-11+deltay,0,0,0);
		fb->draw_line_clip(deltax+169,-11+deltay,deltax+176,-28+deltay,0,0,0);
		fb->draw_line_clip(deltax+176,-28+deltay,deltax+167,-28+deltay,0,0,0);
		fb->draw_line_clip(deltax+167,-28+deltay,deltax+154,-9+deltay,0,0,0);
		fb->draw_line_clip(deltax+154,-9+deltay,deltax+112,-11+deltay,0,0,0);
		fb->draw_line_clip(deltax+112,-11+deltay,deltax+110,-21+deltay,0,0,0);
		fb->draw_line_clip(deltax+110,-21+deltay,deltax+134,-57+deltay,0,0,0);
		fb->draw_line_clip(deltax+134,-57+deltay,deltax+125,-57+deltay,0,0,0);
		fb->draw_line_clip(deltax+125,-57+deltay,deltax+81,-11+deltay,0,0,0);
		fb->draw_line_clip(deltax+81,-11+deltay,deltax+16,-11+deltay,0,0,0);
		fb->draw_line_clip(deltax+16,-11+deltay,deltax+0,-3+deltay,0,0,0);
		fb->draw_line_clip(deltax+0,3+deltay,deltax+0,0+deltay,0,0,0);
		fb->draw_line_clip(deltax+0,-3+deltay,deltax+0,0+deltay,0,0,0);
		// fb->reset_fill_clip(deltax+0,-3+deltay,0,0,0);
		//if (isdestroyed) {
		//	break;
		//}

		deltax = deltax-13;
		if (deltax <= 200) {
			deltax = 900;
		}
		//xFront = deltax;
		//printf("%d\n", xFront);
	}

    delete fb;
	return 0;
}
