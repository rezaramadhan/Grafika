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


void drawBan1(int xc, int yc, int radius, int r, int g, int b) {
	fb->draw_circle(xc,yc,radius,r,g,b);
	//fb->flood_fill(xc,yc,100,100,0);
	fb->draw_line_clip(xc-radius,yc,xc+radius,yc,r,g,b);
}

void drawBan(int xc, int yc, int xl1, int yl1, int xl2, int yl2, int radius, int r, int g, int b) {
	fb->draw_circle(xc,yc,radius,r,g,b);
	//flood_fill(xc,yc,0,100,100);
	fb->draw_line_clip(xl1,yl1,xl2,yl2,r,g,b);
	//draw_line_clip(xc-radius,yc,xc+radius,yc,r,g,b);
	//draw_line_clip(xc,yc-radius,xc,yc+radius,r,g,b);
}

void drawsayap(int x, int y,int r,int g,int b) {
	fb->draw_line_clip(x,y,x+44,y-46,0,0,150);
	fb->draw_line_clip(x+44,y-46,x+53,y-46,0,0,150);
	fb->draw_line_clip(x+53,y-46,x+29,y-10,0,0,150);
	fb->draw_line_clip(x+29,y-10,x+31,y,0,0,150);
	fb->draw_line_clip(x+31,y,x,y,0,0,150);
	//flood_fill(x+15,y-5,r,g,b);
}
void draw_base(int x, int y) {

	fb->draw_line_clip(x, y, x+250, y, 0,112,255);
	fb->draw_line_clip(x, y, x+70, y-50, 0,112,255);
	fb->draw_line_clip(x+70, y-50, x+180, y-50, 0,112,255);
	fb->draw_line_clip(x+180, y-50, x+250, y, 0,112,255);


	fb->draw_line_clip(x+100, y-50, x+100, y-85, 0,112,255);
	fb->draw_line_clip(x+100, y-85, x+150, y-85, 0,112,255);
	fb->draw_line_clip(x+150, y-85, x+150, y-50, 0,112,255);

	//fb->flood_fill(x+ 25, y-5, 0,112,255);
	//fb->flood_fill(x + 105, y - 55,  255,255,0);
}

void erase_base(int x, int y) {

	fb->draw_line_clip(x, y, x+250, y, 0,0,0);
	fb->draw_line_clip(x, y, x+70, y-50, 0,0,0);
	fb->draw_line_clip(x+70, y-50, x+180, y-50, 0,0,0);
	fb->draw_line_clip(x+180, y-50, x+250, y, 0,0,0);


	fb->draw_line_clip(x+100, y-50, x+100, y-85, 0,0,0);
	fb->draw_line_clip(x+100, y-85, x+150, y-85, 0,0,0);
	fb->draw_line_clip(x+150, y-85, x+150, y-50, 0,0,0);

	// fb->reset_fill(x+ 25, y-5, 0,0,0);
	// fb->reset_fill(x + 105, y - 55,  0,0,0);
}

void move_base(int xc, int yc) {
	int x = xc, y = yc;
	int delta = 30;
	while(1) {
		draw_base(x,y);
		usleep(150000);

		if (isdestroyed)
			break;

		erase_base(x,y);
		x += delta;
		if (x > 1100)
			x = xc;

		xbullet = x + 125;
	}
}

void draw_bullet(int xbullet, int y2bullet) {
	int a = xbullet - 20;
	int b = xbullet + 20;
	int c = xbullet;

	fb->draw_line_clip(a,y2bullet,b,y2bullet, 255,255,255);
	fb->draw_line_clip(a,y2bullet,a,y2bullet-20, 255,255,255);
	fb->draw_line_clip(b,y2bullet,b,y2bullet-20, 255,255,255);
	fb->draw_line_clip(b,y2bullet-20,c,y2bullet-30, 255,255,255);
	fb->draw_line_clip(a,y2bullet-20,c,y2bullet-30, 255,255,255);
	//fb->flood_fill(b -5,y2bullet -5,255,255,255);
}

void erase_bullet(int xbullet, int y2bullet) {
	int a = xbullet - 20;
	int b = xbullet + 20;
	int c = xbullet;

	fb->draw_line_clip(a,y2bullet,b,y2bullet, 0,0,0);
	fb->draw_line_clip(a,y2bullet,a,y2bullet-20, 0,0,0);
	fb->draw_line_clip(b,y2bullet,b,y2bullet-20, 0,0,0);
	fb->draw_line_clip(b,y2bullet-20,c,y2bullet-30, 0,0,0);
	fb->draw_line_clip(a,y2bullet-20,c,y2bullet-30, 0,0,0);
	// fb->reset_fill(b -5,y2bullet -5,0,0,0);
}

int getXFront() {
	return xFront;
}

int getXBehind() {
	return xFront + 176;
}

void bounce(int xc, int yc) {

	drawBan1(xc,yc,25,100,0,0);
	bool down = true;
	int posy = yc;
	int accy = 0;
	int gravity = 15;
	// int xl = xc + 25, yl;
	while(1){
		drawBan1(xc,posy,25,100,0,0);
		usleep(100000);

		//fb->reset_fill(xc,posy,0,0,0);
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
				drawBan(x,y,xl1,yl1,xl2,yl2,25,100,0,0);
				fb->rotate_point(&xl1,&yl1,30,x,y);
				fb->rotate_point(&xl2,&yl2,30,x,y);
				usleep(100000);
				//fb->reset_fill(x,y,0,0,0);

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
	fb->draw_line_clip(xc-radius,yc,xc+radius,yc,r,g,b);
}

void paket_parasut(int x, int y, int r, int g, int b) { //menggambar parasut dengan paket
	if (r==0 && g==0 && b == 0) {
		// fb->reset_fill(x,y,0,0,0);
		// fb->reset_fill(x,y-60,0,0,0);
		fb->draw_line_clip(x,y-20,x-50,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x+50,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x-30,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x+30,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x-10,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x+10,y-50,r,g,b);
	}
	else {
		fb->draw_circle(x,y-10,10,r,g,b);
		//fb->flood_fill(x,y-10,0,255,0);
		draw_halfcircle(x,y-50,50,r,g,b);
		//fb->flood_fill(x,y-60,r,g,b);
		fb->draw_line_clip(x,y-20,x-50,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x+50,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x-30,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x+30,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x-10,y-50,r,g,b);
		fb->draw_line_clip(x,y-20,x+10,y-50,r,g,b);
	}
}

void rotating_sayap(int x, int y,int rad, int r, int g, int b){

	//drawsayap(x,y,r,g,b);
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

		//fb->reset_fill(xf,yf,0,0,0);
		fb->rotate_point(&x1,&y1,rad,x,y);
		fb->rotate_point(&x2,&y2,rad,x,y);
		fb->rotate_point(&x3,&y3,rad,x,y);
		fb->rotate_point(&x4,&y4,rad,x,y);
		fb->rotate_point(&xf,&yf,rad,x,y);
		fb->draw_line_clip(x,y,x1,y1,255,255,255);
		fb->draw_line_clip(x1,y1,x2,y2,255,255,255);
		fb->draw_line_clip(x2,y2,x3,y3,255,255,255);
		fb->draw_line_clip(x3,y3,x4,y4,255,255,255);
		fb->draw_line_clip(x4,y4,x,y,255,255,255);
		//fb->flood_fill(xf,yf,r,g,b);

		usleep(100000);
		fb->draw_line_clip(x,y,x1,y1,0,0,0);
		fb->draw_line_clip(x1,y1,x2,y2,0,0,0);
		fb->draw_line_clip(x2,y2,x3,y3,0,0,0);
		fb->draw_line_clip(x3,y3,x4,y4,0,0,0);
		fb->draw_line_clip(x4,y4,x,y,0,0,0);

		x += 2;
		y += 2;
	}
	////fb->flood_fill(xf,yf,r,g,b);
}

void rotating_ekor(int x, int y,int rad, int r, int g, int b){

	// fb->draw_line_clip(x,y,x+70,y+5,255,255,255);
	// fb->draw_line_clip(x+70,y+5,x+70,y-35,255,255,255);
	// fb->draw_line_clip(x+70,y-35,x,y-30,255,255,255);
	// fb->draw_line_clip(x,y-30,x,y,255,255,255);
	//fb->flood_fill(x+5,y-5,200,200,200);

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

		//fb->reset_fill(xf,yf,0,0,0);
		fb->rotate_point(&x1,&y1,rad,x,y);
		fb->rotate_point(&x2,&y2,rad,x,y);
		fb->rotate_point(&x3,&y3,rad,x,y);
		fb->rotate_point(&xf,&yf,rad,x,y);
		fb->draw_line_clip(x,y,x1,y1,255,255,255);
		fb->draw_line_clip(x1,y1,x2,y2,255,255,255);
		fb->draw_line_clip(x2,y2,x3,y3,255,255,255);
		fb->draw_line_clip(x3,y3,x,y,255,255,255);


		//fb->flood_fill(xf,yf,r,g,b);
		usleep(100000);

		fb->draw_line_clip(x,y,x1,y1,0,0,0);
		fb->draw_line_clip(x1,y1,x2,y2,0,0,0);
		fb->draw_line_clip(x2,y2,x3,y3,0,0,0);
		fb->draw_line_clip(x3,y3,x,y,0,0,0);

		x -= 3;
		y += 3;
	}
	//fb->flood_fill(xf,yf,r,g,b);
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
	fb->wxoffset = deltax;
	fb->vxoffset = deltax;

	fb->draw_view();
	int n = 0;
	while (1)
	{
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
		fb->clear_view();
		// fb->reset_fill(deltax+90,deltay,0,0,0);
		if (isdestroyed) {
			break;
		}

		deltax = deltax-13;
		fb->wxoffset -= 13;
		fb->vxoffset -= 13;
		fb->draw_view();
		if (deltax <= 200) {
			deltax = 1100;
			fb->wxoffset = deltax;
			fb->vxoffset = deltax;
		}
		xFront = deltax;
		//printf("%d\n", xFront);
	}
	// usleep(10000000);
	// thread t1(draw_parasut,deltax - 50,deltay);
	// thread t2(bounce, deltax - 100, deltay);
	thread t3(rotating_sayap, deltax + 200, deltay, 40, 100, 100, 200);
	thread t4(rotating_ekor, deltax + 300, deltay, -40, 100, 100, 200);
	// // thread t2(drawPecahan,deltax-200,deltay+100, 400,500, 255,255,255, 2);
	// // thread t3(drawPecahan,deltax-100,deltay+100, 500,500, 255,255,255, 3);
	// // thread t4(drawPecahan,deltax+100,deltay+100, 600,500, 255,255,255, 6);
	// // thread t5(drawPecahan,deltax+200,deltay+100, 700,500, 255,255,255, 7);
	// t1.join();
	// t2.join();
	t3.join();
	t4.join();
}

int main() {

	fb = new FrameBuffer;
	fb->wxsize=360;
	fb->wysize=580;
	fb->wxoffset=300;
	fb->wyoffset=100;

	fb->vxsize=360;
	fb->vysize=580;
	fb->vxoffset=300;
	fb->vyoffset=100;

	//fb->draw_view();

	//usleep(10000000);

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
