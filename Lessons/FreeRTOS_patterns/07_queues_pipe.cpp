//Task1 -> Queue1 -> Task2 -> Queue2 -> etc...

void vStage1_Task(void *pvParameters) {
    
    RawData_t raw;
    ProcessedData_t processed;
    
    while(1) {

        //Работаем с raw и формируем processed и передаем другой задаче
        xQueueSendToBack(xQueueStage1to2, &processed, portMAX_DELAY);
    }
}

void vStage2_Task(void *pvParameters) {
    
    ProcessedData_t processed;
    Result_t result;
    
    while(1) {
        
        xQueueReceive(xQueueStage1to2, &processed, portMAX_DELAY);
        
        //Работаем с processed и формируем result и передаем другой задаче и тд и тп

        xQueueSendToBack(xQueueStage2to3, &result, portMAX_DELAY);
    }
}