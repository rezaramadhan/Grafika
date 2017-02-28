#include <pthread.h>
#include <signal.h>

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <queue>

#include "FrameBuffer.h"

using namespace std;

FrameBuffer *fb;

typedef struct {
	queue<int> q;
	pthread_mutex_t mutex;
} my_queue;

my_queue _key_pressed_queue;
pthread_t thread_handle_keypress;
pthread_t thread_handle_response;

typedef struct{
	pthread_cond_t cond;
	pthread_mutex_t mtx;
} my_alarm;
my_alarm _response_my_alarm;
my_alarm _time_to_exit;

sigset_t _sign_response;
bool flag_stop;

void init_my_alarm(my_alarm* _a){
	pthread_mutex_init(&(_a->mtx), NULL);
	pthread_cond_init(&(_a->cond), NULL);
}

void destroy_my_alarm(my_alarm *_a){
	pthread_mutex_destroy(&(_a->mtx));
	pthread_cond_destroy(&(_a->cond));
}

/**
 * Handler sinyal untuk SIGINT yg disebabkan user
 * Misalnya untuk
 */
void handle_user_interrupt(int signum){
	if(signum == SIGINT){
		cout << "Alamakjang diganggu!!" << endl;
		flag_stop = true;
		pthread_cond_broadcast(&_time_to_exit.cond);
	}
}

/**
 * Thread yang "mendengarkan" input user dari keyboard
 */
void* handle_keypress(void* arg){
	while(!flag_stop){
		//Minta user input, masukkin ke queue, lalu "ganggu" thread
		//yang ngehandle respon/rendering.
		int _in = getchar();
		int dump = getchar();
		pthread_mutex_lock(&_key_pressed_queue.mutex);
		_key_pressed_queue.q.push(_in);
		pthread_mutex_unlock(&_key_pressed_queue.mutex);
		pthread_cond_broadcast(&_response_my_alarm.cond);
	}
	pthread_exit(NULL);
}
void redraw(bool *toggle_table) {
	const int GEDUNG = 0;
	const int JALAN = 1;
	const int POHON = 2;
	if (toggle_table[GEDUNG]) {
		fb->draw_bangunan_clip(0,0,255);
	}
	if (toggle_table[JALAN]) {
		fb->draw_street_clip(128,128,128);
	}
	if (toggle_table[POHON]) {
		//nothing yet
	}
	fb->draw_minimap(MINIMAP_X_LOC,MINIMAP_Y_LOC,IMG_X_SIZE / 2, IMG_Y_SIZE / 2);
}
void delete_all(bool *toggle_table) {
	const int GEDUNG = 0;
	const int JALAN = 1;
	const int POHON = 2;
	if (toggle_table[GEDUNG]) {
		fb->draw_bangunan_clip(0,0,0);
	}
	if (toggle_table[JALAN]) {
		fb->draw_street_clip(0,0,0);
	}
	if (toggle_table[POHON]) {
		//nothing yet
	}
	fb->erase_minimap(MINIMAP_X_LOC,MINIMAP_Y_LOC,IMG_X_SIZE / 2, IMG_Y_SIZE / 2);
}

/**
 * Thread yang merespon input user yang diterima dan melakukan
 * segala hal pemanggilan method untuk rendering.
 */
void* handle_response(void* arg){
	//array untuk menyimpan "toggle" untuk masing-masing opsi.
	const int OPTION_COUNTS = 3;
	bool toggle_table[OPTION_COUNTS];
	const int GEDUNG = 0;
	const int JALAN = 1;
	const int POHON = 2;
	int i;
	for(i = 0; i < OPTION_COUNTS; i++){
		toggle_table[i] = true;
	}
	while(!flag_stop){
		//Tunggu dibangunin
		pthread_mutex_lock(&_response_my_alarm.mtx);
		pthread_cond_wait(&_response_my_alarm.cond, &_response_my_alarm.mtx);
		pthread_mutex_unlock(&_response_my_alarm.mtx);
		char c = _key_pressed_queue.q.front();
		delete_all(toggle_table);
		switch(c){

			//gedung
			case 'g' : {
				toggle_table[GEDUNG] = !toggle_table[GEDUNG];
				break;
			}
			//jalan
			case 'j' : {
				toggle_table[JALAN] = !toggle_table[JALAN];
				break;
			}
			//pohon
			case 'p' : {
				toggle_table[POHON] = !toggle_table[POHON];
				break;
			}
			//mungkin ada directional supaya bisa menjelajah map? W, A, S, D
			case 'w' : {
				fb->wyoffset-= 2;
				break;
			}
			case 'a' : {
				fb->wxoffset-= 2;
				break;
			}
			case 's' : {
				fb->wyoffset+= 2;
				break;
			}
			case 'd' : {
				fb->wxoffset+= 2;
				break;
			}
			case 'k' : {
				fb->wxsize -= 4;
				fb->wysize -= 5;
				break;
			}
			case 'l' : {
				fb->wxsize += 4;
				fb->wysize += 5;
				break;
			}

			default: {
				cout << c << endl;
			}
		}
		redraw(toggle_table);
		//buang huruf yang sudah dibaca dari queue
		pthread_mutex_lock(&_key_pressed_queue.mutex);
		_key_pressed_queue.q.pop();
		pthread_mutex_unlock(&_key_pressed_queue.mutex);
	}

	pthread_exit(NULL);
}

/**
 *
 */
int main(){
	//Inisialisasi semua variable
	pthread_mutex_init(&_key_pressed_queue.mutex, NULL);
	init_my_alarm(&_response_my_alarm);
	init_my_alarm(&_time_to_exit);
	if(signal(SIGINT, handle_user_interrupt) == SIG_ERR){
		perror("NYAHAHAHA something went wrong");
		exit(EXIT_FAILURE);
	}
	flag_stop = false;

	//Bikin dan inisialisasi thread.
	int _stat;
	_stat = pthread_create(&thread_handle_keypress, NULL, handle_keypress, NULL);
	if(_stat){
		perror("Walahmakjang error!");
		exit(EXIT_FAILURE);
	}
	_stat = pthread_create(&thread_handle_response, NULL, handle_response, NULL);
	if(_stat){
		perror("Walahmakjang error mulu!");
		exit(EXIT_FAILURE);
	}

	fb = new FrameBuffer;
	fb->wxsize = IMG_X_SIZE;
	fb->wysize = IMG_Y_SIZE;
	fb->wxoffset = 0;
	fb->wyoffset = 0;
	fb->vxsize = IMG_X_SIZE + IMG_X_SIZE / 4;
	fb->vysize = IMG_Y_SIZE + IMG_Y_SIZE / 4;
	fb->vxoffset = 160;
	fb->vyoffset = 90;
	fb->draw_view();
	fb->draw_bangunan_clip(0,0,255);
	fb->draw_street_clip(128,128,128);
	fb->draw_minimap(MINIMAP_X_LOC,MINIMAP_Y_LOC,IMG_X_SIZE / 2, IMG_Y_SIZE / 2);
	// fb->draw_bangunan();
	// fb->draw_street();

	//Menunggu user pencet ctrl-c, (kena SIGINT) lalu beres-beres
	pthread_mutex_lock(&_time_to_exit.mtx);
	pthread_cond_wait(&_time_to_exit.cond, &_time_to_exit.mtx);
	pthread_mutex_unlock(&_time_to_exit.mtx);

	cout << "Press any key..." << endl;
	pthread_join(thread_handle_keypress, NULL);
	pthread_join(thread_handle_response, NULL);
	destroy_my_alarm(&_response_my_alarm);
	destroy_my_alarm(&_time_to_exit);
	pthread_mutex_destroy(&_key_pressed_queue.mutex);

	delete fb;
	return EXIT_SUCCESS;
}
