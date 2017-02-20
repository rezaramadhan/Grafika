#include "FrameBuffer.h"

const char *byte_to_binary(int x) {
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}


int main() {
	// FrameBuffer *fb = new FrameBuffer;

	char ch;
	// system ("/bin/stty raw");

	while ((ch = getchar())) {
		if (ch == 'w') {
			printf("up\n");
		} else if (ch == 'a') {
			printf("left\n");
		} else if (ch == 's') {
			printf("down\n");
		} else if (ch == 'd') {
			printf("right\n");
		} else if (ch == 'i') {
			printf("zoom in\n");
		} else if (ch == 'o') {
			printf("zoom out\n");
		}
	}

	return 0;
}
