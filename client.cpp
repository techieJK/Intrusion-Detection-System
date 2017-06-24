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
	int sockfd;
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
	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	
	fstream file;
	file.open("test.jpg",ios::in|ios::binary);
	file.seekg(0,ios::end);
	int fileSize=file.tellg();
	cout<<"file size: "<<fileSize<<'\n';
	char *packet=new char[8];
	file.seekg(0,ios::beg);

	int blocks=ceil((float)fileSize/8);
	cout<<"No. of blocks: "<<blocks<<'\n';
	write(sockfd,&fileSize,sizeof(fileSize));
	
	while(blocks--)
	{
		file.read(packet,8);
		write(sockfd,packet,sizeof(packet));
	}
	
	
	close(sockfd);
	return 0;
}
