#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc, char** argv) {
init(HOST, PORT);
    msg r, t;
 MINIKERMIT ker;
    Send s;

    
int nrs=0;
int nr=0;
int nrt=0;

while (1)
{
msg *y = receive_message_timeout(15000);                                   // pentru sendinit astept 15 secunde si daca nu s au primit mesaje ies din program
if (y == NULL)
	return 0;							// calculez suma de control pentru mesajul primit si daca este aceeasi cu suma de control din payload trimit mesajul
int x=sizeof ker;
	x=x-3;								// de confirmare si ies din bucla
unsigned short crc3 = crc16_ccitt(y->payload, x);
char verif[2];
memcpy(verif,&crc3,2);
if (y->payload[254]==verif[0] && y->payload[255]==verif[1])	
	{	printf("%s","ACK");
		printf("\n");
		y->payload[3]='Y';
		memcpy(t.payload,&y->payload,sizeof y->payload);
		t.len=sizeof ker;
		send_message(&t);
		break;
	}	
	else if (y->payload[254]!=verif[0] || y->payload[255]!=verif[1])	// in cazul in care sumele nu corespund trimit mesajul de insttintare la emitator
		{
		
		printf("%s","NAK");
		printf("\n");
		y->payload[3]='N';
		for (int i=4;i<254;i++)
		y->payload[i]=0;
		memcpy(t.payload,&y->payload,sizeof y->payload);
		t.len=sizeof ker;
		send_message(&t);
		
}
}
	
while(1)
{
nr=0;
nrt=0;										// apoi cat timp mai primesc mesaje diferite de EOT
while (1)
{										// cand primesc fileheader prefixez numele corespunzator in campul DATA
	msg *y = receive_message_timeout(5000); 
	if (y == NULL )
	{
	 perror("nimic receive header");
	 if (nr>2)
	 return 0;
	 nr++;
    }
else
{
	memcpy(&ker,&y->payload,sizeof y->payload);	
	int x=sizeof ker;
	x=x-3;
    	unsigned short crc4 = crc16_ccitt(y->payload, x);
  	char verific[2];
	memcpy(verific,&crc4,2);
if (y->payload[254]==verific[0] && y->payload[255]==verific[1])
  {  	
	if(y->payload[3]=='B')
	{
	y->payload[3]='Y';
	printf("%s","terminat tot");
	for (int i=4;i<254;i++)
	y->payload[i]=0;
	memcpy(t.payload,&y->payload,sizeof y->payload);
	t.len=sizeof ker;
	send_message(&t);
	return 0;
	}  
	printf("%s","primit header");
	y->payload[3]='Y';
	memcpy(&ker,&y->payload,sizeof y->payload);
	for (int i=4;i<254;i++)
	y->payload[i]=0;
	memcpy(t.payload,&y->payload,sizeof y->payload);
	t.len=sizeof ker;
	send_message(&t);
	char a[10];
	char z[20] ="recv_";
	memcpy(a,&ker.DATA,10);
	nr=0;
	memcpy(ker.DATA,&z,sizeof z);
	strcat(ker.DATA,a);
	break; 		
}
 else if (y->payload[254]!=verific[0] || y->payload[255]!=verific[1])
{   
	printf("%s","Cu erori");
	y->payload[3]='N';
	for (int i=4;i<254;i++)
	y->payload[i]=0;
	memcpy(t.payload,&y->payload,sizeof y->payload);
	t.len=sizeof ker;
	send_message(&t);	
}
}
}
FILE *f = fopen(ker.DATA, "wb");					// deschid fisierul pentru scriere cu datele primite cand acestea sunt corecte trimitand mesajele corespunzatoare cand nu sunt
printf("\n");
printf("%s",ker.DATA);
printf("\n");
while (1)
{	
	msg *y = receive_message_timeout(5000); 
	if (y == NULL )							// contorizez cand am TIMEOUT si retrimit de maxim 3 ori
	{ 
	perror("nimic receive file");
	if (nrt>2)
	return 0;
	nrt++;
    }


else
{
	memcpy(&ker,&y->payload,sizeof y->payload);	 
	unsigned char crcint[2];
	char abc[2];
	int x=sizeof ker;
	x=x-3;
   	unsigned short crc4 = crc16_ccitt(y->payload, x);
  	char verific[2];
	memcpy(verific,&crc4,2);
  	if (y->payload[254]==verific[0] && y->payload[255]==verific[1])
{  	
	if(y->payload[3]=='Z')
	{
	  y->payload[3]='Y';							// cand primesc EOF ies din bucla pt fisier
	for (int i=4;i<254;i++)
		y->payload[i]=0;
	memcpy(t.payload,&y->payload,sizeof y->payload);
	t.len=sizeof ker;
	send_message(&t);
	printf("%s","end primit");
	printf("\n");
	nrt=0;
	break; 
	}
	else if(y->payload[3]=='D')
	{
	y->payload[3]='Y';
	memcpy(&ker,&y->payload,sizeof y->payload);				
	ker.LEN=(unsigned char) y->payload[1];					// scriu in fisier cu lungimea primita ca parametru si fac contorul pentru numararea retrimiterilor 0
	fwrite(ker.DATA ,1, ker.LEN  , f);
	for (int i=4;i<254;i++)
	y->payload[i]=0;
	printf("%s","primit");							
	printf("\n");
	memcpy(t.payload,&y->payload,sizeof y->payload);
	t.len=sizeof ker;
	nrt=0;
	send_message(&t);
}
	 
}
 else if (y->payload[254]!=verific[0] || y->payload[255]!=verific[1])
{       printf("%s","Cu erori");
	printf("\n");
	y->payload[3]='N';
	for (int i=4;i<254;i++)
	y->payload[i]=0;
	memcpy(t.payload,&y->payload,sizeof y->payload);
	t.len=sizeof ker;
	send_message(&t);	
}
}
}
}

return 0;
}
