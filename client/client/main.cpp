#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")

// Пример простого TCP-клиента
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <locale>
#include <iostream>

using namespace std;
#define PORT 10000
#define SERVERADDR "127.0.0.1"


DWORD WINAPI clientReceive(LPVOID lpParam)
{
    char buff[2048];
    SOCKET my_sock = *(SOCKET*)lpParam;
    int nsize;
    while ((nsize = recv(my_sock, &buff[0], sizeof(buff) - 1, 0)) != SOCKET_ERROR) {
        buff[nsize] = 0;
        printf(buff);
    }
}

DWORD WINAPI clientSend(LPVOID lpParam)
{
    char buff[1024];
    SOCKET my_sock = *(SOCKET*)lpParam;

    while (true) {
        fgets(&buff[0], sizeof(buff) - 1, stdin);
        send(my_sock, &buff[0], sizeof(buff) - 1, 0);
    }
}



int main()
{
    setlocale(LC_ALL, "RUS");

    WSAData wsaData;
    char buff[1024];
    printf("Клиент из стартед!\n");
    // Шаг 1 - инициализация библиотеки Winsock   
    if (WSAStartup(MAKEWORD(2, 2), (WSADATA*)&wsaData))
    {
        printf("WSAStart ошибка %i\n", WSAGetLastError());
        return -1;
    }
    // Шаг 2 - создание сокета
    SOCKET my_sock;
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0)
    {
        printf("Socket() ошибка %i\n", WSAGetLastError());
        return -1;
    }

    // Шаг 3 - установка соединения
    // заполнение структуры sockaddr_in – указание адреса и порта сервера
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    HOSTENT* hst;
    // преобразование IP адреса из символьного в сетевой формат
    if (inet_addr(SERVERADDR) != INADDR_NONE)
        dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
    else
        // попытка получить IP адрес по доменному имени сервера
        if (hst = gethostbyname(SERVERADDR))
            // hst->h_addr_list содержит не массив адресов,
            // а массив указателей на адреса
            ((unsigned long*)&dest_addr.sin_addr)[0] =
            ((unsigned long**)hst->h_addr_list)[0][0];
        else
        {
            printf("Инвалид адрес %s\n", SERVERADDR);
            closesocket(my_sock);
            WSACleanup();
            return -1;
        }
    // адрес сервера получен – пытаемся установить соединение
    if (connect(my_sock, (sockaddr*)&dest_addr, sizeof(dest_addr)))
    {
        printf("Коннект эррор %i\n", WSAGetLastError());
        return -1;
    }

    printf("Соединение с %s успешно установлено\n\
         Наберите quit для завершения\n\n", SERVERADDR);



    HANDLE t1 = CreateThread(NULL,
        0,
        clientReceive,
        &my_sock,
        0, NULL);

    if (t1 == NULL)
        cout << "Thread creation error: "
        << GetLastError();

    HANDLE t2 = CreateThread(NULL,
        0,
        clientSend,
        &my_sock,
        0, NULL);

    if (t2 == NULL)
        cout << "Thread creation error: "
        << GetLastError();


    WaitForSingleObject(t1, INFINITE);
    WaitForSingleObject(t2, INFINITE);


    closesocket(my_sock);
    printf("Recv error %d\n", GetLastError());
    WSACleanup();
    return-1;
}
