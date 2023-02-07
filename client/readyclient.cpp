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
    char buff[1024];
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



int __cdecl main()
{

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    /* Confirm that the WinSock DLL supports 2.2.*/
    /* Note that if the DLL supports versions greater    */
    /* than 2.2 in addition to 2.2, it will still return */
    /* 2.2 in wVersion since that is the version we      */
    /* requested.                                        */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        printf("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 1;
    }
    else
        printf("The Winsock 2.2 dll was found okay\n");


    /* The Winsock DLL is acceptable. Proceed to use it. */

    /* Add network programming using Winsock here */

    /* then call WSACleanup when done using the Winsock dll */

    WSACleanup();

}
