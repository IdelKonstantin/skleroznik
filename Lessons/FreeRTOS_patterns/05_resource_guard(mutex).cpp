SemaphoreHandle_t xMutex;

//Где-то в другой функции:
xMutex = xSemaphoreCreateMutex();

void vTaskUsingSPI(void *pvParameters) {
    
    while(1) {

        if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {   // Блокирует только другие задачи с этим мьютексом
            
            // Shared resource or BUS 
            xSemaphoreGive(xMutex);
        }
    }
}


/////////////////////////////////////////////////////////////

void vTaskUsingSPI(void *pvParameters) {
    
    while(1) {

        taskENTER_CRITICAL();           // Останавливает все задачи и прерывания! (не произойдет передачи в шине)

        // Shared resource or BUS 

        taskEXIT_CRITICAL();
    }
}