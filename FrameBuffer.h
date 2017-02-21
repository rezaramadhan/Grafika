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
			draw_line(vxoffset, vyoffset, vxoffset + vxsize, vyoffset, 55, 55, 55);
			draw_line(vxoffset+vxsize, vyoffset, vxoffset+vxsize, vyoffset+vysize, 55,55,55);
			draw_line(vxoffset+vxsize, vyoffset+vysize, vxoffset, vyoffset+vysize, 55,55,55);
			draw_line(vxoffset, vyoffset+vysize, vxoffset ,vyoffset ,55,55,55);
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

				printf("awal %d %d %d %d\n", xt1, yt1, xt2, yt2);
				remap_point(&xt1, &yt1);
				remap_point(&xt2, &yt2);
				printf("remap %d %d %d %d\n", xt1, yt1, xt2, yt2);
				draw_line(xt1, yt1, xt2, yt2, r, g, b);
			}
		}
		
		void flood_fill_clip(int x, int y, int r, int g, int b) {
			remap_point(&x,&y);
			flood_fill(x,y,r,g,b);
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


	
>>>>>>> 8fad3650017cb4c28d235e6973c535b834147430
};



#endif
