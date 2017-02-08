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

void draw_base() {
	a = 750;
		b = 800;
		c = 1000;
		d = 1050;
		e = 800;
		f = 800;
		g = 1000;
		h = 1000;
		i = 880;
		j = 880;
		k = 920;
		m = 920;
	fb->draw_line(a,650,d,650,0,0,255);
	fb->draw_line(e,650,f,600,0,0,255);
	fb->draw_line(f,600,g,600,0,0,255);
	fb->draw_line(g,600,h,650,0,0,255);

	/* meriam */
	fb->draw_line(i,600,j,550,0,255,0);
	fb->draw_line(j,550,k,550,0,255,0);
	fb->draw_line(k,550,m,600,0,255,0);

//	fb->put_pixel(j+ 5, 555, 255,255,255);
//		fb->put_pixel(f + 5, 605, 200,200,200);
	fb->flood_fill(j+ 5, 555, 255,255,255);
	fb->flood_fill(f + 5, 605,  200,200,200);

}

void move_base() {
	int delta = -20;
		while(1){
			a = 750;
			b = 800;
			c = 1000;
			d = 1050;
			e = 800;
			f = 800;
			g = 1000;
			h = 1000;
			i = 880;
			j = 880;
			k = 920;
			m = 920;

			/* deck kapal */
			// fb->draw_line(a,650,b,700,0,0,255);
			// fb->draw_line(b,700,c,700,0,0,255);
			// fb->draw_line(a,650,d,650,0,0,255);
			// fb->draw_line(c,700,d,650,0,0,255);

			/* dock kapal */
			fb->draw_line(a,650,d,650,0,0,255);
			fb->draw_line(e,650,f,600,0,0,255);
			fb->draw_line(f,600,g,600,0,0,255);
			fb->draw_line(g,600,h,650,0,0,255);

			/* meriam */
			fb->draw_line(i,600,j,550,0,255,0);
			fb->draw_line(j,550,k,550,0,255,0);
			fb->draw_line(k,550,m,600,0,255,0);

				fb->flood_fill(j+ 5, 555, 255,255,255);
				fb->flood_fill(f + 5, 605,  200,200,200);

			while(a > 0){
				/* deck kapal */
				// fb->draw_line(a,650,b,700,0,0,0);
				// fb->draw_line(b,700,c,700,0,0,0);
				// fb->draw_line(c,700,d,650,0,0,0);

				/* dock kapal */
				fb->draw_line(a,650,d,650,0,0,0);
				fb->draw_line(e,650,f,600,0,0,0);
				fb->draw_line(f,600,g,600,0,0,0);
				fb->draw_line(g,600,h,650,0,0,0);

				/* meriam */
				fb->draw_line(i,600,j,550,0,0,0);
				fb->draw_line(j,550,k,550,0,0,0);
				fb->draw_line(k,550,m,600,0,0,0);

					fb->reset_fill(j+ 5, 555, 0,0,0);
					fb->reset_fill(f + 5, 605,  0,0,0);
				a += delta;
				b += delta;
				c += delta;
				d += delta;
				e += delta;
				f += delta;
				g += delta;
				h += delta;
				i += delta;
				j += delta;
				k += delta;
				m += delta;

				xbullet = ( j + k )/2;
				/* deck kapal */
				fb->draw_line(a,650,d,650,0,0,255);

				/* dock kapal */
				fb->draw_line(e,650,f,600,0,0,255);
				fb->draw_line(f,600,g,600,0,0,255);
				fb->draw_line(g,600,h,650,0,0,255);

				/* meriam */
				fb->draw_line(i,600,j,550,0,255,0);
				fb->draw_line(j,550,k,550,0,255,0);
				fb->draw_line(k,550,m,600,0,255,0);

					fb->flood_fill(j+ 5, 555, 255,255,255);
					fb->flood_fill(f + 5, 605,  200,200,200);
				usleep(50000);
				}
			/* deck kapal */
			fb->draw_line(a,650,d,650,0,0,0);

			/* dock kapal */
			fb->draw_line(e,650,f,600,0,0,0);
			fb->draw_line(f,600,g,600,0,0,0);
			fb->draw_line(g,600,h,650,0,0,0);

			/* meriam */
			fb->draw_line(i,600,j,550,0,0,0);
			fb->draw_line(j,550,k,550,0,0,0);
			fb->draw_line(k,550,m,600,0,0,0);

				fb->reset_fill(j+ 5, 555, 0,0,0);
				fb->reset_fill(f + 5, 605,  0,0,0);

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

void drawpesawat(){

	// int deltay = 60 ;

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

		if (isdestroyed) {
			break;
		}
		fb->flood_fill(deltax+90,deltay,0,0,150);

		usleep(10000);
		if (isdestroyed) {
			break;
		}

		fb->reset_fill(deltax+90,deltay,0,0,0);

		deltax = deltax-3;
		if (deltax <= 200) {
			deltax = 1100;
			n++;
		}
		xFront = deltax;
		//printf("%d\n", xFront);
	}
	// usleep(10000000);
	thread t1(drawPecahan,deltax-300,deltay+100,300,500, 255,255,255, 1);
	thread t2(drawPecahan,deltax-200,deltay+100, 400,500, 255,255,255, 2);
	thread t3(drawPecahan,deltax-100,deltay+100, 500,500, 255,255,255, 3);
	thread t4(drawPecahan,deltax+100,deltay+100, 600,500, 255,255,255, 6);
	thread t5(drawPecahan,deltax+200,deltay+100, 700,500, 255,255,255, 7);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	// drawPecahan(deltax,deltay, 500,700, 255,255,255, 2);
	// drawPecahan(deltax,deltay, 500,1000, 255,255,255, 3);
	// drawPecahan(deltax,deltay, 500,1000, 255,255,255, 4);
	// drawPecahan(deltax,deltay, 500,1000, 255,255,255, 5);

}

void base() {
	draw_base();
	// usleep(10000000);
	move_base();
}

void draw_bullet(int xbullet) {
	int a = xbullet - 20;
	int b = xbullet + 20;
	int c = xbullet;

	fb->draw_line(a,y2bullet,b,y2bullet, 255,255,255);
	fb->draw_line(a,y2bullet,a,y2bullet-20, 255,255,255);
	fb->draw_line(b,y2bullet,b,y2bullet-20, 255,255,255);
	fb->draw_line(b,y2bullet-20,c,y2bullet-30, 255,255,255);
	fb->draw_line(a,y2bullet-20,c,y2bullet-30, 255,255,255);
	fb->flood_fill(b -5,y2bullet -5,255,255,255);
	// fb->draw_line(a,y2bullet,b,y2bullet, 255,255,255);
	// fb->draw_line(a,y2bullet,b,y2bullet, 255,255,255);
	// // fb->draw_line();
}

void erase_bullet(int xbullet) {
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

int main() {
	fb = new FrameBuffer;
	//drawExplosion(500,500);
	//usleep(1000000);
	// drawPecahan(50,50,500,500,255,255,255,0);

	//draw_bullet(200);
	//usleep(10000000);
	//GAMBAR PESAWAT

	 //	 system("clear");
	  thread t1(base);
	  thread t2(drawpesawat);

	// drawpesawat();
	//base();
	int oldbullet;
	while(!isdestroyed) {
		int ch = getchar();
        if(ch == 10){
            oldbullet = xbullet;
			draw_bullet(oldbullet);
			while(y2bullet - 75 > 0){
				erase_bullet(oldbullet);
				y1bullet -= 5;
                y2bullet -= 5;
				if( (y2bullet - 40 <= deltay +100) && (oldbullet - 10 > xFront && oldbullet + 10 < xFront + 200)){
                  isdestroyed = 1;

				//   while (1) {
				// 	  printf("destroyd");
				//   }
                  break;
                }

				draw_bullet(oldbullet);
                // collision checking


                usleep(9000);
            }
			if (!isdestroyed)
            	erase_bullet(oldbullet);
			oldbullet = xbullet;
            y1bullet = 550;
            y2bullet = 500;
        }
  }

	t1.join();
	t2.join();
	// printf("%i\n", fb->isBlack(150,150));
	// printf("%i\n", fb->isBlack(100,100));
	// printf("%i\n", fb->isBlack(400,500));

	//selectPart(500,500,0,255,0,8,0);
	//drawPecahan(200,200,500,500,0,255,0,1);
	delete fb;
	return 0;
}
