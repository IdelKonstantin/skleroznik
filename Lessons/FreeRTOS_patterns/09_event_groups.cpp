//События (Event Groups) в FreeRTOS — это механизм синхронизации, позволяющий задачам ожидать наступления нескольких 
//различных событий одновременно или по сложной логике (например, «произошло событие A ИЛИ B», или «произошло A И B И C»).

//Если сравнивать с простыми инструментами:

//Семафор — это как дверь: ждем одного сигнала.
//Очередь — это как почтовый ящик: ждем данные.
//События — это как флажки в контрольном списке: ждем, пока установятся нужные флажки.

// В основе события лежит битовая маска (обычно 24 бита). Каждый бит отвечает за какое-то одно событие 
//(например, бит 0 — «Пришло сообщение по UART», бит 1 — «Нажата кнопка», бит 2 — «Готовы данные датчика»).

// Задача может:

//1) Установить (зажечь) один или несколько битов.
//2) Ожидать, пока определенные биты установятся (зажгутся) по логике ИЛИ (любой из списка) или И (все из списка).

/***************************************************************************************************************** 

Основные функции:

xEventGroupCreate() — создать группу событий
xEventGroupSetBits() — установить биты (задача сообщает о событии)
xEventGroupWaitBits() — ожидать биты (задача засыпает, пока не появятся нужные биты)
xEventGroupClearBits() — очистить биты (сбросить флаг)

*****************************************************************************************************************/

// Пример логики И: 

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#define BIT_SALAD_READY   (1 << 0)      // Бит 0: Салат готов
#define BIT_SOUP_READY    (1 << 1)      // Бит 1: Суп готов


EventGroupHandle_t xOrderEventGroup;    // Глобальная группа событий

// Задача 1: Повар, готовящий салат
void vTaskCookSalad(void *pvParameters) {

    while(1) {
        
        // СОБЫТИЕ: Салат готов! Устанавливаем бит
        xEventGroupSetBits(xOrderEventGroup, BIT_SALAD_READY);
    }
}

// Задача 2: Повар, готовящий суп
void vTaskCookSoup(void *pvParameters) {
    
    while(1) {

        // СОБЫТИЕ: Суп готов!
        xEventGroupSetBits(xOrderEventGroup, BIT_SOUP_READY);
    }
}

// Задача 3: Официант, ждущий оба блюда
void vTaskWaiter(void *pvParameters) {
    EventBits_t uxBits;
    
    while(1) {

        // Ждем, пока установятся оба бита (Салат И Суп)
        // Параметры:
        // 1. Группа событий
        // 2. Какие биты нас интересуют (BIT_SALAD_READY | BIT_SOUP_READY)
        // 3. xWaitForAllBits = pdTRUE (ждем ВСЕ биты)
        // 4. xClearOnExit = pdTRUE (автоматически очистить биты после выхода)
        // 5. Бесконечное ожидание

        uxBits = xEventGroupWaitBits(

            xOrderEventGroup,
            BIT_SALAD_READY | BIT_SOUP_READY,  // Интересуют оба бита
            pdTRUE,   // Очистить биты после получения (как флажок сняли)
            pdTRUE,   // Ждем ВСЕ биты (логика И)
            portMAX_DELAY
        );
        
        // Проверяем, что мы получили именно то, что ждали
        if((uxBits & (BIT_SALAD_READY | BIT_SOUP_READY)) == (BIT_SALAD_READY | BIT_SOUP_READY)) {
            
            // Do some magic here....
        }
    }
}

void main() {

    xOrderEventGroup = xEventGroupCreate();
    
    xTaskCreate(vTaskCookSalad, "Salad", 1000, NULL, 1, NULL);
    xTaskCreate(vTaskCookSoup, "Soup", 1000, NULL, 1, NULL);
    xTaskCreate(vTaskWaiter, "Waiter", 1000, NULL, 2, NULL);
    
    vTaskStartScheduler();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

//Пример логики ИЛИ: 

#define BIT_COFFEE   (1 << 0)
#define BIT_TEA      (1 << 1)

void vTaskBarista(void *pvParameters) {

    while(1) {

        xEventGroupSetBits(xDrinksGroup, BIT_COFFEE);
    }
}

void vTaskTeaMaster(void *pvParameters) {
    
    while(1) {

        xEventGroupSetBits(xDrinksGroup, BIT_TEA);
    }
}

void vTaskWaiterDrinks(void *pvParameters) {
    
    EventBits_t uxBits;
    
    while(1) {
        
        uxBits = xEventGroupWaitBits(   // Ждем ЛЮБОЙ бит из двух (логика ИЛИ)
            xDrinksGroup,
            BIT_COFFEE | BIT_TEA,   // Интересуют оба
            pdTRUE,                 // Очистить после получения
            pdFALSE,                // НЕ ждем все биты
            portMAX_DELAY
        );
        
        if(uxBits & BIT_COFFEE) {
            
            //Coffee...

        } else if(uxBits & BIT_TEA) {
            
            //Tee...
        }
    }
}