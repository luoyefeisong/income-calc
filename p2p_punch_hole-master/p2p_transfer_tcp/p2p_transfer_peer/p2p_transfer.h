#ifndef __P2P_TRANSFER_H__
#define __P2P_TRANSFER_H__
#ifdef _WIN32

#include <windows.h>
#include <sys/timeb.h>
#include <process.h>
typedef HANDLE pthread_mutex_t;
typedef int pthread_condattr_t;
typedef HANDLE pthread_t;
typedef DWORD pthread_attr_t;

#define pthread_create(thrp, attr, func, arg)                               \
    (((*(thrp) = CreateThread(NULL, 0,                                     \
        (LPTHREAD_START_ROUTINE)(func), (arg), 0, NULL)) == NULL) ? -1 : 0)
#define pthread_join(thr, statusp)                                          \
    ((WaitForSingleObject((thr), INFINITE) == WAIT_OBJECT_0) &&            \
    ((statusp == NULL) ? 0 :                            \
    (GetExitCodeThread((thr), (LPDWORD)(statusp)) ? 0 : -1)))

#endif

#include <stdint.h>
#include <malloc.h>
#ifndef _WIN32
#include <arpa/inet.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

//#define P2P_TRANSFER_SERVER_DOMAIN	(t05b022.p2cdn.com)
#if 0
#define P2P_TRANSFER_SERVER_IP		"192.168.111.123"
#else
#define P2P_TRANSFER_SERVER_IP		"132.232.3.109"
#endif

#define P2P_TRANSFER_SERVER_PORT	(10000)

#define P2P_TRANSFER_SERVER_IP_NUM	(2)

//必须与nat_probe.h中的定义保持一致

enum {
	NP_UNKNOWN = 0,
	NP_PUBLIC_NETWORK,
	NP_FULL_CONE_NAT,
	NP_RESTRICTED_CONE_NAT,
	NP_PORT_RESTRICTED_CONE_NAT,
	NP_SYMMETRIC_NAT
};

enum 
{
	P2P_TRANSFER_UNKNOWN_CMD = 0,
	P2P_TRANSFER_PING,
	P2P_TRANSFER_QUERY_DEVICE_INFO_REQUEST,
	P2P_TRANSFER_QUERY_DEVICE_INFO_RESPONSE,
	P2P_TRANSFER_PUNCH_HOLE,
	P2P_TRANSFER_CLOSE
};

#define P2P_TRANSFER_MAGIC (0xbeefdead1234)

struct p2p_msg_ping_t
{
	uint64_t	device_sn;           //设备id
	int			network_type;       //网络类型
};

struct p2p_msg_device_info_t
{
	uint64_t			device_sn;
	struct in_addr		ip_addr;
	uint16_t			port;
	int					network_type;
};


struct p2p_msg_head_t
{
	uint64_t        magic;          //魔数校验码
	uint64_t        msgid;          //包id,TCP协议该字段无用
	uint64_t        src_device_sn;   //发送消息的设备sn
	uint64_t        dst_device_sn;   //发送消息的设备sn
	uint32_t        cmd_len;         //数据长度
	int             cmd;
	char            cmd_data[0];    //数据
};


#define UNUSED __attribute__((unused))

#define SAFE_CALLOC(type, ptr, size) \
	do \
	{ \
		ptr = (type)calloc(1, size); \
		if (NULL == ptr) \
		{ \
			XL_DEBUG(EN_PRINT_ERROR, "call calloc() failed, size: %d, err: %s", size, strerror(errno)); \
			goto ERR; \
		} \
	} \
	while(0)

#define SAFE_FREE(ptr) \
	do \
	{ \
		if (ptr != NULL) \
		{ \
			free(ptr); \
			ptr = NULL; \
		} \
	} \
	while(0)
#define SAFE_CLOSE(sock) \
	do \
	{ \
		if (sock > 0) \
		{ \
			close(sock); \
			sock = -1; \
		} \
	}while(0)

/**
 * 获取字符串格式的网络类型
 * @param network_type	网络类型
 * 
 */
const char *get_string_network_type(int network_type);

int set_sock_opt(int sock, int flag);

#ifdef __cplusplus
};
#endif
#endif
