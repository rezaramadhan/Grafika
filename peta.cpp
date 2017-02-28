#include "FrameBuffer.h"

int main() {
	FrameBuffer *fb = new FrameBuffer;
/*
	fb->vyoffset = 70;
    fb->vxoffset = 900;
    fb->vxsize = 300;
    fb->vysize = 300;

    fb->draw_view();
    fb->wyoffset = 70;
    fb->wxoffset = 900;
    fb->wxsize = 300;
    fb->wysize = 300;


    fb->draw_line_clip(250,310,400,350, 0,0,255);
	//fb->draw_line_clip(1000,60,1000,120,0,0,150);
	
//pesawat
	int deltay = 60 ;
	int deltax = 1100;
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

		//fb->flood_fill(deltax+90,deltay,0,0,150);

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
		//fb->reset_fill(deltax+0,-3+deltay,0,0,0);
		//if (isdestroyed) {
		//	break;
		//}

		deltax = deltax-13;
		if (deltax <= 200) {
			deltax = 1100;
		}
		//xFront = deltax;
		//printf("%d\n", xFront);
	}*/
    fb->draw_bangunan();
	fb->draw_street();
    delete fb;
	return 0;
}
