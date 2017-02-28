#include <pthread.h>
#include <signal.h>

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <queue>
  
using namespace std; 
typedef struct{
	queue<int> q;
	pthread_mutex_t mutex;
}my_queue;

my_queue _key_pressed_queue;
pthread_t thread_handle_keypress;
pthread_t thread_handle_response;

typedef struct{
	pthread_cond_t cond;
	pthread_mutex_t mtx;
}alarm;
alarm _response_alarm;
alarm _time_to_exit;

sigset_t _sign_response;
bool flag_stop;

void init_alarm(alarm* _a){
	pthread_mutex_init(&(_a->mtx), NULL);
	pthread_cond_init(&(_a->cond), NULL);
}

void destroy_alarm(alarm *_a){
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
		pthread_cond_broadcast(&_response_alarm.cond);
	}
	pthread_exit(NULL);
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
		toggle_table[i] = false;
	}
	while(!flag_stop){
		//Tunggu dibangunin
		pthread_mutex_lock(&_response_alarm.mtx);
		pthread_cond_wait(&_response_alarm.cond, &_response_alarm.mtx);
		pthread_mutex_unlock(&_response_alarm.mtx);
		char c = _key_pressed_queue.q.front();
		switch(c){
			//TODO: ganti kode yang dilabeli "stub" dengan kode buat 
			//ngerender peta dengan opsi-opsi:
			//g : gedung, j : jalan, p : pohon
			//Tapi silakan diganti-ganti kalo kurang sesuai
			 
			//gedung
			case 'g' : {
				toggle_table[GEDUNG] = !toggle_table[GEDUNG];
				if(toggle_table[GEDUNG]){
					cout << "GEDUNG ON" << endl; //stub
				}
				else{
					cout << "GEDUNG OFF" << endl; //stub
				}
				break;
			}
			//jalan
			case 'j' : {
				toggle_table[JALAN] = !toggle_table[JALAN];
				if(toggle_table[JALAN]){
					cout << "JALAN ON" << endl; //stub
				}
				else{
					cout << "JALAN OFF" << endl; //stub
				}
				break;
			}
			//pohon
			case 'p' : {
				toggle_table[POHON] = !toggle_table[POHON];
				if(toggle_table[POHON]){
					cout << "POHON ON" << endl; //stub
				}
				else{
					cout << "POHON OFF" << endl; //stub
				}
				break;
			}
			//mungkin ada directional supaya bisa menjelajah map? W, A, S, D
			case 'w' : {
				cout << "w" << endl; //stub
				break;
			}
			case 'a' : {
				cout << "a" << endl; //stub
				break;
			}
			case 's' : {
				cout << "s" << endl; //stub
				break;
			}
			case 'd' : {
				cout << "s" << endl; //stub
				break;
			}
			default: {
				cout << c << endl;
			}
		}
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
	init_alarm(&_response_alarm);
	init_alarm(&_time_to_exit);
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
	//Menunggu user pencet ctrl-c, (kena SIGINT) lalu beres-beres
	pthread_mutex_lock(&_time_to_exit.mtx);
	pthread_cond_wait(&_time_to_exit.cond, &_time_to_exit.mtx);
	pthread_mutex_unlock(&_time_to_exit.mtx);
	
	cout << "Press any key..." << endl;
	pthread_join(thread_handle_keypress, NULL);
	pthread_join(thread_handle_response, NULL);
	destroy_alarm(&_response_alarm);
	destroy_alarm(&_time_to_exit);
	pthread_mutex_destroy(&_key_pressed_queue.mutex);
	
	return EXIT_SUCCESS;
}
