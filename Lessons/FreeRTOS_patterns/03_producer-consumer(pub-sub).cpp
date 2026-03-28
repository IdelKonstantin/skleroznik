QueueHandle_t xDataQueue;

void vProducerTask(void *pvParameters) {
    
    int32_t lDataToSend = 42;
    int32_t ticksToSend = 10;
    
    while(1) {
        
        // Копируем и отправляем данные в очередь (ждем максимум 10 тиков если очередь полна)
        if(xQueueSend(xDataQueue, &lDataToSend, ticksToSend) == pdPASS) {
        
            // Данные успешно отправлены
        } else {
        
            // Очередь заполнена, отправка не удалась
        }

        xQueueSend(xDataQueue, &lDataToSend, ticksToSend);  
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void vConsumerTask(void *pvParameters) {
    
    int32_t lReceivedData;
    
    while(1) {
        
        // Получаем данные из очереди с удалением (ждем вечно при portMAX_DELAY или не блокируемся при 0,
        // требует INCLUDE_vTaskSuspend == 1), результат errQUEUE_EMPTY/pdPASS

        if(xQueueReceive(xDataQueue, &lReceivedData, portMAX_DELAY) == pdPASS) {
            
            //Do some magic here...
        }

        // Получаем данные из очереди без удаления (ждем вечно при portMAX_DELAY или не блокируемся при 0,
        // требует INCLUDE_vTaskSuspend == 1), результат errQUEUE_EMPTY/pdPASS
        if(xQueuePeek(xDataQueue, &lReceivedData, 0) == pdPASS) {

            //Do some magic here...
        }

        // Получение из прерывания (логика, как в примерах выше)
        if(xQueueReceiveFromISR(xDataQueue, &lReceivedData, &xHigherPriorityTaskWoken) == pdPASS) {

            //Do some magic here...

            // Переключение контекста если необходимо
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

// В другом месте:

xDataQueue = xQueueCreate(10, sizeof(int32_t));
xTaskCreate(vProducerTask, "Prod", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
xTaskCreate(vConsumerTask, "Cons", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

/////////////////////////////////////////////////////////////////////////////////////////////

// Чтение из нескольких очередей

void vReceiverTask(void *pvParameters) {
    
    int32_t data;
    
    for(;;) {
        
        // Ждем данные из основной очереди
        if(xQueueReceive(xMainQueue, &data, portMAX_DELAY) == pdPASS) {
            ProcessMainData(data);
        }
        
        // Проверяем второстепенную очередь без блокировки
        while(xQueueReceive(xSecondaryQueue, &data, 0) == pdPASS) {
            ProcessSecondaryData(data); // Обрабатываем все накопленные
        }
    }
}