#include "FrameBuffer.h"
using namespace std;

FrameBuffer *fb;

int main() {
	fb = new FrameBuffer;
	int deltay = 60 ;

	//GAMBAR PESAWAT
	
	int deltax = 1100;
	int n = 0;

	while (n < 3) 
	{
		fb->draw_line(deltax+0,3+deltay,deltax+16, 11+deltay,0,0,150);
		fb->draw_line(deltax+16,11+deltay,deltax+81,11+deltay,0,0,150);
		fb->draw_line(deltax+81,11+deltay,deltax+125,57+deltay,0,0,150);
		fb->draw_line(deltax+125,57+deltay,deltax+134,57+deltay,0,0,150);
		fb->draw_line(deltax+134,57+deltay,deltax+110,21+deltay,0,0,150);
		fb->draw_line(deltax+110,21+deltay,deltax+112,11+deltay,0,0,150);
		fb->draw_line(deltax+112,11+deltay,deltax+154,9+deltay,0,0,150);
		fb->draw_line(deltax+154,9+deltay,deltax+167,28+deltay,0,0,150);
		fb->draw_line(deltax+167,28+deltay,deltax+176,28+deltay,0,0,150);
		fb->draw_line(deltax+176,28+deltay,deltax+169,11+deltay,0,0,150);
		fb->draw_line(deltax+169,11+deltay,deltax+169,-11+deltay,0,0,150);
		fb->draw_line(deltax+169,-11+deltay,deltax+176,-28+deltay,0,0,150);
		fb->draw_line(deltax+176,-28+deltay,deltax+167,-28+deltay,0,0,150);
		fb->draw_line(deltax+167,-28+deltay,deltax+154,-9+deltay,0,0,150);
		fb->draw_line(deltax+154,-9+deltay,deltax+112,-11+deltay,0,0,150);
		fb->draw_line(deltax+112,-11+deltay,deltax+110,-21+deltay,0,0,150);
		fb->draw_line(deltax+110,-21+deltay,deltax+134,-57+deltay,0,0,150);
		fb->draw_line(deltax+134,-57+deltay,deltax+125,-57+deltay,0,0,150);
		fb->draw_line(deltax+125,-57+deltay,deltax+81,-11+deltay,0,0,150);
		fb->draw_line(deltax+81,-11+deltay,deltax+16,-11+deltay,0,0,150);
		fb->draw_line(deltax+16,-11+deltay,deltax+0,-3+deltay,0,0,150);
		fb->draw_line(deltax+0,3+deltay,deltax+0,0+deltay,0,0,150);
		fb->draw_line(deltax+0,-3+deltay,deltax+0,0+deltay,0,0,150);
		fb->flood_fill(deltax+90,deltay,0,0,150);
		
		usleep(10000);

		fb->reset_fill(deltax+90,deltay,0,0,0);
		
		deltax = deltax-3;
		if (deltax <= 0) {
			deltax = 1100;
			n++;
		}
	}


	// printf("%i\n", fb->isBlack(150,150));
	// printf("%i\n", fb->isBlack(100,100));
	// printf("%i\n", fb->isBlack(400,500));

	delete fb;
	return 0;
}
