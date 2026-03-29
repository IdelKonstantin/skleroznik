// Задача - предохранитель

enum {

	TASK_KICK_1 = 0,
	TASK_KICK_2,
	TASK_KICK_3
};

////////////////////////////////////////////////////////////////////////

void task_1(void *pvParameters) {

	while (1) {

		// Симуляция полезной работы тут...

		watchdog_kick(TASK_KICK_1);		//Пинок в предохранитель
		vTaskDelay(pdMS_TO_TICKS(5000));
	}
}

void task_2(void *pvParameters) {

	while (1) {

		// Симуляция полезной работы тут...

		watchdog_kick(TASK_KICK_2);		//Пинок в предохранитель
		vTaskDelay(pdMS_TO_TICKS(3000));
	}
}

void task_3(void *pvParameters) {

	while (1) {

		// Симуляция полезной работы тут...

		watchdog_kick(TASK_KICK_3);		//Пинок в предохранитель
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

////////////////////////////////////////////////////////////////////////

#define WATCHDOG_TIMEOUT_TICKS   (pdMS_TO_TICKS(10000)) // 10 секунд

// Структура для хранения состояния каждой задачи
typedef struct {

	TaskHandle_t handle;        // Хендл задачи
	TickType_t last_tick;       // Время последнего "пинка"
	bool is_alive;              // Флаг для отладки
	const char *name;           // Имя задачи

} task_watchdog_item_t;

// Массив задач для контроля (3 рабочие задачи)
static task_watchdog_item_t watched_tasks[3];

// Функция "пинка" (обновляет время жизни задачи)
// Вызывается каждой рабочей задачей в своем цикле

void watchdog_kick(int task_index) {
	
	if (task_index >= 0 && task_index < 3) {
	
		watched_tasks[task_index].last_tick = xTaskGetTickCount();
		watched_tasks[task_index].is_alive = true;
	}
}

// Задача-предохранитель (сторож)
void watchdog_task(void *pvParameters) {

    while (1) {
        
		TickType_t now = xTaskGetTickCount();
		bool all_alive = true;

		// Проверяем каждую задачу
		for (int i = 0; i < 3; i++) {

			// Если хендл не нулевой (задача создана) и время с последнего пинка больше таймаута

			if (watched_tasks[i].handle != NULL) {

				TickType_t diff = now - watched_tasks[i].last_tick;

				if (diff > WATCHDOG_TIMEOUT_TICKS) {
					all_alive = false;

				}
			}
		}

		// Если хотя бы одна задача не отвечает - перезагружаем устройство
		if (!all_alive) {

			//Failure callback here...
			NVIC_SystemReset() или аналоги
		}

		vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

////////////////////////////////////////////////////////////////////////////

void main(void) {

    // Инициализируем структуру задач для контроля
    watched_tasks[0] = (task_watchdog_item_t) {
        .handle = NULL,
        .last_tick = 0,
        .is_alive = false,
        .name = "Task_1"
    };
    watched_tasks[1] = (task_watchdog_item_t){
        .handle = NULL,
        .last_tick = 0,
        .is_alive = false,
        .name = "Task_2"
    };
    watched_tasks[2] = (task_watchdog_item_t){
        .handle = NULL,
        .last_tick = 0,
        .is_alive = false,
        .name = "Task_3"
    };

    // Создаем рабочие задачи
    xTaskCreate(task_1, "Task1", 4096, NULL, 5, &watched_tasks[0].handle);
    xTaskCreate(task_2, "Task2", 4096, NULL, 5, &watched_tasks[1].handle);
    xTaskCreate(task_3, "Task3", 4096, NULL, 5, &watched_tasks[2].handle);

    // Убеждаемся, что все задачи созданы успешно
    if (watched_tasks[0].handle == NULL || watched_tasks[1].handle == NULL || watched_tasks[2].handle == NULL) {
        
        NVIC_SystemReset();
    }

    // Инициализируем время последнего пинка (текущее время, чтобы не сработал ложный сброс)
    TickType_t start_tick = xTaskGetTickCount();
    
    for (int i = 0; i < 3; i++) {
    
        watched_tasks[i].last_tick = start_tick;
    }

    // Запускаем задачу-сторож (приоритет выше, чем у рабочих задач, для надежности)
    xTaskCreate(watchdog_task, "Watchdog", 2048, NULL, 10, NULL);
}