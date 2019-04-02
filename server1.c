#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")

int main(){
   WSADATA wsd;
   int res = WSAStartup(MAKEWORD(2,2),&wsd);
   if(res == SOCKET_ERROR){
      printf("WSASTARTUP ERROR ... \n");
      WSACleanup();
      exit(0);
   }
   SOCKET s;
   s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
   if(s == SOCKET_ERROR){
      printf("socket creation err ... \n");
      WSACleanup();
      exit(0);
   }
   SOCKADDR_IN servaddr;
   servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(1234);

   
   //binding socket
   bind(s,(SOCKADDR * )&servaddr,(int)sizeof(servaddr));
   listen(s , 4);
   
   // waiting for connection
   SOCKADDR_IN clientaddr;
   SOCKET clientsock;
   int clen = sizeof(clientsock);
   clientsock = accept(s,NULL,NULL);
   printf("accepted conn ... \n");
   // printf("Got conn. from %s\n",inet_ntoa(clientaddr.sin_addr));
   // printf("Got remote port %d",ntohs(clientaddr.sin_port));
   while(1)
   {  char buff[2065];
      int rb,sb;
      printf("enter data : \n");
      gets(buff);  
      sb = send(clientsock,buff,strlen(buff)+1,0);
      printf("%d",sb);
      memset(buff,0,sizeof(buff));
      rb  = recv(clientsock,buff,1024,0);
      printf("\t\t\t\t\t%s\n",buff);
      memset(buff,0,sizeof(buff));
   }
   closesocket(s);
   closesocket(clientsock);
   return 0;
}