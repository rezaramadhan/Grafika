#include "FrameBuffer.h"
using namespace std;


int deltay =200 ;

FrameBuffer *fb;
int a, b, c, d, e,f,g,h,i,j,k,m,l;
int isdestroyed = 0;
int xbullet = 0;

int y1bullet = 550;
int y2bullet = 500;

int xFront = 900;

void drawExplosion(int xc, int yc){
	//Inner explosion
	fb->draw_line(xc+15,yc+20,xc+50,yc+40,255,255,0); //1
	fb->draw_line(xc+50,yc+40,xc+20,yc+10,255,255,0); //2
	fb->draw_line(xc+20,yc+10,xc+40,yc,255,255,0); //3
	fb->draw_line(xc+40,yc,xc+20,yc-10,255,255,0); //4
	fb->draw_line(xc+20,yc-10,xc+25,yc-40,255,255,0); //5
	fb->draw_line(xc+25,yc-40,xc,yc-15,255,255,0); //6
	fb->draw_line(xc,yc-15,xc-10,yc-30,255,255,0); //7
	fb->draw_line(xc-10,yc-30,xc-20,yc-5,255,255,0); //8
	fb->draw_line(xc-20,yc-5,xc-45,yc-13,255,255,0); //9
	fb->draw_line(xc-45,yc-13,xc-30,yc+7,255,255,0); //10
	fb->draw_line(xc-30,yc+7,xc-55,yc+15,255,255,0); //11
	fb->draw_line(xc-55,yc+15,xc-14,yc+23,255,255,0); //12
	fb->draw_line(xc-14,yc+23,xc-2,yc+38,255,255,0); //13
	fb->draw_line(xc-2,yc+38,xc+15,yc+20,255,255,0); //14

	//jeda untuk ledakan
	usleep(300000);

	//Middle explosion
	fb->draw_line(xc+20,yc+40,xc+80,yc+65,255,165,0); //1
	fb->draw_line(xc+80,yc+65,xc+45,yc+12,255,165,0); //2
	fb->draw_line(xc+45,yc+12,xc+63,yc-5,255,165,0); //3
	fb->draw_line(xc+63,yc-5,xc+35,yc-17,255,165,0); //4
	fb->draw_line(xc+35,yc-17,xc+40,yc-60,255,165,0); //5
	fb->draw_line(xc+40,yc-60,xc,yc-32,255,165,0); //6
	fb->draw_line(xc,yc-32,xc-13,yc-43,255,165,0); //7
	fb->draw_line(xc-13,yc-43,xc-25,yc-17,255,165,0); //8
	fb->draw_line(xc-25,yc-17,xc-65,yc-26,255,165,0); //9
	fb->draw_line(xc-65,yc-26,xc-52,yc+4,255,165,0); //10
	fb->draw_line(xc-52,yc+4,xc-70,yc+15,255,165,0); //11
	fb->draw_line(xc-70,yc+15,xc-18,yc+39,255,165,0); //12
	fb->draw_line(xc-18,yc+39,xc-8,yc+57,255,165,0); //13
	fb->draw_line(xc-8,yc+57,xc+20,yc+40,255,165,0); //14

	//jeda untuk ledakan
	usleep(300000);

	//Outer explosion
	fb->draw_line(xc+25,yc+60,xc+110,yc+95,255,0,0); //1
	fb->draw_line(xc+110,yc+95,xc+60,yc+10,255,0,0); //2
	fb->draw_line(xc+60,yc+10,xc+85,yc-10,255,0,0); //3
	fb->draw_line(xc+85,yc-10,xc+45,yc-22,255,0,0); //4
	fb->draw_line(xc+45,yc-22,xc+52,yc-72,255,0,0); //5
	fb->draw_line(xc+52,yc-72,xc,yc-48,255,0,0); //6
	fb->draw_line(xc,yc-48,xc-18,yc-62,255,0,0); //7
	fb->draw_line(xc-18,yc-62,xc-34,yc-24,255,0,0); //8
	fb->draw_line(xc-34,yc-24,xc-80,yc-38,255,0,0); //9
	fb->draw_line(xc-80,yc-38,xc-65,yc+4,255,0,0); //10
	fb->draw_line(xc-65,yc+4,xc-87,yc+18,255,0,0); //11
	fb->draw_line(xc-87,yc+18,xc-20,yc+45,255,0,0); //12
	fb->draw_line(xc-20,yc+45,xc-10,yc+70,255,0,0); //13
	fb->draw_line(xc-10,yc+70,xc+25,yc+60,255,0,0); //14
}

void selectPart(float x, float y, int r, int g, int b, int num, bool reset) {
	int bbor;
	// Menghapus instance pecahan sebelumnya
	if (reset) {
		bbor = 0;
	}
	else {
		bbor = 150;
	}

	switch(num) {
		case 1: { //moncong pesawat; x,y sebagai pusat
			fb->draw_line(x-8,y-3,x+8,y-11,0,0,bbor);
			fb->draw_line(x-8,y-3,x-8,y+3,0,0,bbor);
			fb->draw_line(x-8,y+3,x+8,y+11,0,0,bbor);
			fb->draw_line(x+8,y-11,x+8,y+11,0,0,bbor);
			if (!reset) fb->flood_fill(x,y,r,g,b);
			break;
		}
		case 2: { //badan depan pesawat; x,y sebagai pusat
			fb->draw_line(x-32,y-11,x+33,y-11,0,0,bbor);
			fb->draw_line(x-32,y+11,x+33,y+11,0,0,bbor);
			fb->draw_line(x-32,y-11,x-32,y+11,0,0,bbor);
			fb->draw_line(x+33,y-11,x+33,y+11,0,0,bbor);
			if (!reset) fb->flood_fill(x,y,r,g,b);
			break;
		}
		case 3: { //sayap atas; x,y sebagai titik kiri bawah sayap
			fb->draw_line(x,y,x+44,y-46,0,0,bbor);
			fb->draw_line(x+44,y-46,x+53,y-46,0,0,bbor);
			fb->draw_line(x+53,y-46,x+29,y-10,0,0,bbor);
			fb->draw_line(x+29,y-10,x+31,y,0,0,bbor);
			fb->draw_line(x+31,y,x,y,0,0,bbor);
			if (!reset) fb->flood_fill(x+15,y-5,r,g,b);
			break;
		}
		case 4: { //sayap bawah; x,y sebagai titik kiri atas sayap
			fb->draw_line(x,y,x+44,y+46,0,0,bbor);
			fb->draw_line(x+44,y+46,x+53,y+46,0,0,bbor);
			fb->draw_line(x+53,y+46,x+29,y+10,0,0,bbor);
			fb->draw_line(x+29,y+10,x+31,y,0,0,bbor);
			fb->draw_line(x+31,y,x,y,0,0,bbor);
			if (!reset) fb->flood_fill(x+15,y+5,r,g,b);
			break;
		}
		case 5: { //badan tengah pesawat(diapit sayap); x,y sebagai pusat
			fb->draw_line(x-15,y-11,x+16,y-11,0,0,bbor);
			fb->draw_line(x-15,y+11,x+16,y+11,0,0,bbor);
			fb->draw_line(x-15,y-11,x-15,y+11,0,0,bbor);
			fb->draw_line(x+16,y-11,x+16,y+11,0,0,bbor);
			if (!reset) fb->flood_fill(x,y,r,g,b);
			break;
		}
		case 6: { //badan belakang pesawat (diapit ekor); x,y sebagai pertengahan antara O dan H
			fb->draw_line(x,y-9,x+15,y-11,0,0,bbor);
			fb->draw_line(x+15,y-11,x+15,y+11,0,0,bbor);
			fb->draw_line(x+15,y+11,x,y+9,0,0,bbor);
			fb->draw_line(x,y+9,x-42,y+11,0,0,bbor);
			fb->draw_line(x-42,y+11,x-42,y-11,0,0,bbor);
			fb->draw_line(x-42,y-11,x,y-9,0,0,bbor);
			if (!reset) fb->flood_fill(x,y,r,g,b);
			break;
		}
		case 7: { //ekor atas ; x,y sebagai titik kiri bawah ekor
			fb->draw_line(x,y,x+13,y-19,0,0,bbor);
			fb->draw_line(x+13,y-19,x+24,y-19,0,0,bbor);
			fb->draw_line(x+24,y-19,x+17,y-2,0,0,bbor);
			fb->draw_line(x+17,y-2,x,y,0,0,bbor);
			if (!reset) fb->flood_fill(x+7,y-5,r,g,b);
			break;
		}
		case 8: { //ekor bawah ; x,y sebagai titik kiri atas ekor
			fb->draw_line(x,y,x+13,y+19,0,0,bbor);
			fb->draw_line(x+13,y+19,x+24,y+19,0,0,bbor);
			fb->draw_line(x+24,y+19,x+17,y+2,0,0,bbor);
			fb->draw_line(x+17,y+2,x,y,0,0,bbor);
			if (!reset) fb->flood_fill(x+7,y+5,r,g,b);
			break;
		}
	}
}

void draw_base(int x, int y) {

	fb->draw_line(x, y, x+250, y, 0,112,255);
	fb->draw_line(x, y, x+70, y-50, 0,112,255);
	fb->draw_line(x+70, y-50, x+180, y-50, 0,112,255);
	fb->draw_line(x+180, y-50, x+250, y, 0,112,255);


	fb->draw_line(x+100, y-50, x+100, y-85, 255,255,0);
	fb->draw_line(x+100, y-85, x+150, y-85, 255,255,0);
	fb->draw_line(x+150, y-85, x+150, y-50, 255,255,0);

	fb->flood_fill(x+ 25, y-5, 0,112,255);
	fb->flood_fill(x + 105, y - 55,  255,255,0);
}

void erase_base(int x, int y) {

	fb->draw_line(x, y, x+250, y, 0,0,0);
	fb->draw_line(x, y, x+70, y-50, 0,0,0);
	fb->draw_line(x+70, y-50, x+180, y-50, 0,0,0);
	fb->draw_line(x+180, y-50, x+250, y, 0,0,0);


	fb->draw_line(x+100, y-50, x+100, y-85, 0,0,0);
	fb->draw_line(x+100, y-85, x+150, y-85, 0,0,0);
	fb->draw_line(x+150, y-85, x+150, y-50, 0,0,0);

	fb->reset_fill(x+ 25, y-5, 0,0,0);
	fb->reset_fill(x + 105, y - 55,  0,0,0);
}

void move_base(int xc, int yc) {
	int x = xc, y = yc;
	int delta = 20;
	while(1) {
		draw_base(x,y);
		usleep(100000);
		if (isdestroyed)
			break;
		erase_base(x,y);
		x += delta;
		if (x > 1100)
			x = xc;

		xbullet = x + 125;
	}
}

void drawPecahan(float x1, float y1,float x2, float y2, int r, int g, int b, int num)
{
	// Bresenham's line algorithm
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep){
	  std::swap(x1, y1);
	  std::swap(x2, y2);
	}

	if(x1 > x2) {
	  std::swap(x1, x2);
	  std::swap(y1, y2);
	}

	const float dx = x2 - x1;
	const float dy = fabs(y2 - y1);

	float error = dx / 2.0f;
	const int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	const int maxX = (int)x2;

	for(int x=(int)x1; x<maxX; x++) {
	  if(steep) {
		  selectPart(y,x,r,g,b,num,0);
		  usleep(100000);
		  switch(num) {
			  case 3: {
				fb->reset_fill(x+15,y-5,0,0,0);
				break;
				}
			  case 4: {
				fb->reset_fill(x+15,y+5,0,0,0);
				break;
				}
			  case 7: {
				fb->reset_fill(x+7,y-5,0,0,0);
				break;
				}
			  case 8: {
				fb->reset_fill(x+7,y+5,0,0,0);
				break;
				}
			  default: {
				fb->reset_fill(x,y,0,0,0);
				}
		  }
		  usleep(100000);
		  selectPart(y,x,r,g,b,num,1);
	  }
	  else {
		  selectPart(x,y,r,g,b,num,0);
		  usleep(100000);
		  switch(num) {
			  case 3: {
				fb->reset_fill(x+15,y-5,0,0,0);
				break;
				}
			  case 4: {
				fb->reset_fill(x+15,y+5,0,0,0);
				break;
				}
			  case 7: {
				fb->reset_fill(x+7,y-5,0,0,0);
				break;
				}
			  case 8: {
				fb->reset_fill(x+7,y+5,0,0,0);
				break;
				}
			  default: {
				fb->reset_fill(x,y,0,0,0);
				}
		  }
		  usleep(100000);
		  selectPart(x,y,r,g,b,num,1);
	  }

	  error -= dy;
	  if(error < 0) {
		  y += ystep;
		  error += dx;
	  }
	}
}
//
// void base() {
// 	// draw_base();
// 	move_base();
// }

void draw_bullet(int xbullet, int y2bullet) {
	int a = xbullet - 20;
	int b = xbullet + 20;
	int c = xbullet;

	fb->draw_line(a,y2bullet,b,y2bullet, 255,255,255);
	fb->draw_line(a,y2bullet,a,y2bullet-20, 255,255,255);
	fb->draw_line(b,y2bullet,b,y2bullet-20, 255,255,255);
	fb->draw_line(b,y2bullet-20,c,y2bullet-30, 255,255,255);
	fb->draw_line(a,y2bullet-20,c,y2bullet-30, 255,255,255);
	fb->flood_fill(b -5,y2bullet -5,255,255,255);
}

void erase_bullet(int xbullet, int y2bullet) {
	int a = xbullet - 20;
	int b = xbullet + 20;
	int c = xbullet;

	fb->draw_line(a,y2bullet,b,y2bullet, 0,0,0);
	fb->draw_line(a,y2bullet,a,y2bullet-20, 0,0,0);
	fb->draw_line(b,y2bullet,b,y2bullet-20, 0,0,0);
	fb->draw_line(b,y2bullet-20,c,y2bullet-30, 0,0,0);
	fb->draw_line(a,y2bullet-20,c,y2bullet-30, 0,0,0);
	fb->reset_fill(b -5,y2bullet -5,0,0,0);
}

int getXFront() {
	return xFront;
}

int getXBehind() {
	return xFront + 176;
}

void bounce(int xc, int yc) {

	fb->drawBan1(xc,yc,25,100,0,0);
	bool down = true;
	int posy = yc;
	int accy = 0;
	int gravity = 15;
	// int xl = xc + 25, yl;
	while(1){
		fb->drawBan1(xc,posy,25,100,0,0);
		usleep(100000);

		fb->reset_fill(xc,posy,0,0,0);
		if(down){
			accy = accy + gravity;
			posy += accy;
		}
		else{
			accy = ((accy - gravity) * 7)/8;
			posy -= accy;
		}
		if (posy + 25 >= 700){
			// accy = 10;
			posy = 675;
			down = false;
		}
		if (accy <= 0){
			down = true;
		}
		if ((posy==675)&&(accy==0)) {
			int x = xc ;
			int y = posy ;
			int xl1;
			int yl1;
			int xl2;
			int yl2;
			xl1 = x-25;
			xl2 = x+25;
			yl1 = y;
			yl2 = y;

			while(1){
				fb->drawBan(x,y,xl1,yl1,xl2,yl2,25,100,0,0);
				fb->rotate_point(&xl1,&yl1,30,x,y);
				fb->rotate_point(&xl2,&yl2,30,x,y);
				usleep(100000);
				fb->reset_fill(x,y,0,0,0);

			}
		}
	}
}

void draw_halfcircle(int xc, int yc, int radius, int r, int g, int b) { //menggambar setengah lingkaran
	int x=0;
	int y=radius;
	fb->put_pixel(xc+x,yc-y,r,g,b);

	int p=3-(2*r);

	for(x=0;x<=y;x++) {
		if (p<0) {
			y=y;
			p=(p+(4*x)+6);
		} else {
			y=y-1;
			p=p+((4*(x-y)+10));
		}

		fb->put_pixel(xc+x,yc-y,r,g,b);
		fb->put_pixel(xc-x,yc-y,r,g,b);
		fb->put_pixel(xc+y,yc-x,r,g,b);
		fb->put_pixel(xc-y,yc-x,r,g,b);
	}
	fb->draw_line(xc-radius,yc,xc+radius,yc,r,g,b);
}

void paket_parasut(int x, int y, int r, int g, int b) { //menggambar parasut dengan paket
	if (r==0 && g==0 && b == 0) {
		fb->reset_fill(x,y,0,0,0);
		fb->reset_fill(x,y-60,0,0,0);
		fb->draw_line(x,y-20,x-50,y-50,r,g,b);
		fb->draw_line(x,y-20,x+50,y-50,r,g,b);
		fb->draw_line(x,y-20,x-30,y-50,r,g,b);
		fb->draw_line(x,y-20,x+30,y-50,r,g,b);
		fb->draw_line(x,y-20,x-10,y-50,r,g,b);
		fb->draw_line(x,y-20,x+10,y-50,r,g,b);
	}
	else {
		fb->draw_circle(x,y-10,10,r,g,b);
		fb->flood_fill(x,y-10,0,255,0);
		draw_halfcircle(x,y-50,50,r,g,b);
		fb->flood_fill(x,y-60,r,g,b);
		fb->draw_line(x,y-20,x-50,y-50,r,g,b);
		fb->draw_line(x,y-20,x+50,y-50,r,g,b);
		fb->draw_line(x,y-20,x-30,y-50,r,g,b);
		fb->draw_line(x,y-20,x+30,y-50,r,g,b);
		fb->draw_line(x,y-20,x-10,y-50,r,g,b);
		fb->draw_line(x,y-20,x+10,y-50,r,g,b);
	}
}

void rotating_sayap(int x, int y,int rad, int r, int g, int b){

	fb->drawsayap(x,y,r,g,b);
	int x1 = x + 44;
	int y1 = y - 46;
	int x2 = x + 53;
	int y2 = y - 46;
	int x3 = x + 29;
	int y3 = y - 10;
	int x4 = x + 31;
	int y4 = y;
	int xf = x + 15;
	int yf = y - 5;
	while(x < 1000 && y < 700) {

		fb->reset_fill(xf,yf,0,0,0);
		fb->rotate_point(&x1,&y1,rad,x,y);
		fb->rotate_point(&x2,&y2,rad,x,y);
		fb->rotate_point(&x3,&y3,rad,x,y);
		fb->rotate_point(&x4,&y4,rad,x,y);
		fb->rotate_point(&xf,&yf,rad,x,y);
		fb->draw_line(x,y,x1,y1,255,255,255);
		fb->draw_line(x1,y1,x2,y2,255,255,255);
		fb->draw_line(x2,y2,x3,y3,255,255,255);
		fb->draw_line(x3,y3,x4,y4,255,255,255);
		fb->draw_line(x4,y4,x,y,255,255,255);
		fb->flood_fill(xf,yf,r,g,b);
		x += 2;
		y += 2;
		usleep(100000);
	}
	//fb->flood_fill(xf,yf,r,g,b);
}

void rotating_ekor(int x, int y,int rad, int r, int g, int b){

	fb->draw_line(x,y,x+70,y+5,255,255,255);
	fb->draw_line(x+70,y+5,x+70,y-35,255,255,255);
	fb->draw_line(x+70,y-35,x,y-30,255,255,255);
	fb->draw_line(x,y-30,x,y,255,255,255);
	fb->flood_fill(x+5,y-5,200,200,200);

	// usleep(100000000);

	int x1 = x + 70;
	int y1 = y + 5;
	int x2 = x + 70;
	int y2 = y - 35;
	int x3 = x;
	int y3 = y - 30;
	int xf = x + 25;
	int yf = y - 25;

	while(x < 1000 && y < 600) {

		fb->reset_fill(xf,yf,0,0,0);
		fb->rotate_point(&x1,&y1,rad,x,y);
		fb->rotate_point(&x2,&y2,rad,x,y);
		fb->rotate_point(&x3,&y3,rad,x,y);
		fb->rotate_point(&xf,&yf,rad,x,y);
		fb->draw_line(x,y,x1,y1,255,255,255);
		fb->draw_line(x1,y1,x2,y2,255,255,255);
		fb->draw_line(x2,y2,x3,y3,255,255,255);
		fb->draw_line(x3,y3,x,y,255,255,255);
		fb->flood_fill(xf,yf,r,g,b);
		x -= 3;
		y += 3;
		usleep(100000);
	}
	fb->flood_fill(xf,yf,r,g,b);
}

void draw_parasut(int x, int y) {
	// int x = 200;
	int a = x;
	// int y = 200;
	int b = y;
	for(i=1;i<90;i++) {
		paket_parasut(x,y,0,0,0);
		fb->rotate_point(&x,&y,-1,a+100,b);
		paket_parasut(x,y,100,0,0);
		usleep(5000);
	}
	for(i=1;i<90;i++) {
		paket_parasut(x,y,0,0,0);
		fb->rotate_point(&x,&y,1,a,b+100);
		paket_parasut(x,y,100,0,0);
		usleep(5000);
	}
	for(i=1;i<90;i++) {
		paket_parasut(x,y,0,0,0);
		fb->rotate_point(&x,&y,-1,a+100,b+200);
		paket_parasut(x,y,100,0,0);
		usleep(5000);
	}
	for(i=1;i<90;i++) {
		paket_parasut(x,y,0,0,0);
		fb->rotate_point(&x,&y,1,a,b+300);
		paket_parasut(x,y,100,0,0);
		usleep(5000);
	}
}


void drawpesawat(){

	// int deltay = 60 ;

	int deltax = 1100;
	int n = 0;
	while (1)
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

		usleep(100000);

		fb->reset_fill(deltax+90,deltay,0,0,0);
		if (isdestroyed) {
			break;
		}

		deltax = deltax-13;
		if (deltax <= 200) {
			deltax = 1100;
		}
		xFront = deltax;
		//printf("%d\n", xFront);
	}
	// usleep(10000000);
	thread t1(draw_parasut,deltax - 50,deltay);
	thread t2(bounce, deltax - 100, deltay);
	thread t3(rotating_sayap, deltax + 200, deltay, 40, 100, 100, 200);
	thread t4(rotating_ekor, deltax + 300, deltay, -40, 100, 100, 200);
	// // thread t2(drawPecahan,deltax-200,deltay+100, 400,500, 255,255,255, 2);
	// // thread t3(drawPecahan,deltax-100,deltay+100, 500,500, 255,255,255, 3);
	// // thread t4(drawPecahan,deltax+100,deltay+100, 600,500, 255,255,255, 6);
	// // thread t5(drawPecahan,deltax+200,deltay+100, 700,500, 255,255,255, 7);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
}

int main() {

	fb = new FrameBuffer;

	// rotating_ekor(400, 200, -20, 100, 100, 200);
	// bounce(100,30);

/////////////////////////////////////////MAIN START HERE
	int baseX = 75, baseY = 700;
	thread t1(move_base, baseX, baseY);
	thread t2(drawpesawat);

	int oldbullet;

	while(!isdestroyed) {
		int ch = getchar();
	    if(ch == 10){
	        oldbullet = xbullet;
			int ybullet = baseY - 90;
			draw_bullet(oldbullet,ybullet);

			while(ybullet - 50 > 0){
				erase_bullet(oldbullet,ybullet);
				y1bullet -= 5;
	            ybullet -= 5;
				if( (ybullet - 40 <= deltay +100) && (oldbullet - 10 > xFront && oldbullet + 10 < xFront + 200)){
					isdestroyed = 1;
					break;
				}
				draw_bullet(oldbullet,ybullet);

	            usleep(9000);
	        }

			if (!isdestroyed)
	        	erase_bullet(oldbullet,ybullet);

			oldbullet = xbullet;
	        y1bullet = 550;
	        y2bullet = 500;
	    }
	}



	t1.join();
	t2.join();
////////////////////////////////////////////?MAIN ENDS HERE
	delete fb;
	return 0;
}
