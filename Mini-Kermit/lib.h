#ifndef LIB
#define LIB
const char SOH=0x01;
const char SEQ=0x00;
const unsigned char NPAD=0x00;
const char PADC=0x00;
const char EOL=0x0D;
const char QCTL=0x00;
const char QBIN=0x00;
const char CHKT=0x00;
const char REPT=0x00;
const char CAPA=0x00; 
const char R=0x00;
const char MARK=0x0D;
const unsigned char MAXL=250;
const char TIME=5;
//#pragma pack(1)
typedef struct __attribute__((packed)) {
    int len;
    char payload[1400];
} msg;
//#pragma pack(1)
typedef struct __attribute__((packed))  {
    char SOH;
    unsigned char LEN;
    char SEQ;
    char TYPE;
    unsigned char DATA[250];//MAXL
    char CHECK[2];
    char MARK;
	} MINIKERMIT;
//#pragma pack(1)
typedef struct __attribute__((packed)) {
	unsigned char MAXL;
	char TIME;
	unsigned char NPAD;
	char PADC;
	char EOL;
	} dateker;
//#pragma pack(1)
typedef struct __attribute__((packed))  {
	unsigned char MAXL;
	char TIME;
	unsigned char NPAD;
	char PADC;
	char EOL;
	char QCTL;
	char QBIN;
	char CHKT;
	char REPT;
	char CAPA;
	char R;
	//MINIKERMIT kermit; 
	} Send;
	 
void init(char* remote, int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);
int send_message(const msg* m);
int recv_message(msg* r);
msg* receive_message_timeout(int timeout); //timeout in milliseconds
unsigned short crc16_ccitt(const void *buf, int len);

#endif

