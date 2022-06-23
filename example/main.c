#include "main.h"

int main() {
	ak_init_tasks(AK_TASK_LIST_LEN, task_list_init);

    ak_start_all_tasks();

    ak_stop_all_tasks();

	return 0;
}



