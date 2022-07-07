/**
 ******************************************************************************
 * @author: ThanNT
 * @date:   12/01/2017
 * @brief:  timer
 ******************************************************************************
**/
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>

#include "ak/ak.h"
#include "ak/ak_dbg.h"
#include "ak/timer.h"
#include "ak/sys_dbg.h"

/* internal define */
#define CLOCKID				CLOCK_REALTIME
#define SIG					SIGUSR1

#define TIMER_ADD_SIG		0x01
#define AK_TIMER_UNIT 		1000000
uint32_t ak_timer_unit_init = AK_TIMER_UNIT;

/* declare internal struct */
typedef struct {
	uint32_t des_task_id;
	uint32_t sig;

	int32_t period;
	int32_t counter;
} timer_data_t;

typedef struct timer_msg_t{
	struct timer_msg_t* next;
	struct timer_msg_t* prev;
	timer_data_t data;
} timer_msg_t;

typedef struct {
	pthread_mutex_t mt;
	timer_msg_t* head;
	timer_msg_t* tail;
} timer_service_t;

/* external variable module */
q_msg_t timer_mailbox;

/* internal variable */
static timer_service_t timer_service;
static pthread_mutex_t mt_timer_service;

/* internal function */
static uint32_t timer_service_remove_node(uint32_t dst, uint32_t sig);

/* timer stick counter */
static volatile uint32_t timer_stick_counter;

void timer_handler(int sig, siginfo_t *si, void *uc) {
	timer_stick_counter++;
	// AK_PRINT("[DEBUG] tick: %d\n", timer_stick_counter);
	pthread_mutex_lock(&mt_timer_service);

	timer_msg_t* remove_node_mask = NULL;
	timer_msg_t* timer_node_temp = timer_service.tail;

	while (timer_node_temp != NULL) {
		timer_node_temp->data.counter--;
		if (timer_node_temp->data.counter <= 0) {

			task_post_pure_msg_func(AK_TASK_TIMER_ID, timer_node_temp->data.des_task_id, timer_node_temp->data.sig);
			AK_TIMER_DBG("[TIMER][timer_handler] des_task_id:%d sig:%d\n", s_msg->header->des_task_id, s_msg->header->sig);

			if (timer_node_temp->data.period != 0) {
				timer_node_temp->data.counter = timer_node_temp->data.period;
			}
			else {
				remove_node_mask = timer_node_temp;
			}
		}

		/* check next node */
		timer_node_temp = timer_node_temp->next;

		/* remove node */
		if (remove_node_mask != NULL) {
			timer_service_remove_node(remove_node_mask->data.des_task_id, remove_node_mask->data.sig);
			remove_node_mask = NULL;
		}
	}

	pthread_mutex_unlock(&mt_timer_service);
}

void* timer_entry(void) {
	timer_service.head = NULL;
	timer_service.tail = NULL;

	/* configure timer */
	struct sigevent sev;
	struct itimerspec its;
	static timer_t timer_id_1ms;
	sigset_t mask;

   	AK_PRINT("Blocking signal %d\n", SIG);
    sigemptyset(&mask);
    sigaddset(&mask, SIG);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
        perror("sigprocmask");
	AK_PRINT("timer ID is 0x%lx\n", (long) timer_id_1ms);

	/* create the timer */
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timer_id_1ms;
	sev.sigev_notify_attributes	= NULL;
	sev.sigev_notify_function = timer_handler;
	timer_create(CLOCKID, &sev, &timer_id_1ms);

	/* start the timer */
	its.it_value.tv_sec = 0;
	its.it_value.tv_nsec = ak_timer_unit_init; /* timer 1ms */
	its.it_interval.tv_sec = its.it_value.tv_sec;
	its.it_interval.tv_nsec = its.it_value.tv_nsec;
	timer_settime(timer_id_1ms, 0, &its, NULL);

	AK_PRINT("Unblocking signal %d\n", SIG);
	if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
		perror("sigprocmask");

	wait_all_tasks_started();

	return (void*)0;
}

void calibrate_ak_timer_unit(uint32_t ak_timer_unit) {
	ak_timer_unit_init = ak_timer_unit;
}

// void init_timer_system() {

// 	AK_PRINT("init_timer_system\n");

// 	timer_service.head = NULL;
// 	timer_service.tail = NULL;

// 	/* configure timer */
// 	/* configure timer */
// 	struct timeval my_value={1,0};
// 	struct timeval my_interval={1,0};
// 	struct itimerval my_timer={my_interval, my_value};
// 	setitimer(ITIMER_REAL, &my_timer, 0);

// 	signal(SIGALRM,  timer_handler);
// }

// void *timer_entry() {

// 	AK_PRINT("init_timer_system\n");

// 	timer_service.head = NULL;
// 	timer_service.tail = NULL;
//     struct sigaction sa;
// 	memset (&sa, 0, sizeof (sa));
// 	sa.sa_handler = &timer_handler;
// 	sigemptyset(&sa.sa_mask);
// 	sigaction (SIGALRM/*SIGVTALRM*/, &sa, NULL);

// 	/* configure timer */
// 	/* configure timer */
// 	struct timeval my_value={1,0};
// 	struct timeval my_interval={1,0};
// 	struct itimerval my_timer={my_interval, my_value};
// 	setitimer(ITIMER_REAL, &my_timer, 0);

// 	// signal(SIGALRM,  timer_handler);
// 	wait_all_tasks_started();
// }

// void* timer_entry(void) {
// 	timer_service.head = NULL;
// 	timer_service.tail = NULL;

//     timer_t timerid;
//     struct sigevent sev;
//     struct itimerspec its;
//     // long long freq_nanosecs;
//     sigset_t mask;
//     struct sigaction sa;

//    /* Establish handler for timer signal */

//    	AK_PRINT("Establishing handler for signal %d\n", SIG);
//     sa.sa_flags = SA_SIGINFO;
//     sa.sa_sigaction = timer_handler;
//     sigemptyset(&sa.sa_mask);
//     if (sigaction(SIG, &sa, NULL) == -1)
//         perror("sigaction");

//    /* Block timer signal temporarily */

//    	AK_PRINT("Blocking signal %d\n", SIG);
//     sigemptyset(&mask);
//     sigaddset(&mask, SIG);
//     if (sigprocmask(SIG_SETMASK, &mask, NULL) == -1)
//         perror("sigprocmask");
// 	AK_PRINT("timer ID is 0x%lx\n", (long) timerid);

// 	/* create the timer */

// 	sev.sigev_notify = SIGEV_SIGNAL;
// 	sev.sigev_signo = SIG;
// 	sev.sigev_value.sival_ptr = &timerid;
// 	// sev.sigev_notify_attributes	= NULL;
// 	// sev.sigev_notify_function = timer_handler;
// 	// timer_create(CLOCKID, &sev, &timer_id_1ms);
//     if (timer_create(CLOCKID, &sev, &timerid) == -1)
//         perror("timer_create");

//    	AK_PRINT("timer ID is 0x%lx\n", (long) timerid);

// 	/* start the timer */

// 	its.it_value.tv_sec = 1;
// 	its.it_value.tv_nsec = 0; /* timer 1ms */
// 	its.it_interval.tv_sec = its.it_value.tv_sec;
// 	its.it_interval.tv_nsec = its.it_value.tv_nsec;
//    	if (timer_settime(timerid, 0, &its, NULL) == -1)
//         perror("timer_settime");


// 	AK_PRINT("Unblocking signal %d\n", SIG);
// 	if (sigprocmask(SIG_UNBLOCK, &mask, NULL) == -1)
// 		perror("sigprocmask");

// 	wait_all_tasks_started();

// 	return (void*)0;
// }

uint32_t timer_set(uint32_t des_task_id, uint32_t sig, uint32_t duty, timer_type_t timer_type) {
	AK_TIMER_DBG("[TIMER][timer_set] des_task_id:%d sig:%d duty:%d timer_type:%d\n", des_task_id, sig, duty, timer_type);

	/* add node to timer list */
	pthread_mutex_lock(&mt_timer_service);

	timer_msg_t* timer_node_temp = timer_service.tail;

	while (timer_node_temp != NULL) {
		if (timer_node_temp->data.des_task_id == des_task_id &&
				timer_node_temp->data.sig == sig) {

			timer_node_temp->data.counter = duty;
			AK_TIMER_DBG("[TIMER] clear counter\n");

			pthread_mutex_unlock(&mt_timer_service);

			return AK_RET_OK;
		}
		else {
			timer_node_temp = timer_node_temp->next;
		}
	}

	timer_msg_t* timer_new = (timer_msg_t*)malloc(sizeof(timer_msg_t));

	if (timer_new == NULL) {
		FATAL("TIMER", 0x02);
	}

	/* update timer node infomation */
	timer_new->data.des_task_id = des_task_id;
	timer_new->data.sig = sig;
	timer_new->data.counter = duty;

	switch (timer_type) {
	case TIMER_ONE_SHOT:
		timer_new->data.period       = (int32_t)0;
		break;

	case TIMER_PERIODIC:
		timer_new->data.period       = (int32_t)duty;
		break;

	default: {
		FATAL("TIMER", 0x01);
	}
		break;
	}

	if (timer_service.tail != NULL) {
		timer_new->prev = NULL;
		timer_new->next = timer_service.tail;

		timer_service.tail->prev = timer_new;
		timer_service.tail = timer_new;
	}
	else {
		timer_new->next = NULL;
		timer_new->prev = NULL;
		timer_service.tail = timer_service.head = timer_new;
	}

	pthread_mutex_unlock(&mt_timer_service);

	return AK_RET_OK;
}

uint32_t timer_remove_attr(uint32_t des_task_id, uint32_t sig) {
	uint32_t ret;
	pthread_mutex_lock(&mt_timer_service);
	ret = timer_service_remove_node(des_task_id, sig);
	pthread_mutex_unlock(&mt_timer_service);
	return ret;
}

uint32_t timer_service_remove_node(uint32_t dst, uint32_t sig) {
	timer_msg_t* timer_node_temp = timer_service.tail;

	while (timer_node_temp != NULL) {
		if (timer_node_temp->data.des_task_id == dst &&
				timer_node_temp->data.sig == sig) {

			/* remove timer node */
			/* last node */
			if (timer_node_temp->next == NULL &&
					timer_node_temp->prev == NULL) {
				timer_service.head = timer_service.tail = NULL;
			}
			/* middle node */
			else if (timer_node_temp->next != NULL &&
					 timer_node_temp->prev != NULL) {
				timer_node_temp->next->prev = timer_node_temp->prev;
				timer_node_temp->prev->next = timer_node_temp->next;
			}
			/* tail node */
			else if (timer_node_temp->prev == NULL){
				timer_service.tail = timer_service.tail->next;
				timer_service.tail->prev = NULL;
			}
			/* head node */
			else if (timer_node_temp->next == NULL){
				timer_service.head = timer_service.head->prev;
				timer_service.head->next = NULL;
			}
			else {
				FATAL("TIMER", 0xAA);
			}

			free(timer_node_temp);

			return AK_RET_OK;
		}
		else {
			timer_node_temp = timer_node_temp->next;
		}
	}

	return AK_RET_NG;
}

uint32_t timer_stick_get() {
	return timer_stick_counter;
}
