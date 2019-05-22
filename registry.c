#include <WinSock2.h>
#include <windows.h>
#include <MSWSock.h>
#include <stdio.h>
#pragma comment(lib,"Mswsock.lib")
#pragma comment(lib,"ws2_32.lib")

SOCKET s;
char buff[1024];

void regquery(char my[]){
    char *arr;
    char *arr1;
    char *arr2;
    arr = strtok(my," ");
    arr1 = strtok(NULL," ");
    arr2 = strtok(NULL," ");
    HKEY hkey;
    if(strcmp(arr1,"HKEY_LOCAL_MACHINE") == 0)
        hkey = HKEY_LOCAL_MACHINE;
    else if(strcmp(arr1,"HKEY_CURRENT_USER") == 0)
        hkey = HKEY_CURRENT_USER;
    else if(strcmp(arr,"HKEY_CLASSES_ROOT") == 0)
        hkey = HKEY_CLASSES_ROOT;
    else if(strcmp(arr1,"HKEY_USERS") == 0)
        hkey = HKEY_USERS;
    else if(strcmp(arr1,"HKEY_CURRENT_CONFIG") == 0)
        hkey = HKEY_CURRENT_CONFIG;
    else {
        strcpy(buff,"INVALID HIVE NAME");
        return ;
    }
    // LONG res = RegQueryValueEx()
}

void regopenkey(char my[]){
    char *arr,*arr1;
    char *arr2;
    arr = strtok(my," ");
    arr1 = strtok(NULL, " ");
    arr2 = strtok(NULL," ");
    HKEY hkey;
    LPCSTR subkey = (const char *)arr2;
    if(strcmp(arr1,"HKEY_LOCAL_MACHINE") == 0)
        hkey = HKEY_LOCAL_MACHINE;
    else if(strcmp(arr1,"HKEY_CURRENT_USER") == 0)
        hkey = HKEY_CURRENT_USER;
    else if(strcmp(arr,"HKEY_CLASSES_ROOT") == 0)
        hkey = HKEY_CLASSES_ROOT;
    else if(strcmp(arr1,"HKEY_USERS") == 0)
        hkey = HKEY_USERS;
    else if(strcmp(arr1,"HKEY_CURRENT_CONFIG") == 0)
        hkey = HKEY_CURRENT_CONFIG;
    else {
        strcpy(buff,"INVALID HIVE NAME");
        return ;
    }   
    PHKEY phkey;
    LONG res = RegOpenKeyEx(hkey,subkey,0,KEY_ALL_ACCESS,phkey);
    if( res!= ERROR_SUCCESS)
    strcpy(buff,"error opening registry key\n");
    else{
        int status = RegCloseKey(hkey);
        if(status == ERROR_SUCCESS)
            strcpy(buff,"key closed successfully \n");
        else
            strcpy(buff,"cant close key \n");
    }
}

void myregistryshell(char *my){
    char *arr;
    char b1[strlen(my) + 1 ];
    strcpy(b1,my);
    arr = strtok(b1," ");
    if(strcmp("querykey",arr) == 0){
        regquery(my);
    }
    else if(strcmp("openkey",arr) == 0){
        regopenkey(my);
    }else
        strcpy(buff,"none of the function executed\n ");
}

int main(){
    WSADATA wsd;

    //SEARCHING FOR WINSOCK VERSION 2.2
    int res = WSAStartup(MAKEWORD(2,2),&wsd);
    if(res == SOCKET_ERROR){
        printf("WSAStartup error ... ");
        WSACleanup();
        exit(0);
    }
    // CREATING SOCKET FOR CONNECTION
    s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if( s == SOCKET_ERROR){
        printf("error in creating socket .. ");
        WSACleanup();
        exit(0);
    }
    SOCKADDR_IN serveraddr;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(1234);

    if(connect(s,(SOCKADDR *)&serveraddr,sizeof(serveraddr)) == SOCKET_ERROR){
        printf("error in connection ...\n");
        WSACleanup();
        exit(0);
    }
    while(1){
        int b ;
        char my[1024];
        b = recv(s,buff,1024,0);
        printf("recv data :\t\t\t\t %s\n",buff);
        strcpy(my,buff);
        ZeroMemory(buff,sizeof(buff));
        myregistryshell(my);
        send(s,buff,strlen(buff)+1,0);
        ZeroMemory(my,sizeof(my));
        ZeroMemory(buff,sizeof(buff));
    }
    closesocket(s);
    return 0 ;
}