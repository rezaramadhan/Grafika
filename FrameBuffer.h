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
using namespace std;

class FrameBuffer {

	private:
		int fbfd;
	    struct fb_var_screeninfo vinfo;
	    struct fb_fix_screeninfo finfo;
	    long int screensize;
	    char *fbp;

	public:
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

		void drawBan1(int xc, int yc, int radius, int r, int g, int b) {
			draw_circle(xc,yc,radius,r,g,b);
			flood_fill(xc,yc,100,100,0);
			draw_line(xc-radius,yc,xc+radius,yc,r,g,b);
		}

		void drawBan(int xc, int yc, int xl1, int yl1, int xl2, int yl2, int radius, int r, int g, int b) {
			draw_circle(xc,yc,radius,r,g,b);
			flood_fill(xc,yc,0,100,100);
			draw_line(xl1,yl1,xl2,yl2,r,g,b);
			//draw_line(xc-radius,yc,xc+radius,yc,r,g,b);
			//draw_line(xc,yc-radius,xc,yc+radius,r,g,b);
		}
		
		void drawsayap(int x, int y,int r,int g,int b) {
			draw_line(x,y,x+44,y-46,0,0,150);
			draw_line(x+44,y-46,x+53,y-46,0,0,150);
			draw_line(x+53,y-46,x+29,y-10,0,0,150);
			draw_line(x+29,y-10,x+31,y,0,0,150);
			draw_line(x+31,y,x,y,0,0,150);
			flood_fill(x+15,y-5,r,g,b);
		}
};



#endif
