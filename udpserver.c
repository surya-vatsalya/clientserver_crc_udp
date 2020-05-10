// Server side implementation of UDP client-server model 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <math.h>

#define PORT	 8080 
#define MAXLINE 1024 

// Driver code 
int main() { 

	//declaring message and polynomial
	char message[400];
	char poly[30];

	int sockfd; 
	struct sockaddr_in servaddr, cliaddr; 
	
	// Creating socket file descriptor 
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	memset(&servaddr, 0, sizeof(servaddr)); 
	memset(&cliaddr, 0, sizeof(cliaddr)); 
	
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = INADDR_ANY; 
	servaddr.sin_port = htons(PORT); 
	
	// Bind the socket with the server address 
	if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	
	int len, n; 
	len = sizeof(cliaddr); //len is value/resuslt 
	n = recvfrom(sockfd, (char *)poly, 30, 
				MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
				&len); 
	poly[n] = '\0'; 
	printf("polynomial sent by client is:  %s\n", poly); 

	n = recvfrom(sockfd, (char *)message, 400, 
				MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
				&len); 
	message[n] = '\0'; 
	printf("message appended with crc sent by client is: %s\n", message); 
	int messlen = strlen(message);
		
	// int nextind = 0;
	// char copy[30];//temporarily used to store current dividend


	// while(message[nextind] == '0')
	// 	nextind++;	

	// //copy = message.substr(nextind,17);
	// memcpy(copy,message+nextind,17);
	// copy[17]='\0';
	// nextind += 17;

	int nextind = 0;
	char copy[17];		//temporarily used to store current dividend
	memcpy(copy,message+nextind,17);
	copy[17]='\0';
	nextind += 17;

	//cout<<nextind<<endl;
	// //cout<<copy<<endl;
	// printf("%s\n",message);
	// printf("%d %s\n",strlen(poly),poly);
	// printf("%d %s\n",strlen(copy),copy);

	// while(nextind < messlen)
	// {
	// 	for(int i=0; i<17; i++)
	// 		copy[i]=((copy[i]-'0')^(poly[i]-'0'))+'0';

	// 	int start = 0;
	// 	while(copy[start]=='0')
	// 		start++;

	// 	int rem = messlen-nextind;

	// 	if(rem>=start)
	// 		{//copy = copy.substr(start) + message.substr(nextind,start);
	// 		 strcpy(copy,copy+start);
	// 		 strncat(copy,message+nextind,start);
	// 		nextind+= start;}
	// 	else
	// 	{

	// 		//copy = copy.substr(rem,17-rem) + message.substr(nextind,rem);
	// 		strcpy(copy,copy+rem);
	// 		strncat(copy,message+nextind,rem);
	// 		nextind = messlen;
	// 	}

	// 	if(copy[0]=='0')
	// 	{
	// 		strcpy(copy,copy+1);
	// 		strncat(copy,message+nextind,1);
	// 	}
	// }

	while(nextind<messlen)
	{
		if(copy[0]=='0')
		{
			strcpy(copy,copy+1);
			copy[16]=message[nextind++];
			continue;
		}

		for(int i=0; i<17; i++)
			copy[i]=((copy[i]-'0')^(poly[i]-'0'))+'0';
	}

	if(copy[0]=='1')
	for(int i=0; i<17; i++)
			copy[i]=((copy[i]-'0')^(poly[i]-'0'))+'0';

	char remainder[30];
	strcpy(remainder,copy+1);
	//printf("%s\n",remainder);
	remainder[16]='\0';
	if(strcmp(remainder,"0000000000000000")==0)
	{
		printf("The message is free from error,sending confirmation to client\n");
		printf("The remainder from crc operation is: %s\n",remainder);
		char state[30]="message correctly received.\0";

		sendto(sockfd, (const char *)state, strlen(state),  
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
            len); 

	}

	//string input = message.substr(0,messlen-16);
	char input[400];
	memcpy(input,message,messlen-16);
	input[messlen-16]='\0';
	printf("The message extracted from message+crc is : %s\n",input);
	char asci[50];
	int ascilen=0;

	for(int i=0; i<strlen(input); i+=8)
	{

		int asci_val=0;
		for(int j=0; j<8; j++)
			asci_val+=(input[i+j]-'0')*pow(2,7-j);
		
		//printf("%d %c\n",asci_val,(char)asci_val);
		//asci += (char)asci_val;
		asci[ascilen] = (char)asci_val;
		ascilen++;		
	}

	asci[ascilen]='\0';

	printf("The message decoded from binary to ascii is : %s\n",asci);
	
} 
