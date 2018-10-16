#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

int main(int argc, char** argv) {
    init(HOST, PORT);
    unsigned char sum[255];
	unsigned char sum2[255];
    msg t,b,c;
	int lensum;
    MINIKERMIT ker,k,kerm;
    dateker date;
    Send s;
    s.MAXL=MAXL;
    s.TIME=TIME;
    s.NPAD=NPAD;
    s.PADC=PADC;
    s.EOL=EOL;
    s.QCTL=QCTL;
    s.QBIN=QBIN;
    s.CHKT=CHKT;
    s.REPT=REPT;
    s.CAPA=CAPA;
    s.R=R;
    ker.SOH=SOH;
    ker.SEQ=SEQ;
 int nr=0;
int nr2=0;  
int nrs=0; 
int nrx=0;
int nr1=0;
int nrend=0;
	if (ker.SEQ==0)                                           // pentru trimiterea sendinitului pornesc cu nr de secventa 0,completez campurile din secventa cu datele corespunzatoare
		{
		for (int i=0;i<sizeof ker.DATA;i++) 		 // calculez suma de control si completez in structura de tio kermit pe care o copiez in payload trimitand mesajul la receptor
			 ker.DATA[i]=0;	
	  	 ker.TYPE='S';						
	  	 memcpy(ker.DATA,&s,sizeof s);
	         ker.LEN=(sizeof ker);
		 ker.LEN=ker.LEN-2;
	         memcpy(sum,&ker,sizeof ker);
	         int x=sizeof ker;
	         x=x-3;
	         unsigned short crc = crc16_ccitt(sum,x);
	         memcpy(ker.CHECK,&crc,2);
	         ker.MARK=MARK;
	         memcpy(t.payload,&ker,sizeof ker);
	         t.len=sizeof ker;
		 send_message(&t);
	while (1)
{
	msg *y= receive_message_timeout(5000);			// astept confirmarea receptoruului, daca nu vine retrimit mesajul cu aceeasi parametrii si incrementez contorul cu care verific	
	if (y== NULL)
		{ 						// daca acelasi pachet are timeout de 3 ori ies din program
		if (nr2>2)					
		return 0;
		nr2++;
		memcpy(t.payload,&ker,sizeof ker);
	  	t.len=sizeof ker;
		send_message(&t);
		
	}
	else  if (y->payload[3]=='Y')				// daca am primit confirmarea ies din bucla si trec la urmatorul pachet
	{	ker.SEQ++;
		ker.SEQ=ker.SEQ%64;
		printf("%s","trimis init");
		printf("\n");
		break;	}
	else if (y->payload[3]=='N')
		{						// la primirea confirmarii ca pachetul a ajuns cu erori retrimit marind nr de secventa,calculand iarasi suma de control
			ker.SEQ++;
			ker.SEQ=ker.SEQ%64;
			int x=sizeof ker;
			x=x-3;
			memcpy(sum,&ker,sizeof ker);
	 	        unsigned short crc = crc16_ccitt(sum,x);
	  	        memcpy(ker.CHECK,&crc,2);
			memcpy(t.payload,&ker,sizeof ker);
	   		t.len=sizeof ker;
			printf("%s","NAK init");
			send_message(&t);		
	}	
}
}
for (int m = 1; m < argc; m++)					// pentru nr de fisiere primite trimit datele si titlurile fisierelor
{
	nrx=0;
	nr=0;							
	for (int i=0;i<sizeof ker.DATA;i++)
		ker.DATA[i]=0;
	for (int i=0;i<sizeof k.DATA;i++)
		k.DATA[i]=0;
		strcpy(ker.DATA, argv[m]); 
		ker.TYPE='F';
		 memcpy(sum2,&ker,sizeof ker);
		int x=sizeof ker;
		x=x-3;
		unsigned short crc2 = crc16_ccitt(sum2,x);
		memcpy(ker.CHECK,&crc2,2);
		memcpy(t.payload,&ker,sizeof ker);
	        t.len=sizeof ker;
		send_message(&t);
		
	
		
			

while(1)	
{		
msg *y= receive_message_timeout(5000);
if (y == NULL)							// trimit mesajele cu titlurile si astept confirmarile
{ 
perror("nimic send header");
if (nr>2)
	return 0;
	nr++;
	memcpy(t.payload,&ker,sizeof ker);
	t.len=sizeof ker;
	send_message(&t);
}

else  if (y->payload[3]=='N')  {
	ker.SEQ++;
	ker.SEQ=ker.SEQ%64;
	memcpy(sum2,&ker,sizeof ker);
	int x=sizeof ker;
	x=x-3;
	unsigned short crc2 = crc16_ccitt(sum2,x);
	memcpy(ker.CHECK,&crc2,2);
	memcpy(t.payload,&ker,sizeof ker);
        t.len=sizeof ker;
	send_message(&t);
	printf("%s","NAK header");	 
}
else if (y->payload[3]=='Y')

{	
	ker.SEQ++;
	ker.SEQ=ker.SEQ%64;
	nr=0;
	printf("%s","end header");	
	break;
	
       
} 

}	
 FILE *f = fopen(argv[m], "rb");			// deschid cate un fisier din care citesc maxim MAXL date in campul data din structura
printf("\n");
printf("%s",ker.DATA);
printf("\n");	
while(1)
{
	
 int chunk = fread(k.DATA , 1, 250 , f); 		
printf("\t");
printf("%d",chunk);
printf("\t");
if (chunk != 0) 					// cat timp mai am date in fisier trimit si astept confirmarile
{
	ker.TYPE='D';
	ker.LEN=chunk;					
	memcpy(ker.DATA,&k.DATA,sizeof k.DATA);
	memcpy(sum2,&ker,sizeof ker);
	int x=sizeof ker;
	x=x-3;
	unsigned short crc2 = crc16_ccitt(sum2,x);
	memcpy(ker.CHECK,&crc2,2);
	memcpy(t.payload,&ker,sizeof ker);
	t.len=sizeof ker;
	send_message(&t);
	}

else
{  for (int i=0;i<sizeof ker.DATA;i++)
	{						// cand nu mai sunt date trimit pachetul EOF si ies din bucla
	ker.DATA[i]=0;
	}
	nrend++;					// totodata incrementez de fiecare data cand termin de trimis un fisier pt ca atunci cand nr de argumente este argc-1 trimit pachetul EOT si ies
	ker.TYPE='Z';
	memcpy(sum2,&ker,sizeof ker);
	int x=sizeof ker;
	x=x-3;
	unsigned short crc2 = crc16_ccitt(sum2,x);
	memcpy(ker.CHECK,&crc2,2);
	memcpy(t.payload,&ker,sizeof ker);
	t.len=sizeof ker;
	send_message(&t);
	while(1)	
{		
msg *y= receive_message_timeout(5000);
if (y == NULL)
{ 
 perror("nimic la send");
if (nrx>2)
	return 0;
	nrx++;
	memcpy(t.payload,&ker,sizeof ker);
	t.len=sizeof ker;
	send_message(&t);

}


else  if (y->payload[3]=='N')  {
		ker.SEQ++;
		ker.SEQ=ker.SEQ%64;
		memcpy(sum2,&ker,sizeof ker);
		int x=sizeof ker;
		x=x-3;
		unsigned short crc2 = crc16_ccitt(sum2,x);
		memcpy(ker.CHECK,&crc2,2);
		memcpy(t.payload,&ker,sizeof ker);
	        t.len=sizeof ker;
		send_message(&t);
		printf("%s","NAK");
		printf("\n");
	 
}
else if (y->payload[3]=='Y')
{	
	ker.SEQ++;
	ker.SEQ=ker.SEQ%64;	
	nrx=0;
	printf("%s","end file");
	printf("\n");
	break;
	
       
} 

}
	fclose(f);	
	break;
}
 
printf("\t");
printf("%d",chunk);
printf("\t");
if(chunk != 0)
while(1)	
{		
msg *y= receive_message_timeout(5000);
if (y == NULL)
{ 
perror("nimic la send");
if (nrx>2)
	return 0;
	nrx++;
	memcpy(t.payload,&ker,sizeof ker);
	t.len=sizeof ker;
	send_message(&t);
}
else  if (y->payload[3]=='N')  {
   	
		ker.SEQ++;
		ker.SEQ=ker.SEQ%64;
		memcpy(sum2,&ker,sizeof ker);
		int x=sizeof ker;
		x=x-3;
		unsigned short crc2 = crc16_ccitt(sum2,x);
		memcpy(ker.CHECK,&crc2,2);
		memcpy(t.payload,&ker,sizeof ker);
	        t.len=sizeof ker;
		send_message(&t);
		printf("%s","NAK");
		printf("\n");	 
}
else if (y->payload[3]=='Y')
{	
	ker.SEQ++;
	ker.SEQ=ker.SEQ%64;
	nrx=0;
	printf("%s","pachet trimis");
	printf("\n");
	break;
} 

}	
}
if ( nrend==argc-1)
{
for (int i=0;i<sizeof ker.DATA;i++)
	ker.DATA[i]=0;  
	ker.TYPE='B';
	memcpy(sum2,&ker,sizeof ker);
	int x=sizeof ker;
	x=x-3;
	unsigned short crc2 = crc16_ccitt(sum2,x);
	memcpy(ker.CHECK,&crc2,2);
	memcpy(t.payload,&ker,sizeof ker);
	t.len=sizeof ker;
	send_message(&t);
	while(1)	
{		
msg *y= receive_message_timeout(5000);
if (y == NULL)
{ 
	perror(" nimic la send");
if (nr>2)
	return 0;
	nr++;
	memcpy(t.payload,&ker,sizeof ker);
	t.len=sizeof ker;
	send_message(&t);
}
else  if (y->payload[3]=='N')  {
	ker.SEQ++;
	ker.SEQ=ker.SEQ%64;
	memcpy(sum2,&ker,sizeof ker);
	int x=sizeof ker;
	x=x-3;
	unsigned short crc2 = crc16_ccitt(sum2,x);
	memcpy(ker.CHECK,&crc2,2);
	memcpy(t.payload,&ker,sizeof ker);
	t.len=sizeof ker;
	send_message(&t);
	printf("%s","NAK");
	printf("\n");	 
}
else if (y->payload[3]=='Y')
{	
	ker.SEQ++;
	ker.SEQ=ker.SEQ%64;
	nr=0;
	printf("%s","EOT");
	printf("\n");
	return 0;      
} 
}
}	
} 
	
    return 0;
}
