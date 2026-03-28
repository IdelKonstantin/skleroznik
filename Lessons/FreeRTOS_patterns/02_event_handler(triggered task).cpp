void vEventHandlerTask(void *pvParameters) {

    while(1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); //Ожидание триггера (вечное)

        // Обработка события здесь...
    }
}

// Где-то в прерывании или другой задаче (xTaskNotify или xTaskNotifyFromISR):

xTaskNotifyFromISR(vEventHandlerTask, 0, eIncrement, NULL);                   //Увеличение счетчика уведомлений
xTaskNotifyFromISR(vEventHandlerTask, 0x01, eSetBits, NULL);                  //устанавливает биты (для множественных сигналов)
xTaskNotifyFromISR(vEventHandlerTask, 42, eSetValueWithOverwrite, NULL);      // eSetValueWithOverwrite - перезаписывает значение
xTaskNotifyFromISR(vEventHandlerTask, 100, eSetValueWithoutOverwrite, NULL);  // eSetValueWithoutOverwrite - устанавливает только если не было уведомлений
xTaskNotifyFromISR(vEventHandlerTask, 0, eNoAction, NULL);                    // eNoAction - просто уведомление без изменения значения

                                      ^----- Передаваемое в задачу значение

// Можно и вот так, для гарантированного переключения контекста:

void vISR_Handler(void) {
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    xTaskNotifyFromISR(
        vEventHandlerTask,     // Какая задача получает уведомление
        0,                     // Значение (не используется для eIncrement)
        eIncrement,            // Действие - увеличить счетчик
        &xHigherPriorityTaskWoken  // Для возможного переключения контекста (проверяется, что высокоприоритетная задача разбужена)
    );

    if (xHigherPriorityTaskWoken == pdTRUE) {     
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken); //Передача управления высокоприоритетной задаче (единый вызов)
    }
}                                      