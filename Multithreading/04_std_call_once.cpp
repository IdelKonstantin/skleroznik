#include <mutex>

/*
std::call_once - используется для блокировки критической секции
только на время инициализации (вместо однократного захвата мьютекса)
*/

struct data_packet {};
struct connection_handle {

    void send_data(data_packet const&) {}
    data_packet receive_data() {
        return data_packet();
    }
};

class X {
private:

    connection_handle connection;
    std::once_flag connection_init_flag;

    void open_connection() {

        //Как бы соединение формируется здесь (причем один раз)
        //this->connection = connection_manager(...);
    }
public:
    X(){};
    void send_data(data_packet const& data) {

        std::call_once(connection_init_flag,&X::open_connection,this);
        connection.send_data(data);
    }
    data_packet receive_data() {

        std::call_once(connection_init_flag,&X::open_connection,this);
        return connection.receive_data();
    }
};

int main() {}