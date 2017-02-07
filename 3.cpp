#include "FrameBuffer.h"
using namespace std;

FrameBuffer *fb;

void planePart1(float x, float y, int r, int g, int b) { //moncong pesawat; x,y sebagai pusat
	fb->draw_line(x-8,y-3,x+8,y-11,0,0,150);
	fb->draw_line(x-8,y-3,x-8,y+3,0,0,150);
	fb->draw_line(x-8,y+3,x+8,y+11,0,0,150);
	fb->draw_line(x+8,y-11,x+8,y+11,0,0,150);
	fb->flood_fill(x,y,r,g,b);
}

void planePart2(float x, float y, int r, int g, int b) { //badan depan pesawat; x,y sebagai pusat
	fb->draw_line(x-32,y-11,x+33,y-11,0,0,150);
	fb->draw_line(x-32,y+11,x+33,y+11,0,0,150);
	fb->draw_line(x-32,y-11,x-32,y+11,0,0,150);
	fb->draw_line(x+33,y-11,x+33,y+11,0,0,150);
	fb->flood_fill(x,y,r,g,b);
}

void planePart3(float x, float y, int r, int g, int b) { //sayap atas; x,y sebagai titik kiri bawah sayap
	fb->draw_line(x,y,x+44,y-46,0,0,150);
	fb->draw_line(x+44,y-46,x+53,y-46,0,0,150);
	fb->draw_line(x+53,y-46,x+29,y-10,0,0,150);
	fb->draw_line(x+29,y-10,x+31,y,0,0,150);
	fb->draw_line(x+31,y,x,y,0,0,150);
	fb->flood_fill(x+15,y-5,r,g,b);
}

void planePart4(float x, float y, int r, int g, int b) { //sayap bawah; x,y sebagai titik kiri atas sayap
	fb->draw_line(x,y,x+44,y+46,0,0,150);
	fb->draw_line(x+44,y+46,x+53,y+46,0,0,150);
	fb->draw_line(x+53,y+46,x+29,y+10,0,0,150);
	fb->draw_line(x+29,y+10,x+31,y,0,0,150);
	fb->draw_line(x+31,y,x,y,0,0,150);
	fb->flood_fill(x+15,y+5,r,g,b);
}

void planePart5(float x, float y, int r, int g, int b) { //badan tengah pesawat(diapit sayap); x,y sebagai pusat
	fb->draw_line(x-15,y-11,x+16,y-11,0,0,150);
	fb->draw_line(x-15,y+11,x+16,y+11,0,0,150);
	fb->draw_line(x-15,y-11,x-15,y+11,0,0,150);
	fb->draw_line(x+16,y-11,x+16,y+11,0,0,150);
	fb->flood_fill(x,y,r,g,b);
}

void planePart6(float x, float y, int r, int g, int b) { //badan belakang pesawat (diapit ekor); x,y sebagai pertengahan antara O dan H
	fb->draw_line(x,y-9,x+15,y-11,0,0,150);
	fb->draw_line(x+15,y-11,x+15,y+11,0,0,150);
	fb->draw_line(x+15,y+11,x,y+9,0,0,150);
	fb->draw_line(x,y+9,x-42,y+11,0,0,150);
	fb->draw_line(x-42,y+11,x-42,y-11,0,0,150);
	fb->draw_line(x-42,y-11,x,y-9,0,0,150);
	fb->flood_fill(x,y,r,g,b);
}

void planePart7(float x, float y, int r, int g, int b) { //ekor atas ; x,y sebagai titik kiri bawah ekor
	fb->draw_line(x,y,x+13,y-19,0,0,150);
	fb->draw_line(x+13,y-19,x+24,y-19,0,0,150);
	fb->draw_line(x+24,y-19,x+17,y-2,0,0,150);
	fb->draw_line(x+17,y-2,x,y,0,0,150);
	fb->flood_fill(x+7,y-5,r,g,b);
}

void planePart8(float x, float y, int r, int g, int b) { //ekor bawah ; x,y sebagai titik kiri atas ekor
	fb->draw_line(x,y,x+13,y+19,0,0,150);
	fb->draw_line(x+13,y+19,x+24,y+19,0,0,150);
	fb->draw_line(x+24,y+19,x+17,y+2,0,0,150);
	fb->draw_line(x+17,y+2,x,y,0,0,150);
	fb->flood_fill(x+7,y+5,r,g,b);
}

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
