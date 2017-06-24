#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
#include<cstdlib>
#include<unistd.h>
#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

int main(int argc,char **argv)
{
	int sockfd,newfd;
	unsigned int len;
	struct sockaddr_in servaddr,cliaddr;
	char buff[1024];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0)
	{
		cout<<"cannot create socket";
		return 0;
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);
	servaddr.sin_port=htons(atoi(argv[2]));
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
	{
		cout<<"Bind error";
		return 0;
	}
	listen(sockfd,2);
	len=sizeof(cliaddr);
	newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);

	fstream file;
	file.open("test2.jpg",ios::out|ios::binary);
	
	int fileSize;
	char *packet=new char[8];
	read(newfd,&fileSize,sizeof(fileSize));
	
	int blocks=ceil((float)fileSize/8);
	cout<<"No. of blocks: "<<blocks<<'\n';
	char *content=new char[fileSize];
	int i=0;
	while(--blocks)
	{
		read(newfd,&packet,sizeof(packet));
		memcpy(content+i,&packet,8);
		i+=8;
	}
	read(newfd,&packet,sizeof(packet));
	memcpy(content+i,&packet,fileSize-i);//fileSize-i
	file.write(content,fileSize);
	close(sockfd);
	close(newfd);
	return 0;
}
