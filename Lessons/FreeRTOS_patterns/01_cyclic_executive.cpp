////////////////////////// "Мягкое" время

void vPeriodicTask(void *pvParameters) {
    
    const TickType_t xDelay = pdMS_TO_TICKS(100);

    while(1) {
        
        // Do some magic here...

        vTaskDelay(xDelay);
    }
}

////////////////////////// "Жесткое" время

void vPeriodicTask(void *pvParameters) {
    
    const TickType_t xDelay = pdMS_TO_TICKS(100);
    
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1) {
        
        // Do some magic here...

        vTaskDelayUntil(&xLastWakeTime, xDelay);
    }
}

////////////////////////// Ставим на паузу (не потребляем ресурсы CPU)

TaskHandle_t xPeriodicTaskHandle = NULL;

void vPeriodicTask(void *pvParameters) {
    
    const TickType_t xDelay = pdMS_TO_TICKS(100);
    
    xPeriodicTaskHandle = xTaskGetCurrentTaskHandle(); // Или просто используем хендлер задачи

    while(1) {
        // Do some magic here...
        
        vTaskDelay(xDelay);
    }
}

// Где-то в другом месте кода:

vTaskSuspend(xPeriodicTaskHandle);  // Поставить на паузу
vTaskResume(xPeriodicTaskHandle);   // Возобновить выполнение