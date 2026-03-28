typedef enum {
    
    EV_MEASURE = 0,
    EV_STOP,
    EV_SEND

} EventType_t;

typedef struct {

    EventType_t type;
    int32_t data;

} Event_t;

typedef enum {

    STATE_IDLE = 0,
    STATE_MEASURE,
    STATE_SEND

} SystemState_t;

///////////////////////////////////////////////////////////////////////

void vStateMachineTask(void *pvParameters) {
    
    SystemState_t eCurrentState = STATE_IDLE;
    
    int32_t lSensorValue;
    Event_t xEvent;

    while(1) {
        
        switch(eCurrentState) {
            
            case STATE_IDLE:
                
                if(xQueueReceive(xEventQueue, &xEvent, 0) == pdPASS) {
                
                    if(xEvent.type == EV_MEASURE) {
                
                        eCurrentState = STATE_MEASURE;
                    }
                }
                break;

            case STATE_MEASURE:
                
                //Measure here...
                eCurrentState = STATE_SEND;
                break;

            case STATE_SEND:
                
                //Send measurements
                eCurrentState = STATE_IDLE;
                break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}