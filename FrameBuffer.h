#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <bits/stdc++.h>
#define TOP 8
#define BOTTOM 4
#define RIGHT 2
#define LEFT 1
#define IMG_X_SIZE 400
#define IMG_Y_SIZE 500
#define MINIMAP_X_LOC 800
#define MINIMAP_Y_LOC 90
using namespace std;

class FrameBuffer {

	private:
		int fbfd;
	    struct fb_var_screeninfo vinfo;
	    struct fb_fix_screeninfo finfo;
	    long int screensize;
	    char *fbp;

	public:
		int vxoffset;
		int vyoffset;
		int vxsize;
		int vysize;
		int wxoffset;
		int wyoffset;
		int wxsize;
		int wysize;


		FrameBuffer() {
		    // Open the file for reading and writing
		    fbfd = open("/dev/fb0", O_RDWR);
		    if (fbfd == -1) {
		        perror("Error: cannot open FrameBuffer device");
		        exit(1);
		    }

		    // Get fixed screen information
		    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		        perror("Error reading fixed information");
		        exit(2);
		    }

		    // Get variable screen information
		    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		        perror("Error reading variable information");
		        exit(3);
		    }

		    // Figure out the size of the screen in bytes
		    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

		    // Map the device to memory
		    if (!(fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0))) {
		        perror("Error: failed to map FrameBuffer device to memory");
		        exit(4);
		    }

			//reset fb to true black value;
			for(int i = 0; i<vinfo.xres - 5; i++)
				for(int j = 0; j<vinfo.yres - 10; j++)
					put_pixel(i,j,0,0,0);

			vxsize = vinfo.xres / 4 * 3;
			vysize = vinfo.yres / 4 * 3;

			vxoffset = vinfo.xres / 8 * 1;
			vyoffset = vinfo.yres / 8 * 1;

			wxsize = vinfo.xres; wysize = vinfo.yres; wxoffset = 0; wyoffset = 0;
		}

		~FrameBuffer(){
			close(fbfd);
		}

		void put_pixel(int x, int y, int red, int green, int blue) {
            long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
			//printf("%d,%d\n", x,y);
			// printf("Before - R%i ", (char)*(fbp + location + 0));
			// printf("B%i ", (char)*(fbp + location + 1));
			// printf("R%i\n", (char)*(fbp + location + 2));
			if (vinfo.bits_per_pixel == 32) {
                *(fbp + location + 0) = (char) blue;
                *(fbp + location + 1) = (char) green;
                *(fbp + location + 2) = (char) red;
                *(fbp + location + 3) = (char) 0;
				// printf("After  - R%i ", (char)*(fbp + location + 0));
				// printf("B%i ", (char)*(fbp + location + 1));
				// printf("R%i\n\n", (char)*(fbp + location + 2));
            } else  {
                int b = blue;
                int g = green;
                int r = red;
                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }
		}
		int is_black(int x, int y) {
			long int loc = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
			for (int i = 0; i < 3; i++) {

				//printf("v%i\n", *(fbp + loc + i));
				if ((char) *(fbp + loc + i) != 0)
					return 0;
				//*(fbp + loc + i) = 255;
			}
			return 1;
		}
		void flood_fill(int x, int y, int r, int g, int b) {
			if ((is_black(x,y))) {
				put_pixel(x,y,r,g,b);
				//usleep(1000);
				flood_fill(x+1,y,r,g,b);
				flood_fill(x-1,y,r,g,b);
				flood_fill(x,y+1,r,g,b);
				flood_fill(x,y-1,r,g,b);

			}
		}

		void flood_fill_clip(int x, int y, int r, int g, int b) {
			remap_point(&x,&y);
			flood_fill(x,y,r,g,b);
		}

		void draw_line(float x1, float y1,float x2, float y2, int r, int g, int b)
		{
			// first and last pixel
			put_pixel(x1,y1,r,g,b);
			put_pixel(x2,y2,r,g,b);

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
			      put_pixel(y,x,r,g,b);
			  }
			  else {
			      put_pixel(x,y,r,g,b);
			  }

			  error -= dy;
			  if(error < 0) {
			      y += ystep;
			      error += dx;
			  }
			}
		}

		void reset_fill_clip(int x, int y, int r, int g, int b) {
			remap_point(&x,&y);
			reset_fill(x,y,r,g,b);
		}


		void reset_fill(int x, int y, int r, int g, int b) {
			if (!(is_black(x,y))) {
				put_pixel(x,y,r,g,b);
				//usleep(1000);
				reset_fill(x+1,y,r,g,b);
				reset_fill(x-1,y,r,g,b);
				reset_fill(x,y+1,r,g,b);
				reset_fill(x,y-1,r,g,b);

			}

		}

		void draw_circle(int xc, int yc, int radius, int r, int g, int b) {
			int x=0;
			int y=radius;
			put_pixel(xc+x,yc-y,r,g,b);

			int p=3-(2*r);

			for(x=0;x<=y;x++) {
				if (p<0) {
					y=y;
					p=(p+(4*x)+6);
				} else {
					y=y-1;
					p=p+((4*(x-y)+10));
				}

				put_pixel(xc+x,yc-y,r,g,b);
				put_pixel(xc-x,yc-y,r,g,b);
				put_pixel(xc+x,yc+y,r,g,b);
				put_pixel(xc-x,yc+y,r,g,b);
				put_pixel(xc+y,yc-x,r,g,b);
				put_pixel(xc-y,yc-x,r,g,b);
				put_pixel(xc+y,yc+x,r,g,b);
				put_pixel(xc-y,yc+x,r,g,b);
			}
		}

		void rotate_point(int *x, int *y, int angle, int rotation_center_x, int rotation_center_y) {
			//convert angle to radians
			double rad_angle = angle * M_PI / 180;
			// printf("%f\n",rad_angle);
			//translate to origin
			double dx = *x - rotation_center_x;
			double dy = *y - rotation_center_y;
			// printf("%f %f\n", dx,dy);
			//rotate point
			double new_dx = (dx) * cos(rad_angle) - (dy) * sin(rad_angle);
			double new_dy = (dx) * sin(rad_angle) + (dy) * cos(rad_angle);
			// printf("%f %f\n", dx,dy);

			*x = (int)round(new_dx) + rotation_center_x;
			*y = (int)round(new_dy) + rotation_center_y;
		}

		void draw_view() {
			draw_line(vxoffset, vyoffset, vxoffset + vxsize, vyoffset, 22, 22, 22);
			draw_line(vxoffset+vxsize, vyoffset, vxoffset+vxsize, vyoffset+vysize, 22,22,22);
			draw_line(vxoffset+vxsize, vyoffset+vysize, vxoffset, vyoffset+vysize, 22,22,22);
			draw_line(vxoffset, vyoffset+vysize, vxoffset ,vyoffset ,22,22,22);
		}

		void clear_view() {
			draw_line(vxoffset, vyoffset, vxoffset + vxsize, vyoffset, 0,0,0);
			draw_line(vxoffset+vxsize, vyoffset, vxoffset+vxsize, vyoffset+vysize, 0,0,0);
			draw_line(vxoffset+vxsize, vyoffset+vysize, vxoffset, vyoffset+vysize, 0,0,0);
			draw_line(vxoffset, vyoffset+vysize, vxoffset ,vyoffset ,0,0,0);
		}

		void remap_point(int *x, int *y) {
			double newx = *x;
			double newy = *y;

			newx -= wxoffset;
			newy -= wyoffset;

			newx *= ((double) vxsize / (double) wxsize);
			newy *= ((double) vysize / (double) wysize);

			newx += vxoffset;
			newy += vyoffset;

			*x = (int) newx;
			*y = (int) newy;
		}

		int find_region (int x, int y) {
			int retval = 0;

			if (x < wxoffset)
			 	retval |= 1;
			else if (x > (wxoffset + wxsize))
				retval |= 2;

			if (y < wyoffset)
				retval |= 8;
			else if (y > (wyoffset + wysize))
				retval |= 4;

			return retval;
		}

		void draw_line_clip(int xt1, int yt1, int xt2, int yt2, int r, int g, int b) {
			//printf("awal %d %d %d %d\n", xt1, yt1, xt2, yt2);

			int outcode0 = find_region(xt1, yt1);
			int outcode1 = find_region(xt2, yt2);

			bool accept = false;

			double x1 = (double) xt1;
			double y1 = (double) yt1;
			double x2 = (double) xt2;
			double y2 = (double) yt2;
			double ymin = (double) wyoffset;
			double ymax = (double) wyoffset + wysize;
			double xmin = (double) wxoffset;
			double xmax = (double) wxoffset + wxsize;

			while (true) {
				if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
					accept = true;
					// printf("acc\n" );
					break;
				} else if (outcode0 & outcode1) { // Bitwise AND is not 0. (implies both end points are in the same region outside the window). Reject and get out of loop
					accept = false;
					// printf("reject\n" );
					break;
				} else {
					// failed both tests, so calculate the line segment to clip
					// from an outside point to an intersection with clip edge
					double x, y;

					// At least one endpoint is outside the clip rectangle; pick it.
					int outcodeOut = outcode0 ? outcode0 : outcode1;

					// Now find the intersection point;
					// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
					if (outcodeOut & TOP) {           // point is above the clip rectangle
						x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1);
						y = ymin;
					} else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
						x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1);
						y = ymax;
					} else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
						y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1);
						x = xmax;
					} else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
						y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1);
						x = xmin;
					}

					// Now we move outside point to intersection point to clip
					// and get ready for next pass.
					if (outcodeOut == outcode0) {
						x1 = x;
						y1 = y;
						outcode0 = find_region((int) x1, (int)y1);
					} else {
						x2 = x;
						y2 = y;
						outcode1 = find_region((int)x2, (int)y2);
					}
				}
			}

			if (accept) {
				xt1 = x1;
				xt2 = x2;
				yt1 = y1;
				yt2 = y2;

				//printf("awal %d %d %d %d\n", xt1, yt1, xt2, yt2);
				remap_point(&xt1, &yt1);
				remap_point(&xt2, &yt2);
				//printf("remap %d %d %d %d\n", xt1, yt1, xt2, yt2);
				draw_line(xt1, yt1, xt2, yt2, r, g, b);
			}
		}


		void drawpesawat(){

	int deltay = 60 ;
	//FrameBuffer *fb = new FrameBuffer;
	int deltax = 1100;
	int n = 0;
	//while (1)
	//{
		draw_line_clip(deltax+0,3+deltay,deltax+16, 11+deltay,0,0,150);
		draw_line_clip(deltax+16,11+deltay,deltax+81,11+deltay,0,0,150);
		draw_line_clip(deltax+81,11+deltay,deltax+125,57+deltay,0,0,150);
		draw_line_clip(deltax+125,57+deltay,deltax+134,57+deltay,0,0,150);
		draw_line_clip(deltax+134,57+deltay,deltax+110,21+deltay,0,0,150);
		draw_line_clip(deltax+110,21+deltay,deltax+112,11+deltay,0,0,150);
		draw_line_clip(deltax+112,11+deltay,deltax+154,9+deltay,0,0,150);
		draw_line_clip(deltax+154,9+deltay,deltax+167,28+deltay,0,0,150);
		draw_line_clip(deltax+167,28+deltay,deltax+176,28+deltay,0,0,150);
		draw_line_clip(deltax+176,28+deltay,deltax+169,11+deltay,0,0,150);
		draw_line_clip(deltax+169,11+deltay,deltax+169,-11+deltay,0,0,150);
		draw_line_clip(deltax+169,-11+deltay,deltax+176,-28+deltay,0,0,150);
		draw_line_clip(deltax+176,-28+deltay,deltax+167,-28+deltay,0,0,150);
		draw_line_clip(deltax+167,-28+deltay,deltax+154,-9+deltay,0,0,150);
		draw_line_clip(deltax+154,-9+deltay,deltax+112,-11+deltay,0,0,150);
		draw_line_clip(deltax+112,-11+deltay,deltax+110,-21+deltay,0,0,150);
		draw_line_clip(deltax+110,-21+deltay,deltax+134,-57+deltay,0,0,150);
		draw_line_clip(deltax+134,-57+deltay,deltax+125,-57+deltay,0,0,150);
		draw_line_clip(deltax+125,-57+deltay,deltax+81,-11+deltay,0,0,150);
		draw_line_clip(deltax+81,-11+deltay,deltax+16,-11+deltay,0,0,150);
		draw_line_clip(deltax+16,-11+deltay,deltax+0,-3+deltay,0,0,150);
		draw_line_clip(deltax+0,3+deltay,deltax+0,0+deltay,0,0,150);
		draw_line_clip(deltax+0,-3+deltay,deltax+0,0+deltay,0,0,150);
		//fb->flood_fill(deltax+90,deltay,0,0,150);

		//usleep(100000);

		//fb->reset_fill(deltax+90,deltay,0,0,0);
		//if (isdestroyed) {
		//	break;
		//}

		//deltax = deltax-13;
		//if (deltax <= 200) {
		//	deltax = 1100;
		//}
		//xFront = deltax;
		//printf("%d\n", xFront);
	//}
	// usleep(10000000);
	//thread t1(draw_parasut,deltax - 50,deltay);
	//thread t2(bounce, deltax - 100, deltay);
	//thread t3(rotating_sayap, deltax + 200, deltay, 40, 100, 100, 200);
	//thread t4(rotating_ekor, deltax + 300, deltay, -40, 100, 100, 200);
	// // thread t2(drawPecahan,deltax-200,deltay+100, 400,500, 255,255,255, 2);
	// // thread t3(drawPecahan,deltax-100,deltay+100, 500,500, 255,255,255, 3);
	// // thread t4(drawPecahan,deltax+100,deltay+100, 600,500, 255,255,255, 6);
	// // thread t5(drawPecahan,deltax+200,deltay+100, 700,500, 255,255,255, 7);
	//t1.join();
	//t2.join();
	//t3.join();
	//t4.join();
}

		void draw_bangunan(){
			//kelompok 2
			//coklat
			draw_line(231,394,250,405,255,255,255);
			draw_line(250,405,244,415,255,255,255);
			draw_line(244,415,223,415,255,255,255);
			draw_line(223,415,220,413,255,255,255);
			draw_line(220,413,231,394,255,255,255);

			//ungu
			draw_line(173,343,172,349,255,255,255);
			draw_line(172,349,153,349,255,255,255);
			draw_line(153,349,153,343,255,255,255);
			draw_line(153,343,173,343,255,255,255);

			//coklat
			draw_line(210,343,210,349,255,255,255);
			draw_line(210,349,190,349,255,255,255);
			draw_line(190,349,190,343,255,255,255);
			draw_line(190,343,210,343,255,255,255);

			//Albar
			draw_line(149,311,149,315,255,255,255);
			draw_line(149,315,155,315,255,255,255);
			draw_line(155,315,155,317,255,255,255);
			draw_line(155,317,158,317,255,255,255);
			draw_line(158,317,151,326,255,255,255);
			draw_line(151,326,151,335,255,255,255);
			draw_line(151,335,122,335,255,255,255);
			draw_line(122,335,122,328,255,255,255);
			draw_line(122,328,115,328,255,255,255);
			draw_line(115,328,115,317,255,255,255);
			draw_line(115,317,119,317,255,255,255);
			draw_line(119,317,119,314,255,255,255);
			draw_line(119,314,125,315,255,255,255);
			draw_line(125,315,125,311,255,255,255);
			draw_line(125,311,149,311,255,255,255);

			//Altim
			draw_line(237,311,237,315,255,255,255);
			draw_line(237,315,242,315,255,255,255);
			draw_line(242,315,242,317,255,255,255);
			draw_line(242,317,247,317,255,255,255);
			draw_line(247,317,247,327,255,255,255);
			draw_line(247,327,240,327,255,255,255);
			draw_line(240,327,240,335,255,255,255);
			draw_line(240,335,211,335,255,255,255);
			draw_line(211,335,211,327,255,255,255);
			draw_line(211,327,204,327,255,255,255);
			draw_line(204,327,204,318,255,255,255);
			draw_line(204,318,208,318,255,255,255);
			draw_line(208,318,208,315,255,255,255);
			draw_line(208,315,213,315,255,255,255);
			draw_line(213,315,213,311,255,255,255);
			draw_line(213,311,237,311,255,255,255);

			//kelompok3
			//HMTL
			draw_line(234,235,244,235,255,255,255);
			draw_line(244,235,244,238,255,255,255);
			draw_line(244,238,302,238,255,255,255);
			draw_line(302,238,302,250,255,255,255);
			draw_line(302,250,286,250,255,255,255);
			draw_line(286,250,286,253,255,255,255);
			draw_line(286,253,280,253,255,255,255);
			draw_line(280,253,280,250,255,255,255);
			draw_line(280,250,244,250,255,255,255);
			draw_line(244,250,244,253,255,255,255);
			draw_line(244,253,234,253,255,255,255);
			draw_line(234,253,234,235,255,255,255);

			// L. IX A
			draw_line(234,259,269,259,255,255,255);
			draw_line(269,259,269,273,255,255,255);
			draw_line(269,273,250,273,255,255,255);
			draw_line(250,273,250,282,255,255,255);
			draw_line(250,282,234,282,255,255,255);
			draw_line(234,282,234,259,255,255,255);

			// L. IX C
			draw_line(272,259,304,259,255,255,255);
			draw_line(304,259,304,273,255,255,255);
			draw_line(304,273,272,273,255,255,255);
			draw_line(272,273,272,259,255,255,255);

			// Kotak Coklat kecil
			draw_line(267,274,275,274,255,255,255);
			draw_line(275,274,275,281,255,255,255);
			draw_line(275,281,267,281,255,255,255);
			draw_line(267,281,267,274,255,255,255);

			// L. IX B
			draw_line(272,281,304,281,255,255,255);
			draw_line(304,281,304,295,255,255,255);
			draw_line(304,295,272,295,255,255,255);
			draw_line(272,295,272,281,255,255,255);

			// merah besar
			draw_line(243,282,269,282,255,255,255);
			draw_line(269,282,269,295,255,255,255);
			draw_line(269,295,242,295,255,255,255);
			draw_line(242,295,243,282,255,255,255);

			// merah kecil 1
			draw_line(247,298,256,298,255,255,255);
			draw_line(256,298,256,311,255,255,255);
			draw_line(256,311,247,311,255,255,255);
			draw_line(247,311,247,298,255,255,255);

			// merah kecil 2
			draw_line(258,298,268,298,255,255,255);
			draw_line(268,298,268,311,255,255,255);
			draw_line(268,311,258,311,255,255,255);
			draw_line(258,311,258,298,255,255,255);

			//merah kecil 3
			draw_line(256,312,268,312,255,255,255);
			draw_line(268,312,268,320,255,255,255);
			draw_line(268,320,256,320,255,255,255);
			draw_line(256,320,256,312,255,255,255);

			// merah tulisan aneh
			draw_line(270,300,280,300,255,255,255);
			draw_line(280,300,280,306,255,255,255);
			draw_line(280,306,282,306,255,255,255);
			draw_line(282,306,282,314,255,255,255);
			draw_line(282,314,284,314,255,255,255);
			draw_line(284,314,270,325,255,255,255);
			draw_line(270,325,270,300,255,255,255);

			//Kelompok 4
			//Lab UMH
			draw_line(272,177,318,177,255,255,255);
			draw_line(318,177,318,164,255,255,255);
			draw_line(318,164,272,164,255,255,255);
			draw_line(272,164,272,177,255,255,255);

			//GKU Timur
			draw_line(284,180,318,180,255,255,255);
			draw_line(318,180,318,195,255,255,255);
			draw_line(318,195,312,196,255,255,255);
			draw_line(312,196,312,192,255,255,255);
			draw_line(312,192,298,192,255,255,255);
			draw_line(298,192,298,196,255,255,255);
			draw_line(298,196,284,196,255,255,255);
			draw_line(284,196,284,180,255,255,255);

			// M Tek Geodesi
			draw_line(308,197,321,197,255,255,255);
			draw_line(321,197,321,212,255,255,255);
			draw_line(321,212,308,212,255,255,255);
			draw_line(308,212,308,197,255,255,255);

			//M ?
			draw_line(308,212,316,212,255,255,255);
			draw_line(316,212,316,225,255,255,255);
			draw_line(316,225,308,225,255,255,255);
			draw_line(308,225,308,212,255,255,255);

			// M ?2
			draw_line(310,228,316,228,255,255,255);
			draw_line(316,228,316,225,255,255,255);
			draw_line(316,225,321,225,255,255,255);
			draw_line(321,225,321,230,255,255,255);
			draw_line(321,230,318,230,255,255,255);
			draw_line(318,230,318,233,255,255,255);
			draw_line(318,233,321,233,255,255,255);
			draw_line(321,233,321,236,255,255,255);
			draw_line(321,236,316,236,255,255,255);
			draw_line(316,236,316,234,255,255,255);
			draw_line(316,234,310,233,255,255,255);
			draw_line(310,233,310,228,255,255,255);

			// Area parkir
			draw_line(268,200,297,200,255,255,255);
			draw_line(297,200,297,225,255,255,255);
			draw_line(297,225,268,225,255,255,255);
			draw_line(268,225,268,225,255,255,255);

			// AP
			draw_line(268,283,277,183,255,255,255);
			draw_line(277,183,277,177,255,255,255);
			draw_line(277,177,281,177,255,255,255);
			draw_line(281,177,281,182,255,255,255);
			draw_line(281,182,284,183,255,255,255);
			draw_line(284,183,284,195,255,255,255);
			draw_line(284,195,267,196,255,255,255);
			draw_line(267,196,268,283,255,255,255);

			//Kelompok 5
			//Kelompok 6
			// CC Barat
			draw_line(143,240,174,240,255,255,255);
			draw_line(174,240,143,251,255,255,255);
			draw_line(143,251,174,251,255,255,255);
			draw_line(174,251,143,240,255,255,255);

			// Lap basket
			draw_line(146,256,170,256,255,255,255);
			draw_line(170,256,146,287,255,255,255);
			draw_line(146,287,170,287,255,255,255);
			draw_line(170,287,146,256,255,255,255);

			// Gedung ??
			draw_line(147,288,147,304,255,255,255);
			draw_line(147,304,127,304,255,255,255);
			draw_line(127,304,127,288,255,255,255);
			draw_line(127,288,147,288,255,255,255);

			//Kelompok 7
			//Teknik Sipil
			draw_line(62,272,86,272,255,255,255);
			draw_line(86,272,86,281,255,255,255);
			draw_line(86,281,74,281,255,255,255);
			draw_line(74,281,74,285,255,255,255);
			draw_line(74,285,111,285,255,255,255);
			draw_line(111,285,111,280,255,255,255);
			draw_line(111,280,125,280,255,255,255);
			draw_line(125,280,125,308,255,255,255);
			draw_line(125,308,70,308,255,255,255);
			draw_line(70,308,70,299,255,255,255);
			draw_line(70,299,98,299,255,255,255);
			draw_line(98,299,98,294,255,255,255);
			draw_line(98,294,62,294,255,255,255);
			draw_line(62,294,62,272,255,255,255);

			//Fisika HIMAFI
			draw_line(60,233,125,233,255,255,255);
			draw_line(125,233,125,277,255,255,255);
			draw_line(125,277,89,277,255,255,255);
			draw_line(89,277,89,269,255,255,255);
			draw_line(89,269,118,269,255,255,255);
			draw_line(118,269,118,266,255,255,255);
			draw_line(118,266,60,266,255,255,255);
			draw_line(60,266,60,233,255,255,255);

			//Kelompok 8
			//Kelompok 9
			//Kelompok 13
			// A
			draw_line(30,111,78,111,255,255,255);
			draw_line(78,111,78,138,255,255,255);
			draw_line(78,138,30,138,255,255,255);
			draw_line(30,138,30,111,255,255,255);

			// B
			draw_line(30,140,55,140,255,255,255);
			draw_line(55,140,55,153,255,255,255);
			draw_line(55,153,30,153,255,255,255);
			draw_line(30,153,30,140,255,255,255);

			// C
			draw_line(30,155,55,155,255,255,255);
			draw_line(55,155,55,158,255,255,255);
			draw_line(55,158,65,158,255,255,255);
			draw_line(65,158,65,153,255,255,255);
			draw_line(65,153,75,153,255,255,255);
			draw_line(75,153,75,169,255,255,255);
			draw_line(75,169,65,169,255,255,255);
			draw_line(65,169,65,166,255,255,255);
			draw_line(65,166,55,166,255,255,255);
			draw_line(55,166,55,169,255,255,255);
			draw_line(55,169,30,169,255,255,255);
			draw_line(30,169,30,155,255,255,255);

			// D
			draw_line(46,94,71,94,255,255,255);
			draw_line(71,94,71,99,255,255,255);
			draw_line(71,99,73,99,255,255,255);
			draw_line(73,99,73,103,255,255,255);
			draw_line(73,103,71,103,255,255,255);
			draw_line(71,103,71,108,255,255,255);
			draw_line(71,108,47,108,255,255,255);
			draw_line(47,108,46,99,255,255,255);
			draw_line(46,99,33,99,255,255,255);
			draw_line(33,99,33,97,255,255,255);
			draw_line(33,97,46,94,255,255,255);

			// Kelompok 14
			// 1
			draw_line(52,80,77,80,255,255,255);
			draw_line(77,80,77,89,255,255,255);
			draw_line(77,89,53,89,255,255,255);
			draw_line(53,89,52,80,255,255,255);
			// 2
			draw_line(49,72,53,72,255,255,255);
			draw_line(53,72,52,89,255,255,255);
			draw_line(52,89,49,89,255,255,255);
			draw_line(49,89,49,72,255,255,255);
			// 3
			draw_line(34,75,48,75,255,255,255);
			draw_line(48,75,48,85,255,255,255);
			draw_line(48,85,34,85,255,255,255);
			draw_line(34,85,34,75,255,255,255);
			//4
			draw_line(39,62,85,62,255,255,255);
			draw_line(85,62,85,71,255,255,255);
			draw_line(85,71,39,71,255,255,255);
			draw_line(39,71,39,62,255,255,255);
			//5
			draw_line(49,52,97,52,255,255,255);
			draw_line(97,52,97,59,255,255,255);
			draw_line(97,59,49,59,255,255,255);
			draw_line(49,59,49,52,255,255,255);

			//6
			draw_line(48,37,97,37,255,255,255);
			draw_line(97,37,97,46,255,255,255);
			draw_line(97,46,48,46,255,255,255);
			draw_line(48,46,48,37,255,255,255);
			// 7
			draw_line(80,23,95,23,255,255,255);
			draw_line(95,23,91,37,255,255,255);
			draw_line(91,37,84,37,255,255,255);
			draw_line(84,37,80,23,255,255,255);

			//Kelompok 16
			// GSG
			draw_line(113,46,142,46,255,255,255);
			draw_line(142,46,142,74,255,255,255);
			draw_line(142,74,113,74,255,255,255);
			draw_line(113,74,113,46,255,255,255);

			// KPP
			draw_line(140,31,163,31,255,255,255);
			draw_line(163,31,169,37,255,255,255);
			draw_line(169,37,169,75,255,255,255);
			draw_line(169,75,145,75,255,255,255);
			draw_line(145,75,145,42,255,255,255);
			draw_line(145,42,140,42,255,255,255);
			draw_line(140,42,140,31,255,255,255);

			//Tunnel
			draw_line(170,35,174,25,255,255,255);
			draw_line(174,25,175,33,255,255,255);
			draw_line(175,33,185,33,255,255,255);
			draw_line(185,33,186,35,255,255,255);
			draw_line(186,35,190,33,255,255,255);
			draw_line(190,33,190,78,255,255,255);
			draw_line(190,78,186,78,255,255,255);
			draw_line(186,78,185,80,255,255,255);
			draw_line(185,80,175,80,255,255,255);
			draw_line(175,80,174,78,255,255,255);
			draw_line(174,78,170,78,255,255,255);
			draw_line(170,78,170,35,255,255,255);

			//Perpus
			draw_line(192,49,194,49,255,255,255);
			draw_line(194,49,194,40,255,255,255);
			draw_line(194,40,200,35,255,255,255);
			draw_line(200,35,216,35,255,255,255);
			draw_line(216,35,221,40,255,255,255);
			draw_line(221,40,221,49,255,255,255);
			draw_line(221,49,227,51,255,255,255);
			draw_line(227,51,227,59,255,255,255);
			draw_line(227,59,225,29,255,255,255);
			draw_line(225,29,225,68,255,255,255);
			draw_line(225,68,218,74,255,255,255);
			draw_line(218,74,201,74,255,255,255);
			draw_line(201,74,197,68,255,255,255);
			draw_line(197,68,197,58,255,255,255);
			draw_line(197,58,192,57,255,255,255);
			draw_line(192,57,192,49,255,255,255);

			// AP
			draw_line(231,33,263,33,255,255,255);
			draw_line(263,33,263,71,255,255,255);
			draw_line(263,71,258,71,255,255,255);
			draw_line(258,71,258,77,255,255,255);
			draw_line(258,77,231,77,255,255,255);
			draw_line(231,77,231,33,255,255,255);

			//Gedung P2T
			//Halaman
			draw_line(267,12,267,46,255,255,255);
			draw_line(267,46,307,49,255,255,255);
			draw_line(307,49,307,12,255,255,255);
			draw_line(307,12,267,12,255,255,255);

			///Gedung kiri atas
			draw_line(271,19,271,23,255,255,255);
			draw_line(271,23,279,23,255,255,255);
			draw_line(279,23,279,19,255,255,255);
			draw_line(279,19,271,19,255,255,255);

			//Gedung kiri bawah
			draw_line(270,26,280,26,255,255,255);
			draw_line(280,26,280,45,255,255,255);
			draw_line(280,45,270,44,255,255,255);
			draw_line(270,44,270,26,255,255,255);

			//Gedung kanan atas
			draw_line(283,13,302,13,255,255,255);
			draw_line(302,13,302,24,255,255,255);
			draw_line(302,24,283,23,255,255,255);
			draw_line(283,23,283,13,255,255,255);

			//Gedung kanan bawah
			draw_line(285,25,298,26,255,255,255);
			draw_line(298,26,298,46,255,255,255);
			draw_line(298,46,284,45,255,255,255);
			draw_line(284,45,285,25,255,255,255);

			//Labtek 4
			// Halaman kiri
			draw_line(267,85,277,68,255,255,255);
			draw_line(277,68,277,76,255,255,255);
			draw_line(277,76,289,69,255,255,255);
			draw_line(289,69,289,67,255,255,255);
			draw_line(289,67,320,69,255,255,255);
			draw_line(320,69,319,75,255,255,255);
			draw_line(319,75,305,75,255,255,255);
			draw_line(305,75,305,73,255,255,255);
			draw_line(305,73,293,73,255,255,255);
			draw_line(293,73,292,82,255,255,255);
			draw_line(292,82,278,82,255,255,255);
			draw_line(278,82,267,91,255,255,255);
			draw_line(267,91,267,85,255,255,255);

			//Gedung kecil belakang
			draw_line(330,71,330,76,255,255,255);
			draw_line(330,76,339,76,255,255,255);
			draw_line(339,76,339,71,255,255,255);
			draw_line(339,71,330,71,255,255,255);

			//Kimia
			//Gedung bawah
			draw_line(326,154,326,141,255,255,255);
			draw_line(326,141,273,136,255,255,255);
			draw_line(273,136,272,149,255,255,255);
			draw_line(272,149,326,154,255,255,255);

			//Gedung atas
			draw_line(275,114,274,119,255,255,255);
			draw_line(274,119,321,134,255,255,255);
			draw_line(321,134,322,118,255,255,255);
			draw_line(322,118,275,114,255,255,255);

			//BscB
			//Gedung Panjang
			draw_line(274,102,274,109,255,255,255);
			draw_line(274,109,334,114,255,255,255);
			draw_line(334,114,337,84,255,255,255);
			draw_line(337,84,330,83,255,255,255);
			draw_line(330,83,328,106,255,255,255);
			draw_line(328,106,274,102,255,255,255);

			//Gedung dalam
			//yang besar
			draw_line(309,94,309,104,255,255,255);
			draw_line(309,104,329,96,255,255,255);
			draw_line(329,96,328,106,255,255,255);
			draw_line(328,106,309,94,255,255,255);

			//yang kecil
			draw_line(303,95,303,102,255,255,255);
			draw_line(303,102,309,102,255,255,255);
			draw_line(309,102,309,96,255,255,255);
			draw_line(309,96,303,95,255,255,255);

			//FTTM
			draw_line(291,98,303,99,255,255,255);
			draw_line(303,99,305,73,255,255,255);
			draw_line(305,73,293,72,255,255,255);
			draw_line(293,72,291,98,255,255,255);

			//Sebelah FTTM
			//Horizontal
			draw_line(305,75,305,78,255,255,255);
			draw_line(305,78,319,79,255,255,255);
			draw_line(319,79,319,76,255,255,255);
			draw_line(319,76,305,75,255,255,255);

			//vertikal
			draw_line(320,70,318,95,255,255,255);
			draw_line(318,95,324,70,255,255,255);
			draw_line(324,70,322,95,255,255,255);
			draw_line(322,95,320,70,255,255,255);

			//kelompok 5
			draw_line(227,301,227,290,255,255,255);
			draw_line(227,290,213,290,255,255,255);
			draw_line(213,290,213,301,255,255,255);
			draw_line(213,301,227,301,255,255,255);

			draw_line(187,285,219,285,255,255,255);
			draw_line(219,285,219,258,255,255,255);
			draw_line(219,258,187,258,255,255,255);
			draw_line(187,258,187,285,255,255,255);

			draw_line(180,230,234,230,255,255,255);
			draw_line(234,230,234,234,255,255,255);
			draw_line(234,234,184,234,255,255,255);
			draw_line(184,234,184,288,255,255,255);
			draw_line(184,288,228,288,255,255,255);
			draw_line(228,288,228,275,255,255,255);
			draw_line(228,275,226,275,255,255,255);
			draw_line(226,275,226,260,255,255,255);
			draw_line(226,260,228,260,255,255,255);
			draw_line(228,260,228,258,255,255,255);
			draw_line(228,258,231,258,255,255,255);
			draw_line(231,258,180,230,255,255,255);

			//kelompok 8
			draw_line(40,282,51,282,255,255,255);
			draw_line(51,282,51,308,255,255,255);
			draw_line(51,308,40,308,255,255,255);
			draw_line(40,308,40,282,255,255,255);

			draw_line(54,277,27,277,255,255,255);
			draw_line(27,277,27,270,255,255,255);
			draw_line(27,270,54,270,255,255,255);
			draw_line(54,270,54,277,255,255,255);

			draw_line(31,277,31,280,255,255,255);
			draw_line(31,280,28,280,255,255,255);
			draw_line(28,280,28,277,255,255,255);
			draw_line(28,277,31,277,255,255,255);

			draw_line(26,259,26,249,255,255,255);
			draw_line(26,249,56,249,255,255,255);
			draw_line(56,249,56,259,255,255,255);
			draw_line(56,259,26,259,255,255,255);

			draw_line(33,249,33,235,255,255,255);
			draw_line(33,235,56,235,255,255,255);
			draw_line(56,235,56,249,255,255,255);
			draw_line(56,249,33,249,255,255,255);

			draw_line(36,259,30,265,255,255,255);
			draw_line(30,265,36,269,255,255,255);
			draw_line(36,269,43,265,255,255,255);
			draw_line(43,265,36,259,255,255,255);

			//kelompok 10
			draw_line(105,217,105,200,255,255,255);
			draw_line(105,200,174,200,255,255,255);
			draw_line(174,200,174,217,255,255,255);
			draw_line(174,217,105,217,255,255,255);

			draw_line(185,217,185,200,255,255,255);
			draw_line(185,200,255,200,255,255,255);
			draw_line(255,200,255,217,255,255,255);
			draw_line(255,217,185,217,255,255,255);

			draw_line(105,190,105,173,255,255,255);
			draw_line(105,173,174,173,255,255,255);
			draw_line(174,173,174,190,255,255,255);
			draw_line(174,190,105,190,255,255,255);

			draw_line(185,190,185,173,255,255,255);
			draw_line(185,173,255,173,255,255,255);
			draw_line(255,173,255,190,255,255,255);
			draw_line(255,190,185,190,255,255,255);

			//kelompok 12
			draw_line(88,94,124,94,255,255,255);
			draw_line(124,94,121,114,255,255,255);
			draw_line(121,114,88,114,255,255,255);
			draw_line(88,114,88,94,255,255,255);

			draw_line(88,120,124,120,255,255,255);
			draw_line(124,120,124,140,255,255,255);
			draw_line(124,140,88,140,255,255,255);
			draw_line(88,140,88,120,255,255,255);

			draw_line(88,140,88,149,255,255,255);
			draw_line(88,149,95,149,255,255,255);
			draw_line(95,149,95,140,255,255,255);
			draw_line(95,140,88,140,255,255,255);

			draw_line(104,140,104,149,255,255,255);
			draw_line(104,149,111,149,255,255,255);
			draw_line(111,149,111,140,255,255,255);
			draw_line(111,140,104,140,255,255,255);

			//kelompok 11
			//oktagon
			draw_line(157,88,143,101,255,255,255);
			draw_line(143,101,157,115,255,255,255);
			draw_line(157,115,173,101,255,255,255);
			draw_line(173,101,157,88,255,255,255);

			//tvst
			draw_line(157,120,143,133,255,255,255);
			draw_line(143,133,157,147,255,255,255);
			draw_line(157,147,173,133,255,255,255);
			draw_line(173,133,157,120,255,255,255);

			//Lab Fidas
			draw_line(189,105,189,97,255,255,255);
			draw_line(189,97,223,97,255,255,255);
			draw_line(223,97,223,113,255,255,255);
			draw_line(223,113,196,113,255,255,255);
			draw_line(196,113,189,105,255,255,255);

			//Lab
			draw_line(196,122,223,122,255,255,255);
			draw_line(223,122,223,138,255,255,255);
			draw_line(223,138,191,135,255,255,255);
			draw_line(191,135,188,133,255,255,255);
			draw_line(188,133,196,122,255,255,255);

			draw_line(226,94,249,94,255,255,255);
			draw_line(249,94,249,139,255,255,255);
			draw_line(249,139,226,139,255,255,255);
			draw_line(226,139,226,94,255,255,255);


			draw_line(226,139,249,139,255,255,255);
			draw_line(249,139,249,158,255,255,255);
			draw_line(249,158,226,158,255,255,255);
			draw_line(226,158,226,139,255,255,255);

			//GKU BARAT
			draw_line(64,177,75,176,255,255,255);
			draw_line(75,176,82,184,255,255,255);
			draw_line(82,184,82,195,255,255,255);
			draw_line(82,195,74,202,255,255,255);
			draw_line(74,202,64,203,255,255,255);
			draw_line(64,203,55,195,255,255,255);
			draw_line(55,195,55,184,255,255,255);
			draw_line(55,184,64,177,255,255,255);

/*
			string testline;
		    string word[1500];

		    ifstream Test ("data.txt");

		    if (!Test)
		    {
			//cout << "There was a problem opening the file. Press any key to close.\n";
			//getch();
			return 0;
		    }

		    int i=0;

		    while ( getline (Test,testline, ' ') )
		    {
		      word[i]=testline;
			    //cout << word[i];
			    i++;
		    }

		    Test.close();

		    //cout<<"\nArray contents:\n";
		    	int k = 0;
		    	//cout << i << endl;
		    	int xawal;
			    int yawal;
			    int xtemp;
			    int ytemp;
			    int xskrg;
			   	int yskrg;
		    	while (k < i){
					//cout<<"masuk"<<endl;
					//cout<<k<<endl;
			    		int batas = (atoi(word[k].c_str()));
			    		for (int n = k; n < k + batas; n++){
			    			if (n == k){
			    				xawal = atoi(word[k+((n-k)*2)+1].c_str());
			    				yawal = atoi(word[k+((n-k)*2)+2].c_str());
			    				xtemp = atoi(word[k+((n-k)*2)+1].c_str());
			    				ytemp = atoi(word[k+((n-k)*2)+2].c_str());
							//cout<<"masuk2"<<endl;
							//cout<<"xskrg : "<<xskrg<<endl;
							//cout<<"yskrg : "<<yskrg<<endl;
							//cout<<"xtemp : "<<xtemp<<endl;
							//cout<<"ytemp : "<<ytemp<<endl;
			    			}
			    			else if(n == k + batas - 1){
			    				yskrg = atoi(word[k+((n-k)*2)+2].c_str());
			    				xskrg = atoi(word[k+((n-k)*2)+1].c_str());
			    				draw_line(xskrg,yskrg,xtemp,ytemp,255,255,255);
			    				draw_line(xskrg,yskrg,xawal,yawal,255,255,255);
							//cout<<"xskrg : "<<xskrg<<endl;
							//cout<<"yskrg : "<<yskrg<<endl;
							//cout<<"xtemp : "<<xtemp<<endl;
							//cout<<"ytemp : "<<ytemp<<endl;
							//cout<<"masuk3"<<endl;
			    			}
			    			else{
			    				yskrg = atoi(word[k+((n-k)*2)+2].c_str());
			    				xskrg = atoi(word[k+((n-k)*2)+1].c_str());
			    				draw_line(xskrg,yskrg,xtemp,ytemp,255,255,255);
			    				xtemp = xskrg;
			    				ytemp = yskrg;
			    			}
			    		}
			    		k = k + (batas * 2) + 1;
					//cout<<k<<endl;
					//cout<<batas<<endl;
			    	}
*/
}



		void draw_street(){
			//Jl.Taman Sari - Ganeca
			draw_line(45,404,42,414,128,128,128);
			draw_line(45,404,8,280,128,128,128);
			draw_line(45,404,151,376,128,128,128);
			draw_line(151,376,148,414,128,128,128);
			draw_line(151,376,181,372,128,128,128);
			draw_line(181,372,287,402,128,128,128);
			draw_line(287,402,287,414,128,128,128);
			draw_line(287,402,356,402,128,128,128);

			//itb kanan
			draw_line(306,231,308,297,128,128,128);
			draw_line(308,297,292,300,128,128,128);
			draw_line(292,300,291,335,128,128,128);
			draw_line(291,335,287,402,128,128,128);
			draw_line(266,231,266,12,128,128,128);
			draw_line(266,50,341,56,128,128,128);
			draw_line(266,111,326,116,128,128,128);
			draw_line(266,132,326,137,128,128,128);


			//itb tengah
			draw_line(181,372,181,335,128,128,128);
			draw_line(181,335,71,335,128,128,128);
			draw_line(181,335,181,231,128,128,128);
			draw_line(181,231,58,231,128,128,128);
			draw_line(58,231,23,231,128,128,128);
			draw_line(181,231,306,231,128,128,128);
			draw_line(181,335,291,335,128,128,128);
			draw_line(181,231,181,160,128,128,128);
			draw_line(181,160,181,83,128,128,128);
			draw_line(181,29,181,16,128,128,128);
			draw_line(80,16,181,16,128,128,128);
			draw_line(181,16,219,1,128,128,128);


			//itb kiri
			draw_line(23,231,23,294,128,128,128);
			draw_line(23,294,44,330,128,128,128);
			draw_line(44,330,71,335,128,128,128);
			draw_line(58,231,58,315,128,128,128);
			draw_line(58,315,71,335,128,128,128);
			draw_line(23,231,25,173,128,128,128);
			draw_line(25,173,82,173,128,128,128);
			draw_line(82,173,95,173,128,128,128);
			draw_line(95,173,95,199,128,128,128);
			draw_line(95,199,102,202,128,128,128);
			draw_line(102,202,103,231,128,128,128);

			draw_line(106,83,107,28,128,128,128);
			draw_line(107,28,266,33,128,128,128);
			draw_line(82,173,82,83,128,128,128);
			draw_line(82,83,266,83,128,128,128);
			draw_line(82,118,266,118,128,128,128);
			draw_line(82,160,266,160,128,128,128);
			draw_line(25,173,28,46,128,128,128);
			draw_line(28,46,51,28,128,128,128);
			draw_line(51,28,80,28,128,128,128);
			draw_line(80,28,80,16,128,128,128);
			draw_line(80,16,48,16,128,128,128);
			draw_line(48,16,21,50,128,128,128);
			draw_line(21,50,8,280,128,128,128);
		}

		void draw_bangunan_clip(int r, int g, int b){
			//coklat
			draw_line_clip(231,394,250,405,r,g,b);
			draw_line_clip(250,405,244,415,r,g,b);
			draw_line_clip(244,415,223,415,r,g,b);
			draw_line_clip(223,415,220,413,r,g,b);
			draw_line_clip(220,413,231,394,r,g,b);

			//ungu
			draw_line_clip(173,343,172,349,r,g,b);
			draw_line_clip(172,349,153,349,r,g,b);
			draw_line_clip(153,349,153,343,r,g,b);
			draw_line_clip(153,343,173,343,r,g,b);

			//coklat
			draw_line_clip(210,343,210,349,r,g,b);
			draw_line_clip(210,349,190,349,r,g,b);
			draw_line_clip(190,349,190,343,r,g,b);
			draw_line_clip(190,343,210,343,r,g,b);

			//Albar
			draw_line_clip(149,311,149,315,r,g,b);
			draw_line_clip(149,315,155,315,r,g,b);
			draw_line_clip(155,315,155,317,r,g,b);
			draw_line_clip(155,317,158,317,r,g,b);
			draw_line_clip(158,317,151,326,r,g,b);
			draw_line_clip(151,326,151,335,r,g,b);
			draw_line_clip(151,335,122,335,r,g,b);
			draw_line_clip(122,335,122,328,r,g,b);
			draw_line_clip(122,328,115,328,r,g,b);
			draw_line_clip(115,328,115,317,r,g,b);
			draw_line_clip(115,317,119,317,r,g,b);
			draw_line_clip(119,317,119,314,r,g,b);
			draw_line_clip(119,314,125,315,r,g,b);
			draw_line_clip(125,315,125,311,r,g,b);
			draw_line_clip(125,311,149,311,r,g,b);

			//Altim
			draw_line_clip(237,311,237,315,r,g,b);
			draw_line_clip(237,315,242,315,r,g,b);
			draw_line_clip(242,315,242,317,r,g,b);
			draw_line_clip(242,317,247,317,r,g,b);
			draw_line_clip(247,317,247,327,r,g,b);
			draw_line_clip(247,327,240,327,r,g,b);
			draw_line_clip(240,327,240,335,r,g,b);
			draw_line_clip(240,335,211,335,r,g,b);
			draw_line_clip(211,335,211,327,r,g,b);
			draw_line_clip(211,327,204,327,r,g,b);
			draw_line_clip(204,327,204,318,r,g,b);
			draw_line_clip(204,318,208,318,r,g,b);
			draw_line_clip(208,318,208,315,r,g,b);
			draw_line_clip(208,315,213,315,r,g,b);
			draw_line_clip(213,315,213,311,r,g,b);
			draw_line_clip(213,311,237,311,r,g,b);

			//kelompok3
			//HMTL
			draw_line_clip(234,235,244,235,r,g,b);
			draw_line_clip(244,235,244,238,r,g,b);
			draw_line_clip(244,238,302,238,r,g,b);
			draw_line_clip(302,238,302,250,r,g,b);
			draw_line_clip(302,250,286,250,r,g,b);
			draw_line_clip(286,250,286,253,r,g,b);
			draw_line_clip(286,253,280,253,r,g,b);
			draw_line_clip(280,253,280,250,r,g,b);
			draw_line_clip(280,250,244,250,r,g,b);
			draw_line_clip(244,250,244,253,r,g,b);
			draw_line_clip(244,253,234,253,r,g,b);
			draw_line_clip(234,253,234,235,r,g,b);

			// L. IX A
			draw_line_clip(234,259,269,259,r,g,b);
			draw_line_clip(269,259,269,273,r,g,b);
			draw_line_clip(269,273,250,273,r,g,b);
			draw_line_clip(250,273,250,282,r,g,b);
			draw_line_clip(250,282,234,282,r,g,b);
			draw_line_clip(234,282,234,259,r,g,b);

			// L. IX C
			draw_line_clip(272,259,304,259,r,g,b);
			draw_line_clip(304,259,304,273,r,g,b);
			draw_line_clip(304,273,272,273,r,g,b);
			draw_line_clip(272,273,272,259,r,g,b);

			// Kotak Coklat kecil
			draw_line_clip(267,274,275,274,r,g,b);
			draw_line_clip(275,274,275,281,r,g,b);
			draw_line_clip(275,281,267,281,r,g,b);
			draw_line_clip(267,281,267,274,r,g,b);

			// L. IX B
			draw_line_clip(272,281,304,281,r,g,b);
			draw_line_clip(304,281,304,295,r,g,b);
			draw_line_clip(304,295,272,295,r,g,b);
			draw_line_clip(272,295,272,281,r,g,b);

			// merah besar
			draw_line_clip(243,282,269,282,r,g,b);
			draw_line_clip(269,282,269,295,r,g,b);
			draw_line_clip(269,295,242,295,r,g,b);
			draw_line_clip(242,295,243,282,r,g,b);

			// merah kecil 1
			draw_line_clip(247,298,256,298,r,g,b);
			draw_line_clip(256,298,256,311,r,g,b);
			draw_line_clip(256,311,247,311,r,g,b);
			draw_line_clip(247,311,247,298,r,g,b);

			// merah kecil 2
			draw_line_clip(258,298,268,298,r,g,b);
			draw_line_clip(268,298,268,311,r,g,b);
			draw_line_clip(268,311,258,311,r,g,b);
			draw_line_clip(258,311,258,298,r,g,b);

			//merah kecil 3
			draw_line_clip(256,312,268,312,r,g,b);
			draw_line_clip(268,312,268,320,r,g,b);
			draw_line_clip(268,320,256,320,r,g,b);
			draw_line_clip(256,320,256,312,r,g,b);

			// merah tulisan aneh
			draw_line_clip(270,300,280,300,r,g,b);
			draw_line_clip(280,300,280,306,r,g,b);
			draw_line_clip(280,306,282,306,r,g,b);
			draw_line_clip(282,306,282,314,r,g,b);
			draw_line_clip(282,314,284,314,r,g,b);
			draw_line_clip(284,314,270,325,r,g,b);
			draw_line_clip(270,325,270,300,r,g,b);

			//Kelompok 4
			//Lab UMH
			draw_line_clip(272,177,318,177,r,g,b);
			draw_line_clip(318,177,318,164,r,g,b);
			draw_line_clip(318,164,272,164,r,g,b);
			draw_line_clip(272,164,272,177,r,g,b);

			//GKU Timur
			draw_line_clip(284,180,318,180,r,g,b);
			draw_line_clip(318,180,318,195,r,g,b);
			draw_line_clip(318,195,312,196,r,g,b);
			draw_line_clip(312,196,312,192,r,g,b);
			draw_line_clip(312,192,298,192,r,g,b);
			draw_line_clip(298,192,298,196,r,g,b);
			draw_line_clip(298,196,284,196,r,g,b);
			draw_line_clip(284,196,284,180,r,g,b);

			// M Tek Geodesi
			draw_line_clip(308,197,321,197,r,g,b);
			draw_line_clip(321,197,321,212,r,g,b);
			draw_line_clip(321,212,308,212,r,g,b);
			draw_line_clip(308,212,308,197,r,g,b);

			//M ?
			draw_line_clip(308,212,316,212,r,g,b);
			draw_line_clip(316,212,316,225,r,g,b);
			draw_line_clip(316,225,308,225,r,g,b);
			draw_line_clip(308,225,308,212,r,g,b);

			// M ?2
			draw_line_clip(310,228,316,228,r,g,b);
			draw_line_clip(316,228,316,225,r,g,b);
			draw_line_clip(316,225,321,225,r,g,b);
			draw_line_clip(321,225,321,230,r,g,b);
			draw_line_clip(321,230,318,230,r,g,b);
			draw_line_clip(318,230,318,233,r,g,b);
			draw_line_clip(318,233,321,233,r,g,b);
			draw_line_clip(321,233,321,236,r,g,b);
			draw_line_clip(321,236,316,236,r,g,b);
			draw_line_clip(316,236,316,234,r,g,b);
			draw_line_clip(316,234,310,233,r,g,b);
			draw_line_clip(310,233,310,228,r,g,b);

			// Area parkir
			draw_line_clip(268,200,297,200,r,g,b);
			draw_line_clip(297,200,297,225,r,g,b);
			draw_line_clip(297,225,268,225,r,g,b);
			draw_line_clip(268,225,268,225,r,g,b);

			// AP
			draw_line_clip(268,283,277,183,r,g,b);
			draw_line_clip(277,183,277,177,r,g,b);
			draw_line_clip(277,177,281,177,r,g,b);
			draw_line_clip(281,177,281,182,r,g,b);
			draw_line_clip(281,182,284,183,r,g,b);
			draw_line_clip(284,183,284,195,r,g,b);
			draw_line_clip(284,195,267,196,r,g,b);
			draw_line_clip(267,196,268,283,r,g,b);

			//Kelompok 5
			//Kelompok 6
			// CC Barat
			draw_line_clip(143,240,174,240,r,g,b);
			draw_line_clip(174,240,143,251,r,g,b);
			draw_line_clip(143,251,174,251,r,g,b);
			draw_line_clip(174,251,143,240,r,g,b);

			// Lap basket
			draw_line_clip(146,256,170,256,r,g,b);
			draw_line_clip(170,256,146,287,r,g,b);
			draw_line_clip(146,287,170,287,r,g,b);
			draw_line_clip(170,287,146,256,r,g,b);

			// Gedung ??
			draw_line_clip(147,288,147,304,r,g,b);
			draw_line_clip(147,304,127,304,r,g,b);
			draw_line_clip(127,304,127,288,r,g,b);
			draw_line_clip(127,288,147,288,r,g,b);

			//Kelompok 7
			//Teknik Sipil
			draw_line_clip(62,272,86,272,r,g,b);
			draw_line_clip(86,272,86,281,r,g,b);
			draw_line_clip(86,281,74,281,r,g,b);
			draw_line_clip(74,281,74,285,r,g,b);
			draw_line_clip(74,285,111,285,r,g,b);
			draw_line_clip(111,285,111,280,r,g,b);
			draw_line_clip(111,280,125,280,r,g,b);
			draw_line_clip(125,280,125,308,r,g,b);
			draw_line_clip(125,308,70,308,r,g,b);
			draw_line_clip(70,308,70,299,r,g,b);
			draw_line_clip(70,299,98,299,r,g,b);
			draw_line_clip(98,299,98,294,r,g,b);
			draw_line_clip(98,294,62,294,r,g,b);
			draw_line_clip(62,294,62,272,r,g,b);

			//Fisika HIMAFI
			draw_line_clip(60,233,125,233,r,g,b);
			draw_line_clip(125,233,125,277,r,g,b);
			draw_line_clip(125,277,89,277,r,g,b);
			draw_line_clip(89,277,89,269,r,g,b);
			draw_line_clip(89,269,118,269,r,g,b);
			draw_line_clip(118,269,118,266,r,g,b);
			draw_line_clip(118,266,60,266,r,g,b);
			draw_line_clip(60,266,60,233,r,g,b);

			//Kelompok 8
			//Kelompok 9
			//Kelompok 13
			// A
			draw_line_clip(30,111,78,111,r,g,b);
			draw_line_clip(78,111,78,138,r,g,b);
			draw_line_clip(78,138,30,138,r,g,b);
			draw_line_clip(30,138,30,111,r,g,b);

			// B
			draw_line_clip(30,140,55,140,r,g,b);
			draw_line_clip(55,140,55,153,r,g,b);
			draw_line_clip(55,153,30,153,r,g,b);
			draw_line_clip(30,153,30,140,r,g,b);

			// C
			draw_line_clip(30,155,55,155,r,g,b);
			draw_line_clip(55,155,55,158,r,g,b);
			draw_line_clip(55,158,65,158,r,g,b);
			draw_line_clip(65,158,65,153,r,g,b);
			draw_line_clip(65,153,75,153,r,g,b);
			draw_line_clip(75,153,75,169,r,g,b);
			draw_line_clip(75,169,65,169,r,g,b);
			draw_line_clip(65,169,65,166,r,g,b);
			draw_line_clip(65,166,55,166,r,g,b);
			draw_line_clip(55,166,55,169,r,g,b);
			draw_line_clip(55,169,30,169,r,g,b);
			draw_line_clip(30,169,30,155,r,g,b);

			// D
			draw_line_clip(46,94,71,94,r,g,b);
			draw_line_clip(71,94,71,99,r,g,b);
			draw_line_clip(71,99,73,99,r,g,b);
			draw_line_clip(73,99,73,103,r,g,b);
			draw_line_clip(73,103,71,103,r,g,b);
			draw_line_clip(71,103,71,108,r,g,b);
			draw_line_clip(71,108,47,108,r,g,b);
			draw_line_clip(47,108,46,99,r,g,b);
			draw_line_clip(46,99,33,99,r,g,b);
			draw_line_clip(33,99,33,97,r,g,b);
			draw_line_clip(33,97,46,94,r,g,b);

			// Kelompok 14
			// 1
			draw_line_clip(52,80,77,80,r,g,b);
			draw_line_clip(77,80,77,89,r,g,b);
			draw_line_clip(77,89,53,89,r,g,b);
			draw_line_clip(53,89,52,80,r,g,b);
			// 2
			draw_line_clip(49,72,53,72,r,g,b);
			draw_line_clip(53,72,52,89,r,g,b);
			draw_line_clip(52,89,49,89,r,g,b);
			draw_line_clip(49,89,49,72,r,g,b);
			// 3
			draw_line_clip(34,75,48,75,r,g,b);
			draw_line_clip(48,75,48,85,r,g,b);
			draw_line_clip(48,85,34,85,r,g,b);
			draw_line_clip(34,85,34,75,r,g,b);
			//4
			draw_line_clip(39,62,85,62,r,g,b);
			draw_line_clip(85,62,85,71,r,g,b);
			draw_line_clip(85,71,39,71,r,g,b);
			draw_line_clip(39,71,39,62,r,g,b);
			//5
			draw_line_clip(49,52,97,52,r,g,b);
			draw_line_clip(97,52,97,59,r,g,b);
			draw_line_clip(97,59,49,59,r,g,b);
			draw_line_clip(49,59,49,52,r,g,b);

			//6
			draw_line_clip(48,37,97,37,r,g,b);
			draw_line_clip(97,37,97,46,r,g,b);
			draw_line_clip(97,46,48,46,r,g,b);
			draw_line_clip(48,46,48,37,r,g,b);
			// 7
			draw_line_clip(80,23,95,23,r,g,b);
			draw_line_clip(95,23,91,37,r,g,b);
			draw_line_clip(91,37,84,37,r,g,b);
			draw_line_clip(84,37,80,23,r,g,b);

			//Kelompok 16
			// GSG
			draw_line_clip(113,46,142,46,r,g,b);
			draw_line_clip(142,46,142,74,r,g,b);
			draw_line_clip(142,74,113,74,r,g,b);
			draw_line_clip(113,74,113,46,r,g,b);

			// KPP
			draw_line_clip(140,31,163,31,r,g,b);
			draw_line_clip(163,31,169,37,r,g,b);
			draw_line_clip(169,37,169,75,r,g,b);
			draw_line_clip(169,75,145,75,r,g,b);
			draw_line_clip(145,75,145,42,r,g,b);
			draw_line_clip(145,42,140,42,r,g,b);
			draw_line_clip(140,42,140,31,r,g,b);

			//Tunnel
			draw_line_clip(170,35,174,25,r,g,b);
			draw_line_clip(174,25,175,33,r,g,b);
			draw_line_clip(175,33,185,33,r,g,b);
			draw_line_clip(185,33,186,35,r,g,b);
			draw_line_clip(186,35,190,33,r,g,b);
			draw_line_clip(190,33,190,78,r,g,b);
			draw_line_clip(190,78,186,78,r,g,b);
			draw_line_clip(186,78,185,80,r,g,b);
			draw_line_clip(185,80,175,80,r,g,b);
			draw_line_clip(175,80,174,78,r,g,b);
			draw_line_clip(174,78,170,78,r,g,b);
			draw_line_clip(170,78,170,35,r,g,b);

			//Perpus
			draw_line_clip(192,49,194,49,r,g,b);
			draw_line_clip(194,49,194,40,r,g,b);
			draw_line_clip(194,40,200,35,r,g,b);
			draw_line_clip(200,35,216,35,r,g,b);
			draw_line_clip(216,35,221,40,r,g,b);
			draw_line_clip(221,40,221,49,r,g,b);
			draw_line_clip(221,49,227,51,r,g,b);
			draw_line_clip(227,51,227,59,r,g,b);
			draw_line_clip(227,59,225,29,r,g,b);
			draw_line_clip(225,29,225,68,r,g,b);
			draw_line_clip(225,68,218,74,r,g,b);
			draw_line_clip(218,74,201,74,r,g,b);
			draw_line_clip(201,74,197,68,r,g,b);
			draw_line_clip(197,68,197,58,r,g,b);
			draw_line_clip(197,58,192,57,r,g,b);
			draw_line_clip(192,57,192,49,r,g,b);

			// AP
			draw_line_clip(231,33,263,33,r,g,b);
			draw_line_clip(263,33,263,71,r,g,b);
			draw_line_clip(263,71,258,71,r,g,b);
			draw_line_clip(258,71,258,77,r,g,b);
			draw_line_clip(258,77,231,77,r,g,b);
			draw_line_clip(231,77,231,33,r,g,b);

			//Gedung P2T
			//Halaman
			draw_line_clip(267,12,267,46,r,g,b);
			draw_line_clip(267,46,307,49,r,g,b);
			draw_line_clip(307,49,307,12,r,g,b);
			draw_line_clip(307,12,267,12,r,g,b);

			///Gedung kiri atas
			draw_line_clip(271,19,271,23,r,g,b);
			draw_line_clip(271,23,279,23,r,g,b);
			draw_line_clip(279,23,279,19,r,g,b);
			draw_line_clip(279,19,271,19,r,g,b);

			//Gedung kiri bawah
			draw_line_clip(270,26,280,26,r,g,b);
			draw_line_clip(280,26,280,45,r,g,b);
			draw_line_clip(280,45,270,44,r,g,b);
			draw_line_clip(270,44,270,26,r,g,b);

			//Gedung kanan atas
			draw_line_clip(283,13,302,13,r,g,b);
			draw_line_clip(302,13,302,24,r,g,b);
			draw_line_clip(302,24,283,23,r,g,b);
			draw_line_clip(283,23,283,13,r,g,b);

			//Gedung kanan bawah
			draw_line_clip(285,25,298,26,r,g,b);
			draw_line_clip(298,26,298,46,r,g,b);
			draw_line_clip(298,46,284,45,r,g,b);
			draw_line_clip(284,45,285,25,r,g,b);

			//Labtek 4
			// Halaman kiri
			draw_line_clip(267,85,277,68,r,g,b);
			draw_line_clip(277,68,277,76,r,g,b);
			draw_line_clip(277,76,289,69,r,g,b);
			draw_line_clip(289,69,289,67,r,g,b);
			draw_line_clip(289,67,320,69,r,g,b);
			draw_line_clip(320,69,319,75,r,g,b);
			draw_line_clip(319,75,305,75,r,g,b);
			draw_line_clip(305,75,305,73,r,g,b);
			draw_line_clip(305,73,293,73,r,g,b);
			draw_line_clip(293,73,292,82,r,g,b);
			draw_line_clip(292,82,278,82,r,g,b);
			draw_line_clip(278,82,267,91,r,g,b);
			draw_line_clip(267,91,267,85,r,g,b);

			//Gedung kecil belakang
			draw_line_clip(330,71,330,76,r,g,b);
			draw_line_clip(330,76,339,76,r,g,b);
			draw_line_clip(339,76,339,71,r,g,b);
			draw_line_clip(339,71,330,71,r,g,b);

			//Kimia
			//Gedung bawah
			draw_line_clip(326,154,326,141,r,g,b);
			draw_line_clip(326,141,273,136,r,g,b);
			draw_line_clip(273,136,272,149,r,g,b);
			draw_line_clip(272,149,326,154,r,g,b);

			//Gedung atas
			draw_line_clip(275,114,274,119,r,g,b);
			draw_line_clip(274,119,321,134,r,g,b);
			draw_line_clip(321,134,322,118,r,g,b);
			draw_line_clip(322,118,275,114,r,g,b);

			//BscB
			//Gedung Panjang
			draw_line_clip(274,102,274,109,r,g,b);
			draw_line_clip(274,109,334,114,r,g,b);
			draw_line_clip(334,114,337,84,r,g,b);
			draw_line_clip(337,84,330,83,r,g,b);
			draw_line_clip(330,83,328,106,r,g,b);
			draw_line_clip(328,106,274,102,r,g,b);

			//Gedung dalam
			//yang besar
			draw_line_clip(309,94,309,104,r,g,b);
			draw_line_clip(309,104,329,96,r,g,b);
			draw_line_clip(329,96,328,106,r,g,b);
			draw_line_clip(328,106,309,94,r,g,b);

			//yang kecil
			draw_line_clip(303,95,303,102,r,g,b);
			draw_line_clip(303,102,309,102,r,g,b);
			draw_line_clip(309,102,309,96,r,g,b);
			draw_line_clip(309,96,303,95,r,g,b);

			//FTTM
			draw_line_clip(291,98,303,99,r,g,b);
			draw_line_clip(303,99,305,73,r,g,b);
			draw_line_clip(305,73,293,72,r,g,b);
			draw_line_clip(293,72,291,98,r,g,b);

			//Sebelah FTTM
			//Horizontal
			draw_line_clip(305,75,305,78,r,g,b);
			draw_line_clip(305,78,319,79,r,g,b);
			draw_line_clip(319,79,319,76,r,g,b);
			draw_line_clip(319,76,305,75,r,g,b);

			//vertikal
			draw_line_clip(320,70,318,95,r,g,b);
			draw_line_clip(318,95,324,70,r,g,b);
			draw_line_clip(324,70,322,95,r,g,b);
			draw_line_clip(322,95,320,70,r,g,b);

			//kelompok 5
			draw_line_clip(227,301,227,290,r,g,b);
			draw_line_clip(227,290,213,290,r,g,b);
			draw_line_clip(213,290,213,301,r,g,b);
			draw_line_clip(213,301,227,301,r,g,b);

			draw_line_clip(187,285,219,285,r,g,b);
			draw_line_clip(219,285,219,258,r,g,b);
			draw_line_clip(219,258,187,258,r,g,b);
			draw_line_clip(187,258,187,285,r,g,b);

			draw_line_clip(180,230,234,230,r,g,b);
			draw_line_clip(234,230,234,234,r,g,b);
			draw_line_clip(234,234,184,234,r,g,b);
			draw_line_clip(184,234,184,288,r,g,b);
			draw_line_clip(184,288,228,288,r,g,b);
			draw_line_clip(228,288,228,275,r,g,b);
			draw_line_clip(228,275,226,275,r,g,b);
			draw_line_clip(226,275,226,260,r,g,b);
			draw_line_clip(226,260,228,260,r,g,b);
			draw_line_clip(228,260,228,258,r,g,b);
			draw_line_clip(228,258,231,258,r,g,b);
			draw_line_clip(231,258,180,230,r,g,b);

			//kelompok 8
			draw_line_clip(40,282,51,282,r,g,b);
			draw_line_clip(51,282,51,308,r,g,b);
			draw_line_clip(51,308,40,308,r,g,b);
			draw_line_clip(40,308,40,282,r,g,b);

			draw_line_clip(54,277,27,277,r,g,b);
			draw_line_clip(27,277,27,270,r,g,b);
			draw_line_clip(27,270,54,270,r,g,b);
			draw_line_clip(54,270,54,277,r,g,b);

			draw_line_clip(31,277,31,280,r,g,b);
			draw_line_clip(31,280,28,280,r,g,b);
			draw_line_clip(28,280,28,277,r,g,b);
			draw_line_clip(28,277,31,277,r,g,b);

			draw_line_clip(26,259,26,249,r,g,b);
			draw_line_clip(26,249,56,249,r,g,b);
			draw_line_clip(56,249,56,259,r,g,b);
			draw_line_clip(56,259,26,259,r,g,b);

			draw_line_clip(33,249,33,235,r,g,b);
			draw_line_clip(33,235,56,235,r,g,b);
			draw_line_clip(56,235,56,249,r,g,b);
			draw_line_clip(56,249,33,249,r,g,b);

			draw_line_clip(36,259,30,265,r,g,b);
			draw_line_clip(30,265,36,269,r,g,b);
			draw_line_clip(36,269,43,265,r,g,b);
			draw_line_clip(43,265,36,259,r,g,b);

			//kelompok 10
			draw_line_clip(105,217,105,200,r,g,b);
			draw_line_clip(105,200,174,200,r,g,b);
			draw_line_clip(174,200,174,217,r,g,b);
			draw_line_clip(174,217,105,217,r,g,b);

			draw_line_clip(185,217,185,200,r,g,b);
			draw_line_clip(185,200,255,200,r,g,b);
			draw_line_clip(255,200,255,217,r,g,b);
			draw_line_clip(255,217,185,217,r,g,b);

			draw_line_clip(105,190,105,173,r,g,b);
			draw_line_clip(105,173,174,173,r,g,b);
			draw_line_clip(174,173,174,190,r,g,b);
			draw_line_clip(174,190,105,190,r,g,b);

			draw_line_clip(185,190,185,173,r,g,b);
			draw_line_clip(185,173,255,173,r,g,b);
			draw_line_clip(255,173,255,190,r,g,b);
			draw_line_clip(255,190,185,190,r,g,b);

			//kelompok 12
			draw_line_clip(88,94,124,94,r,g,b);
			draw_line_clip(124,94,121,114,r,g,b);
			draw_line_clip(121,114,88,114,r,g,b);
			draw_line_clip(88,114,88,94,r,g,b);

			draw_line_clip(88,120,124,120,r,g,b);
			draw_line_clip(124,120,124,140,r,g,b);
			draw_line_clip(124,140,88,140,r,g,b);
			draw_line_clip(88,140,88,120,r,g,b);

			draw_line_clip(88,140,88,149,r,g,b);
			draw_line_clip(88,149,95,149,r,g,b);
			draw_line_clip(95,149,95,140,r,g,b);
			draw_line_clip(95,140,88,140,r,g,b);

			draw_line_clip(104,140,104,149,r,g,b);
			draw_line_clip(104,149,111,149,r,g,b);
			draw_line_clip(111,149,111,140,r,g,b);
			draw_line_clip(111,140,104,140,r,g,b);

			//kelompok 11
			//oktagon
			draw_line_clip(157,88,143,101,r,g,b);
			draw_line_clip(143,101,157,115,r,g,b);
			draw_line_clip(157,115,173,101,r,g,b);
			draw_line_clip(173,101,157,88,r,g,b);

			//tvst
			draw_line_clip(157,120,143,133,r,g,b);
			draw_line_clip(143,133,157,147,r,g,b);
			draw_line_clip(157,147,173,133,r,g,b);
			draw_line_clip(173,133,157,120,r,g,b);

			//Lab Fidas
			draw_line_clip(189,105,189,97,r,g,b);
			draw_line_clip(189,97,223,97,r,g,b);
			draw_line_clip(223,97,223,113,r,g,b);
			draw_line_clip(223,113,196,113,r,g,b);
			draw_line_clip(196,113,189,105,r,g,b);

			//Lab
			draw_line_clip(196,122,223,122,r,g,b);
			draw_line_clip(223,122,223,138,r,g,b);
			draw_line_clip(223,138,191,135,r,g,b);
			draw_line_clip(191,135,188,133,r,g,b);
			draw_line_clip(188,133,196,122,r,g,b);

			draw_line_clip(226,94,249,94,r,g,b);
			draw_line_clip(249,94,249,139,r,g,b);
			draw_line_clip(249,139,226,139,r,g,b);
			draw_line_clip(226,139,226,94,r,g,b);


			draw_line_clip(226,139,249,139,r,g,b);
			draw_line_clip(249,139,249,158,r,g,b);
			draw_line_clip(249,158,226,158,r,g,b);
			draw_line_clip(226,158,226,139,r,g,b);

			//GKU BARAT
			draw_line_clip(64,177,75,176,r,g,b);
			draw_line_clip(75,176,82,184,r,g,b);
			draw_line_clip(82,184,82,195,r,g,b);
			draw_line_clip(82,195,74,202,r,g,b);
			draw_line_clip(74,202,64,203,r,g,b);
			draw_line_clip(64,203,55,195,r,g,b);
			draw_line_clip(55,195,55,184,r,g,b);
			draw_line_clip(55,184,64,177,r,g,b);
		}

		void draw_street_clip(int r, int g, int b){
			//Jl.Taman Sari - Ganeca
			draw_line_clip(45,404,42,414,r,g,b);
			draw_line_clip(45,404,8,280,r,g,b);
			draw_line_clip(45,404,151,376,r,g,b);
			draw_line_clip(151,376,148,414,r,g,b);
			draw_line_clip(151,376,181,372,r,g,b);
			draw_line_clip(181,372,287,402,r,g,b);
			draw_line_clip(287,402,287,414,r,g,b);
			draw_line_clip(287,402,356,402,r,g,b);

			//itb kanan
			draw_line_clip(306,231,308,297,r,g,b);
			draw_line_clip(308,297,292,300,r,g,b);
			draw_line_clip(292,300,291,335,r,g,b);
			draw_line_clip(291,335,287,402,r,g,b);
			draw_line_clip(266,231,266,12,r,g,b);
			draw_line_clip(266,50,341,56,r,g,b);
			draw_line_clip(266,111,326,116,r,g,b);
			draw_line_clip(266,132,326,137,r,g,b);


			//itb tengah
			draw_line_clip(181,372,181,335,r,g,b);
			draw_line_clip(181,335,71,335,r,g,b);
			draw_line_clip(181,335,181,231,r,g,b);
			draw_line_clip(181,231,58,231,r,g,b);
			draw_line_clip(58,231,23,231,r,g,b);
			draw_line_clip(181,231,306,231,r,g,b);
			draw_line_clip(181,335,291,335,r,g,b);
			draw_line_clip(181,231,181,160,r,g,b);
			draw_line_clip(181,160,181,83,r,g,b);
			draw_line_clip(181,29,181,16,r,g,b);
			draw_line_clip(80,16,181,16,r,g,b);
			draw_line_clip(181,16,219,1,r,g,b);


			//itb kiri
			draw_line_clip(23,231,23,294,r,g,b);
			draw_line_clip(23,294,44,330,r,g,b);
			draw_line_clip(44,330,71,335,r,g,b);
			draw_line_clip(58,231,58,315,r,g,b);
			draw_line_clip(58,315,71,335,r,g,b);
			draw_line_clip(23,231,25,173,r,g,b);
			draw_line_clip(25,173,82,173,r,g,b);
			draw_line_clip(82,173,95,173,r,g,b);
			draw_line_clip(95,173,95,199,r,g,b);
			draw_line_clip(95,199,102,202,r,g,b);
			draw_line_clip(102,202,103,231,r,g,b);

			draw_line_clip(106,83,107,28,r,g,b);
			draw_line_clip(107,28,266,33,r,g,b);
			draw_line_clip(82,173,82,83,r,g,b);
			draw_line_clip(82,83,266,83,r,g,b);
			draw_line_clip(82,118,266,118,r,g,b);
			draw_line_clip(82,160,266,160,r,g,b);
			draw_line_clip(25,173,28,46,r,g,b);
			draw_line_clip(28,46,51,28,r,g,b);
			draw_line_clip(51,28,80,28,r,g,b);
			draw_line_clip(80,28,80,16,r,g,b);
			draw_line_clip(80,16,48,16,r,g,b);
			draw_line_clip(48,16,21,50,r,g,b);
			draw_line_clip(21,50,8,280,r,g,b);
		}

		void pohon(int x, int y,int r, int g, int b) {
			draw_line_clip(x,y-6,x-3,y,r,g,b);
			draw_line_clip(x,y-6,x+3,y,r,g,b);
			draw_line_clip(x-3,y,x+3,y,r,g,b);
		}

		void draw_pohon_clip(int r, int g, int b) {
			pohon(125,241,r,g,b);
			pohon(128,241,r,g,b);
			pohon(128,234,r,g,b);
			pohon(173,234,r,g,b);
			pohon(173,234,r,g,b);
			pohon(175,235,r,g,b);
			pohon(175,287,r,g,b);
			pohon(171,287,r,g,b);
			pohon(55,387,r,g,b);
			pohon(61,387,r,g,b);
			pohon(67,387,r,g,b);
			pohon(73,387,r,g,b);
			pohon(79,387,r,g,b);
			pohon(55,383,r,g,b);
			pohon(61,383,r,g,b);
			pohon(67,383,r,g,b);
			pohon(73,383,r,g,b);
			pohon(79,383,r,g,b);
			pohon(81,383,r,g,b);
			pohon(82,383,r,g,b);
			pohon(85,383,r,g,b);
			pohon(56,375,r,g,b);
			pohon(63,375,r,g,b);
			pohon(69,375,r,g,b);
			pohon(78,375,r,g,b);
			pohon(55,363,r,g,b);
			pohon(57,375,r,g,b);
			pohon(257,375,r,g,b);
			pohon(259,373,r,g,b);
			pohon(260,379,r,g,b);
			pohon(263,367,r,g,b);
			pohon(267,365,r,g,b);
			pohon(250,350,r,g,b);
			pohon(252,342,r,g,b);
			pohon(261,351,r,g,b);
			pohon(242,354,r,g,b);
			pohon(253,343,r,g,b);
			pohon(269,333,r,g,b);
			pohon(237,340,r,g,b);
			pohon(249,345,r,g,b);
			pohon(244,347,r,g,b);
			pohon(232,352,r,g,b);
			pohon(230,349,r,g,b);
			pohon(236,337,r,g,b);
			pohon(235,338,r,g,b);
			pohon(239,342,r,g,b);
			pohon(220,350,r,g,b);
			pohon(223,360,r,g,b);
			pohon(227,347,r,g,b);
			pohon(231,362,r,g,b);
			pohon(220,361,r,g,b);
		}


		void draw_minimap(int x, int y, int xsize, int ysize) {
			int oldvxsize = vxsize;
			int oldvysize = vysize;
			int oldvxoffset = vxoffset;
			int oldvyoffset = vyoffset;

			int oldwxsize = wxsize;
			int oldwysize = wysize;
			int oldwxoffset = wxoffset;
			int oldwyoffset = wyoffset;

			vxsize = xsize;
			vysize = ysize;
			vxoffset = x;
			vyoffset = y;
			wxsize = IMG_X_SIZE;
			wysize = IMG_Y_SIZE;
			wxoffset = 0;
			wyoffset = 0;
			draw_view();
			draw_window(oldwxoffset, oldwyoffset, oldwxsize, oldwysize);
			draw_street_clip(255,255,255);
			draw_bangunan_clip(200,200,200);
			draw_pohon_clip(0,100,0);

			wxsize = oldwxsize;
			wysize = oldwysize;
			vxsize = oldvxsize;
			vysize = oldvysize;

			wxoffset = oldwxoffset;
			wyoffset = oldwyoffset;
			vxoffset = oldvxoffset;
			vyoffset = oldvyoffset;
		}

		void draw_window(int wxoffset, int wyoffset, int wxsize, int wysize) {
			draw_line_clip(wxoffset, wyoffset, wxoffset + wxsize, wyoffset, 0, 100, 0);
			draw_line_clip(wxoffset+wxsize, wyoffset, wxoffset+wxsize, wyoffset+wysize, 0,100,0);
			draw_line_clip(wxoffset+wxsize, wyoffset+wysize, wxoffset, wyoffset+wysize, 0,100,0);
			draw_line_clip(wxoffset, wyoffset+wysize, wxoffset ,wyoffset, 0,100,0);
		}

		void erase_minimap(int x, int y, int xsize, int ysize) {
			int oldvxsize = vxsize;
			int oldvysize = vysize;
			int oldvxoffset = vxoffset;
			int oldvyoffset = vyoffset;

			int oldwxsize = wxsize;
			int oldwysize = wysize;
			int oldwxoffset = wxoffset;
			int oldwyoffset = wyoffset;

			vxsize = xsize;
			vysize = ysize;
			vxoffset = x;
			vyoffset = y;
			wxsize = IMG_X_SIZE;
			wysize = IMG_Y_SIZE;
			wxoffset = 0;
			wyoffset = 0;
			draw_view();
			erase_window(oldwxoffset, oldwyoffset, oldwxsize, oldwysize);
			draw_street_clip(0,0,0);
			draw_bangunan_clip(0,0,0);
			draw_pohon_clip(0,0,0);
			wxsize = oldwxsize;
			wysize = oldwysize;
			vxsize = oldvxsize;
			vysize = oldvysize;

			wxoffset = oldwxoffset;
			wyoffset = oldwyoffset;
			vxoffset = oldvxoffset;
			vyoffset = oldvyoffset;
		}

		void erase_window(int wxoffset, int wyoffset, int wxsize, int wysize) {
			draw_line_clip(wxoffset, wyoffset, wxoffset + wxsize, wyoffset, 0, 0, 0);
			draw_line_clip(wxoffset+wxsize, wyoffset, wxoffset+wxsize, wyoffset+wysize, 0,0,0);
			draw_line_clip(wxoffset+wxsize, wyoffset+wysize, wxoffset, wyoffset+wysize, 0,0,0);
			draw_line_clip(wxoffset, wyoffset+wysize, wxoffset ,wyoffset, 0,0,0);
		}
};



#endif
