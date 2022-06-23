
#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "ak/ak.h"
#include "task_1.h"
#include "task_2.h"

enum {
	/* SYSTEM TASKS */
	DEFAULT_AK_FIRST_TASK = AK_TASK_TIMER_ID,

	/* APP TASKS */
	GW_TASK_APP_1,
	GW_TASK_APP_2,
	/* EOT task ID */
	AK_TASK_LIST_LEN,
};

extern ak_task_t task_list_init[];

#endif


