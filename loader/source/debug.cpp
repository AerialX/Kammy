#include "debug.h"

#include <stdarg.h>

#include <cell/sysmodule.h>
#include <netex/net.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG_IP "192.168.1.13"
#define DEBUG_PORT 1100

#define DEBUG
#ifdef DEBUG
static int SocketFD;
bool Debug_Connect()
{
	if (cellSysmoduleLoadModule(CELL_SYSMODULE_NET) < 0)
		return false;
	if (sys_net_initialize_network() < 0)
		return false;
	struct sockaddr_in stSockAddr;
	SocketFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&stSockAddr, 0, sizeof stSockAddr);

	stSockAddr.sin_family = AF_INET;
	stSockAddr.sin_port = htons(DEBUG_PORT);
	inet_pton(AF_INET, DEBUG_IP, &stSockAddr.sin_addr);

	connect(SocketFD, (struct sockaddr *)&stSockAddr, sizeof stSockAddr);
	return true;
}

void Debug_Disconnect()
{
	shutdown(SocketFD, SHUT_RDWR);
 
	close(SocketFD);
	
	sys_net_finalize_network();
	cellSysmoduleUnloadModule(CELL_SYSMODULE_NET);
}

void Debug_Print(const char* fmt, ...)
{
	char buffer[0x800];
	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, arg);
	va_end(arg);
	send(SocketFD, buffer, strlen(buffer), 0);
}
#else
bool Debug_Connect()
{
	return true;
}

void Debug_Print(const char* fmt, ...) { }

void Debug_Disconnect() { }
#endif
