#include "FrameBuffer.h"
using namespace std;

FrameBuffer *fb;

int main() {
	fb = new FrameBuffer;

	fb->draw_line(100,100,150,150,0,0,150);
	fb->draw_line(150,150,100,200,0,0,150);
	fb->draw_line(100,100,100,200,0,0,150);

	fb->flood_fill(125,150,0,0,150);
	// printf("%i\n", fb->isBlack(150,150));
	// printf("%i\n", fb->isBlack(100,100));
	// printf("%i\n", fb->isBlack(400,500));

	delete fb;
	return 0;
}
