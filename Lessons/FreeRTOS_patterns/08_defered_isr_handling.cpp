void vHighPriorityHandlerTask(void *pvParameters) {

    while(1) {
        
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    // Ждем уведомления от прерывания

        // Длинная обработка события, которую нельзя было делать в прерывании
    }
}

void ADC_IRQHandler(void) {
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // ... очистка флага прерывания, чтение данных ADC

    vTaskNotifyGiveFromISR(vHighPriorityHandlerTask, &xHigherPriorityTaskWoken);    // Пробуждаем задачу
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);                                   // Немедленное переключение контекста если нужно
}