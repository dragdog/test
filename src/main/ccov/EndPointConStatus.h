#ifndef EndPointConStatus_H
#define EndPointConStatus_H

#define CON_STA 1
#define NO_CON_STA 0

typedef struct
{
	unsigned int ip;
	unsigned short port;
	unsigned char status;
}EndPointConStatus_Stru;

#endif