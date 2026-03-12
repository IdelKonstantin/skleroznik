#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dbus/dbus.h>

#define SERVICE_NAME    "com.laowai.Robot"
#define OBJECT_PATH     "/robot"
#define INTERFACE_NAME  "com.laowai.Robot"

void send_method_call(DBusConnection *conn, const char *method, const char *arg) {
    DBusMessage *msg;
    DBusMessage *reply;
    DBusError err;
    
    dbus_error_init(&err);

    // Создаем сообщение
    if (arg) {
        msg = dbus_message_new_method_call(SERVICE_NAME,
                                            OBJECT_PATH,
                                            INTERFACE_NAME,
                                            method);
        if (!msg) {
            fprintf(stderr, "Ошибка: не удалось создать сообщение\n");
            return;
        }

        if (!dbus_message_append_args(msg,
                                       DBUS_TYPE_STRING, &arg,
                                       DBUS_TYPE_INVALID)) {
            fprintf(stderr, "Ошибка: не удалось добавить аргумент\n");
            dbus_message_unref(msg);
            return;
        }
        printf("[Клиент] 📤 Отправляю: %s \"%s\"\n", method, arg);
    } else {
        msg = dbus_message_new_method_call(SERVICE_NAME,
                                            OBJECT_PATH,
                                            INTERFACE_NAME,
                                            method);
        if (!msg) {
            fprintf(stderr, "Ошибка: не удалось создать сообщение\n");
            return;
        }
        printf("[Клиент] 📤 Отправляю: %s\n", method);
    }

    // Отправляем и ждем ответ
    reply = dbus_connection_send_with_reply_and_block(conn, msg, 5000, &err);
    if (!reply) {
        if (dbus_error_is_set(&err)) {
            fprintf(stderr, "[Клиент] ❌ Ошибка: %s\n", err.message);
            dbus_error_free(&err);
        } else {
            fprintf(stderr, "[Клиент] ❌ Таймаут или ошибка\n");
        }
        dbus_message_unref(msg);
        return;
    }

    // Читаем ответ
    char *response_text = NULL;
    if (dbus_message_get_args(reply, &err,
                              DBUS_TYPE_STRING, &response_text,
                              DBUS_TYPE_INVALID)) {
        printf("[Клиент] 📥 Робот ответил: \"%s\"\n", response_text);
    } else {
        if (dbus_error_is_set(&err)) {
            fprintf(stderr, "[Клиент] Ошибка чтения ответа: %s\n", err.message);
            dbus_error_free(&err);
        }
    }
    
    dbus_message_unref(msg);
    dbus_message_unref(reply);
}

int main() {
    DBusError err;
    DBusConnection *conn;

    dbus_error_init(&err);

    // Подключаемся к сессионной шине
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (!conn) {
        fprintf(stderr, "[Клиент] Ошибка подключения: %s\n", err.message);
        dbus_error_free(&err);
        exit(1);
    }

    printf("[Клиент] 🔌 Подключен к шине\n");
    printf("🤖 Управление роботом:\n");
    printf("  hello    - позвать робота\n");
    printf("  вперед   - движение вперед\n");
    printf("  назад    - движение назад\n");
    printf("  влево    - поворот налево\n");
    printf("  вправо   - поворот направо\n");
    printf("  exit     - выход\n");

    char command[100];

    while (1) {
        printf("\n> ");
        fflush(stdout);
        
        if (!fgets(command, sizeof(command), stdin)) break;
        command[strcspn(command, "\n")] = 0;
        
        if (strlen(command) == 0) continue;

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "hello") == 0) {
            send_method_call(conn, "SayHello", NULL);
        } else if (strcmp(command, "вперед") == 0 ||
                   strcmp(command, "назад") == 0 ||
                   strcmp(command, "влево") == 0 ||
                   strcmp(command, "вправо") == 0) {
            send_method_call(conn, "Move", command);
        } else {
            printf("Неизвестная команда: %s", command);
        }
    }

    dbus_connection_unref(conn);
    printf("[Клиент] Завершение работы\n");
    return 0;
}