#include "task_2.h"

q_msg_t gw_task_app2_mailbox;

void* gw_task_app2_entry(void) {
	wait_all_tasks_started();

	AK_PRINT("[STARTED] gw_task_app2_entry\n");

	ak_msg_t* msg;

	timer_set(GW_TASK_APP_2, 0, 1000, TIMER_PERIODIC);

	while (1) {
		/* get messge */
		msg = ak_msg_rev(GW_TASK_APP_2);

		switch (msg->header->sig) {

		case 0: {
			char* data = (char*)msg->header->payload;
			AK_PRINT("[DEBUG] app1 running with thread id: %d\n", get_task_id());
			unsigned char* test_msg = (unsigned char*)calloc(1, 50);
			strcpy(test_msg, "hello from app2");
			task_post_dynamic_msg(GW_TASK_APP_1, 1, (uint8_t*)test_msg, strlen(test_msg)+1);
		}
			break;

		case 1: {
			char* data = (char*)msg->header->payload;
			if(data)
				AK_PRINT("[DEBUG] app2 recieve message: %s\n", data);
		}
			break;

		default:
			break;
		}

		/* free message */
		ak_msg_free(msg);
	}

	return (void*)0;
}