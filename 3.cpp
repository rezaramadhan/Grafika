#include "FrameBuffer.h"
using namespace std;

FrameBuffer *fb;

int main() {
	fb = new FrameBuffer;
	int deltay = 60 ;

	//GAMBAR PESAWAT
	fb->draw_line(0,3+deltay,16, 11+deltay,0,0,150);
	fb->draw_line(16,11+deltay,81,11+deltay,0,0,150);
	fb->draw_line(81,11+deltay,125,57+deltay,0,0,150);
	fb->draw_line(125,57+deltay,134,57+deltay,0,0,150);
	fb->draw_line(134,57+deltay,110,21+deltay,0,0,150);
	fb->draw_line(110,21+deltay,112,11+deltay,0,0,150);
	fb->draw_line(112,11+deltay,154,9+deltay,0,0,150);
	fb->draw_line(154,9+deltay,167,28+deltay,0,0,150);
	fb->draw_line(167,28+deltay,176,28+deltay,0,0,150);
	fb->draw_line(176,28+deltay,169,11+deltay,0,0,150);
	fb->draw_line(169,11+deltay,169,-11+deltay,0,0,150);
	fb->draw_line(169,-11+deltay,176,-28+deltay,0,0,150);
	fb->draw_line(176,-28+deltay,167,-28+deltay,0,0,150);
	fb->draw_line(167,-28+deltay,154,-9+deltay,0,0,150);
	fb->draw_line(154,-9+deltay,112,-11+deltay,0,0,150);
	fb->draw_line(112,-11+deltay,110,-21+deltay,0,0,150);
	fb->draw_line(110,-21+deltay,134,-57+deltay,0,0,150);
	fb->draw_line(134,-57+deltay,125,-57+deltay,0,0,150);
	fb->draw_line(125,-57+deltay,81,-11+deltay,0,0,150);
	fb->draw_line(81,-11+deltay,16,-11+deltay,0,0,150);
	fb->draw_line(16,-11+deltay,0,-3+deltay,0,0,150);

	fb->flood_fill(90,deltay,0,0,150);
	// printf("%i\n", fb->isBlack(150,150));
	// printf("%i\n", fb->isBlack(100,100));
	// printf("%i\n", fb->isBlack(400,500));

	delete fb;
	return 0;
}
