#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>

#define SERVICE_NAME    "com.laowai.Robot"
#define OBJECT_PATH     "/robot"
#define INTERFACE_NAME  "com.laowai.Robot"

// Обработчик входящих сообщений
DBusHandlerResult handle_robot_method(DBusConnection *conn,
                                       DBusMessage *msg,
                                       void *user_data) {
    (void)user_data;  // Заглушка для неиспользуемого параметра
    
    // --- SayHello ---
    if (dbus_message_is_method_call(msg, INTERFACE_NAME, "SayHello")) {
        printf("[Робот] Получил команду SayHello\n");

        DBusMessage *reply = dbus_message_new_method_return(msg);
        if (!reply) {
            fprintf(stderr, "Ошибка: не удалось создать ответ\n");
            return DBUS_HANDLER_RESULT_NEED_MEMORY;
        }

        // Отправляем осмысленный ответ
        const char *response_text = "Приветствую, командир! Я к вашим услугам.";
        if (!dbus_message_append_args(reply,
                                       DBUS_TYPE_STRING, &response_text,
                                       DBUS_TYPE_INVALID)) {
            fprintf(stderr, "Ошибка: не удалось добавить аргумент в ответ\n");
        }

        if (!dbus_connection_send(conn, reply, NULL)) {
            fprintf(stderr, "Ошибка: не удалось отправить ответ\n");
        }
        dbus_message_unref(reply);

        printf("[Робот] Ответ отправлен: \"%s\"\n", response_text);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    // --- Move ---
    if (dbus_message_is_method_call(msg, INTERFACE_NAME, "Move")) {
        DBusError err;
        dbus_error_init(&err);

        char *direction = NULL;

        // Читаем аргумент (направление движения)
        if (!dbus_message_get_args(msg, &err,
                                   DBUS_TYPE_STRING, &direction,
                                   DBUS_TYPE_INVALID)) {
            fprintf(stderr, "[Робот] Ошибка чтения аргумента: %s\n", err.message);
            dbus_error_free(&err);

            // Отправляем ошибку
            DBusMessage *error = dbus_message_new_error(msg,
                                                         "com.laowai.Robot.InvalidArgument",
                                                         "Ожидался один аргумент типа string");
            if (error) {
                dbus_connection_send(conn, error, NULL);
                dbus_message_unref(error);
            }
            return DBUS_HANDLER_RESULT_HANDLED;
        }

        // Делаем копию строки (чтобы безопасно работать)
        char *direction_copy = strdup(direction);
        printf("[Робот] Получил команду Move: \"%s\"\n", direction_copy);

        // Формируем ответ в зависимости от направления
        char response_buffer[256];
        
        if (strcmp(direction_copy, "вперед") == 0) {
            snprintf(response_buffer, sizeof(response_buffer), 
                     "✅ Робот двигается ВПЕРЕД со скоростью 0.5 м/с");
        } else if (strcmp(direction_copy, "назад") == 0) {
            snprintf(response_buffer, sizeof(response_buffer), 
                     "✅ Робот двигается НАЗАД, будьте осторожны");
        } else if (strcmp(direction_copy, "влево") == 0) {
            snprintf(response_buffer, sizeof(response_buffer), 
                     "✅ Робот поворачивает НАЛЕВО, угол 90 градусов");
        } else if (strcmp(direction_copy, "вправо") == 0) {
            snprintf(response_buffer, sizeof(response_buffer), 
                     "✅ Робот поворачивает НАПРАВО, угол 90 градусов");
        } else {
            snprintf(response_buffer, sizeof(response_buffer), 
                     "⚠️ Неизвестная команда движения: \"%s\"", direction_copy);
        }

        // Создаем ответ с этим сообщением
        DBusMessage *reply = dbus_message_new_method_return(msg);
        if (reply) {
            const char *resp_text = response_buffer;
            if (!dbus_message_append_args(reply,
                                           DBUS_TYPE_STRING, &resp_text,
                                           DBUS_TYPE_INVALID)) {
                fprintf(stderr, "Ошибка: не удалось добавить аргумент в ответ\n");
            }
            
            dbus_connection_send(conn, reply, NULL);
            dbus_message_unref(reply);
        }

        free(direction_copy);
        printf("[Робот] Ответ отправлен: \"%s\"\n", response_buffer);
        return DBUS_HANDLER_RESULT_HANDLED;
    }

    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

// Обработчик сообщений для зарегистрированного пути
DBusHandlerResult path_message_handler(DBusConnection *conn,
                                        DBusMessage *msg,
                                        void *user_data) {
    return handle_robot_method(conn, msg, user_data);
}

// Обработчик отключения пути
void path_unregister_handler(DBusConnection *conn, void *user_data) {
    (void)conn;
    (void)user_data;
    printf("[Робот] Путь %s отрегистрирован\n", OBJECT_PATH);
}

// VTable с указателями на наши функции
DBusObjectPathVTable robot_vtable = {
    .unregister_function = path_unregister_handler,
    .message_function = path_message_handler,
    .dbus_internal_pad1 = NULL,
    .dbus_internal_pad2 = NULL,
    .dbus_internal_pad3 = NULL,
    .dbus_internal_pad4 = NULL
};

int main() {
    DBusError err;
    DBusConnection *conn;
    int ret;

    printf("[Робот] Запуск...\n");
    printf("[Робот] PID: %d\n", getpid());

    dbus_error_init(&err);

    // Подключаемся к сессионной шине
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (!conn) {
        fprintf(stderr, "[Робот] Ошибка подключения: %s\n", err.message);
        dbus_error_free(&err);
        exit(1);
    }

    // Запрашиваем известное имя
    ret = dbus_bus_request_name(conn, SERVICE_NAME,
                                DBUS_NAME_FLAG_REPLACE_EXISTING,
                                &err);
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "[Робот] Не удалось получить имя %s: %s\n",
                SERVICE_NAME, err.message);
        dbus_error_free(&err);
        exit(1);
    }
    printf("[Робот] ✅ Успешно зарегистрирован как %s\n", SERVICE_NAME);

    // Регистрируем путь объекта с VTable
    if (!dbus_connection_register_object_path(conn,
                                               OBJECT_PATH,
                                               &robot_vtable,
                                               NULL)) {
        fprintf(stderr, "[Робот] ❌ Не удалось зарегистрировать путь %s\n", OBJECT_PATH);
        exit(1);
    }
    printf("[Робот] ✅ Путь объекта: %s (с VTable)\n", OBJECT_PATH);

    printf("[Робот] 🤖 Ожидание вызовов...\n");
    printf("[Робот] Нажми Ctrl+C для выхода\n\n");

    // Главный цикл обработки сообщений
    while (dbus_connection_read_write_dispatch(conn, -1)) {
        // Бесконечный цикл обработки
    }

    printf("[Робот] Завершение работы\n");
    dbus_connection_unref(conn);
    return 0;
}