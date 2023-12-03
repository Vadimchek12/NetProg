#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>
using namespace std;
int main()
{
    int s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == -1) {
        cerr << "Ошибка при создании сокета" << endl;
        return 1;
    }
    unique_ptr <sockaddr_in> serv_addr(new sockaddr_in);
    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(8080);
    serv_addr->sin_addr.s_addr = inet_addr("172.16.40.1");

    int rc = 0;

    rc = connect(s, (sockaddr*) serv_addr.get(),
                 sizeof(sockaddr_in));
    if (rc == -1) {
        std::cerr << "Ошибка при подключении" << std::endl;
        close(s);
        return 2;
    }
    string message = "Hello server!";
    rc = send(s, message.c_str(), message.length(), 0);
    if (rc == -1) {
        cerr << "Ошибка при отправке сообщения" << endl;
        close(s);
        return 3;
    }
    char buffer[4096];
    rc = recv(s, buffer, sizeof(buffer), 0);
    if (rc == -1) {
        cerr << "Ошибка при получении ответа" << endl;
        close(s);
        return 4;
    }
    buffer[rc] = '\0';
    cout << "Полученный ответ от сервера: " << buffer << endl;
    close(s);
    return 0;
}
