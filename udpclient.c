// Client side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT	 8080 
#define MAXLINE 1024 


int main() 
{
	char input[50];
	printf("please enter the message (less than 50 char)\n");
	scanf("%[^\n]%*c", input); 
	strcat(input,"\n");
	int inplen = strlen(input);
	char inpb[400];

	for(int i=0; i<inplen; i++)
	{
		char character[8];
		int val = (int) input[i];
		int len=0;

		while(len != 8)
		{
			character[7-len] = ((char)(val%2+'0'));
			val = val/2;
			len++;
		}

		strcat(inpb,character);
	}



	
	printf("The message is : %s\n", input);
	printf("The message converted to binary is %s\n",inpb);

	strcat(inpb,"0000000000000000");
	int messlen = strlen(inpb);

	char poly[30];
	printf("please enter a 17 bit 0s and 1s to be used as polynomial, leading digit should be 1, example : 10001000000100001\n");
	scanf("%s",poly);
	
	//enter flag to check if only 0 and 1 are present

	while(strlen(poly) != 17 || poly[0] != '1')
	{
		printf("Incorrect! please enter a 17 digit 0s and 1s to be used as polynomial, leading digit should be 1\n");
		scanf("%s",poly);
	}	
		
	printf("The polynomial which you have entered to be used is: %s\n",poly);

	int nextind = 0;
	char copy[17];		//temporarily used to store current dividend
	memcpy(copy,inpb+nextind,17);
	copy[17]='\0';
	nextind += 17;


	// while(nextind < messlen)
	// {
	// 	for(int i=0; i<17; i++)
	// 		copy[i]=((copy[i]-'0')^(poly[i]-'0'))+'0';

	// 	int start = 0;
	// 	while(copy[start]=='0')
	// 		start++;

	// 	int rem = messlen-nextind;

	// 	if(rem>=start)
	// 		{//copy = copy.substr(start) + inpb.substr(nextind,start);
			
	// 		 strcpy(copy,copy+start);
	// 		 strncat(copy,inpb+nextind,start);
	// 		nextind+= start;}
	// 	else
	// 	{

	// 		//copy = copy.substr(rem,17-rem) + inpb.substr(nextind,rem);
	// 		strcpy(copy,copy+rem);
	// 		strncat(copy,inpb+nextind,rem);
	// 		nextind = messlen;
	// 	}

	// 	printf("%s\n",copy);
	// }


	while(nextind<messlen)
	{
		if(copy[0]=='0')
		{
			strcpy(copy,copy+1);
			copy[16]=inpb[nextind++];
			continue;
		}

		for(int i=0; i<17; i++)
			copy[i]=((copy[i]-'0')^(poly[i]-'0'))+'0';
	}

	if(copy[0]=='1')
	for(int i=0; i<17; i++)
			copy[i]=((copy[i]-'0')^(poly[i]-'0'))+'0';

	printf("The crc value calculated is: %s\n",copy+1);
	//string finalMessage = inpb.substr(0,messlen-16) + copy.substr(1);
	char finalMessage[400];
	memcpy(finalMessage,inpb,messlen-16);
	finalMessage[messlen-16]='\0';
	strcat(finalMessage,copy+1);
	printf("The message appended with crc to be sent is : %s\n",finalMessage);
	
	

	//creating server socket

	int sockfd; 
	//char buffer[MAXLINE]; 
	//char *hello = "Hello from client"; 
	struct sockaddr_in	 servaddr; 

	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 

	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(PORT); 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	
	int n, len; 
	
	sendto(sockfd, (const char *)poly, strlen(poly), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr)); 
	printf("polynomial sent...\n"); 
		

	sendto(sockfd, (const char *)finalMessage, strlen(finalMessage), 
		MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
			sizeof(servaddr)); 
	printf("message sent...\n"); 
		

	char buffer[30];
	n = recvfrom(sockfd, (char *)buffer, 30, 
				MSG_WAITALL, ( struct sockaddr *) &servaddr, 
				&len); 

	printf("server says :%s\n", buffer);

	close(sockfd); 
	return 0; 
} 

