#include "task_list.h"

ak_task_t task_list_init[] = {
	/* SYSTEM TASKS */
	{	DEFAULT_AK_FIRST_TASK,	TASK_PRI_LEVEL_1,	timer_entry     ,	&timer_mailbox		,	"timer service"			},

	/* APP TASKS */
	{	GW_TASK_APP_1,	TASK_PRI_LEVEL_1,	gw_task_app1_entry	,	&gw_task_app1_mailbox,	"handle commands"			},
	{	GW_TASK_APP_2,	TASK_PRI_LEVEL_1,	gw_task_app2_entry	,	&gw_task_app2_mailbox,	"handle commands"			},
};
