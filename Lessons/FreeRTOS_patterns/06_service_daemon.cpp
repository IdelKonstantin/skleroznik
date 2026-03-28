/* Высокоприоритетная задача-сервис, которая выполняет фоновую работу по запросу низкоприоритетных задач */

void vLowPriorityTask(void *pvParameters) {

    // Do some magic here and then ask high priority task for help :)
    xTaskNotify(vHeavyServiceTask, (uint32_t)&data, eSetValueWithOverwrite);
}

void vHeavyServiceTask(void *pvParameters) {
    
    Data_t *pData;
    
    while(1) {
        
        xTaskNotifyWait(0, 0, (uint32_t*)&pData, portMAX_DELAY);    // Ожидаем запрос вечно
        //Do heavy work here...
    }
}

/////////////////////////////////////////////////////////////////////////////////////

BaseType_t xTaskNotifyWait(
    uint32_t ulBitsToClearOnEntry,   // Очистить биты перед ожиданием (в примере выше не очишаем биты)
    uint32_t ulBitsToClearOnExit,    // Очистить биты после получения (в примере выше не очишаем биты)
    uint32_t *pulNotificationValue,  // Указатель для сохранения значения уведомления
    TickType_t xTicksToWait          // Время ожидания
);

