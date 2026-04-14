#include <cstdint>

#define SEND_DELAY_MS 5

typedef struct {

	uint32_t timeTicks{0};
	uint32_t fakeData{0};

} foo_t;

osMutexId_t			sharedMutex;		// идентификатор мьютекса

void Dataprocesser_Thread(void *argument) {

	foo_t localData{};	
	static uint32_t lastTimeTick{0};

	osStatus_t status;

	while(1) {

		status = osMutexAcquire(sharedMutex, 0);

		if(status == osOK) {
		
			localData.timeTicks = osKernelGetTickCount();
			localData.fakeData = 42;

			osMutexRelease(sharedMutex);

			if(localData.timeTicks - lastTimeTick >= pdMS_TO_TICKS(SEND_DELAY_MS)) {

				lastTimeTick = localData.timeTicks;

				//Prepare protobuf, lock semaphore and send protobuf into UART, then release semaphore in UART Tx callback
			}	
		}

		osDelay(pdMS_TO_TICKS(1));	
	}
}

void InitRTOSObjects(void) {

	sharedMutex = osMutexNew(NULL);

	if (sharedMutex == NULL) {
		while(1);
	}

	const osThreadAttr_t processeAttr = {

		.name = "Processer",
		.stack_size = 1024,
		.priority = osPriorityNormal
	};

	osThreadNew(Dataprocesser_Thread, NULL, &processeAttr);
}

int main(void) {
    
	HAL_Init();

	osKernelInitialize();

	InitRTOSObjects();

	osKernelStart();

	while(1);
}
