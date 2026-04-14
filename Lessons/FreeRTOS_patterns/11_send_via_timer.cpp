#include <cstdint>

typedef struct {

	uint32_t timeTicks{0};
	uint32_t fakeData{0};

} foo_t;

osTimerId_t				periodicTimer;		// идентификатор периодического таймера
osMutexId_t				sharedMutex;		// идентификатор мьютекса
osMessageQueueId_t		messageQueue;		// идентификатор очереди сообщений

foo_t globalData{};

void Timer_Callback(void *argument) {

	static uint32_t lastTimeTick{0};
	static foo_t localData;

	osStatus_t status;

	status = osMutexAcquire(sharedMutex, 0);

	if(status != osOK) {
		return;
	}

	localData = globalData;
	
	osMutexRelease(sharedMutex);

	if(localData.timeTicks != lastTimeTick) {

		lastTimeTick = localData.timeTicks;

		status = osMessageQueuePut(messageQueue, &localData, 0, 0);

		if(status != osOK) {

			// Очередь заполнена — обработка ошибки (логирование, индикация и т.д.)
		}
	}
}

void Dataprocesser_Thread(void *argument) {

	osStatus_t status;

	while(1) {

		status = osMutexAcquire(sharedMutex, 0);

		if(status == osOK) {
		
			globalData.timeTicks = osKernelGetTickCount();
			globalData.fakeData = 42;
			osMutexRelease(sharedMutex);
		}

		osDelay(pdMS_TO_TICKS(1));	
	}
}

void Dataconsumer_Thread(void *argument) {

	foo_t consumerData{};
	osStatus_t status;

	while(1) {

		status = osMessageQueueGet(messageQueue, &consumerData, NULL, osWaitForever);

		if(status == osOK) {

			//Prepare protobuf, lock semaphore and send protobuf into UART, then release semaphore in UART Tx callback
		}
	}
}

void InitRTOSObjects(void) {

	sharedMutex = osMutexNew(NULL);
	
	if (sharedMutex == NULL) {
		while(1);
	}

	messageQueue = osMessageQueueNew(10, sizeof(foo_t), NULL);

	if (messageQueue == NULL) {
		while(1);
	}

	periodicTimer = osTimerNew(Timer_Callback, osTimerPeriodic, NULL, NULL);

	if (periodicTimer == NULL) {
		while(1);
	}

	if (osTimerStart(periodicTimer, pdMS_TO_TICKS(5)) != osOK) {
		while(1);
	}

	const osThreadAttr_t processeAttr = {
		.name = "Processer",
		.stack_size = 1024,
		.priority = osPriorityNormal
	};

	osThreadNew(Dataprocesser_Thread, NULL, &processeAttr);

	const osThreadAttr_t consumerAttr = {
		.name = "Consumer",
		.stack_size = 1024,
		.priority = osPriorityHigh
	};

	osThreadNew(Dataconsumer_Thread, NULL, &consumerAttr);
}

int main(void) {
    
	HAL_Init();
	osKernelInitialize();
	InitRTOSObjects();
	osKernelStart();

	while(1);
}
