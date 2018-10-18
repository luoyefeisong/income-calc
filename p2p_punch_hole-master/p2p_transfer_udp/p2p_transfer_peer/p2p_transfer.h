#ifndef __P2P_TRANSFER_H__
#define __P2P_TRANSFER_H__

#include <stdint.h>
#include <malloc.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

//#define P2P_TRANSFER_SERVER_DOMAIN	(t05b022.p2cdn.com)
#if 1
#define P2P_TRANSFER_SERVER_IP		"0.0.0.0"
#else
#define P2P_TRANSFER_SERVER_IP		"58.220.12.22"
#endif

#define P2P_TRANSFER_SERVER_PORT	(7172)

#define P2P_TRANSFER_SERVER_IP_NUM	(2)

#define P2P_TRANSFER_MAX_MSG_LENGTH	(128)

//必须与nat_probe.h中的定义保持一致
enum
{
	NP_UNKNOWN					= 0,	/**< 未知网络类型. */
	NP_PUBLIC_NETWORK,					/**< 公有网络. */
	NP_FULL_CONE_NAT,					/**< 全锥型NAT. */
	NP_RESTRICTED_CONE_NAT,				/**< 限制型NAT. */
	NP_PORT_RESTRICTED_CONE_NAT,		/**< 端口限制型NAT. */
	NP_SYMMETRIC_NAT,					/**< 对称型NAT. */
};

enum
{
	P2P_TRANSFER_UNKNOWN_CMD = 0,
	P2P_TRANSFER_PING,
	P2P_TRANSFER_QUERY_DEVICE_INFO_REQUEST,
	P2P_TRANSFER_QUERY_DEVICE_INFO_RESPONSE,
	P2P_TRANSFER_PUNCH_HOLE,
	P2P_TRANSFER_PUNCH_HOLE_TO_PEER,
	P2P_TRANSFER_USER_DATA,
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
	uint32_t        cmd_len;		//数据长度
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


#define SAFE_RECVFROM(sock, buf, buf_len, src_addr) \
	do \
	{ \
		socklen_t addr_len_ = sizeof(struct sockaddr); \
		socklen_t *paddr_len_ = &addr_len_; \
		if (-1 == recvfrom(sock, buf, buf_len, 0, (struct sockaddr *)src_addr, paddr_len_)) \
		{ \
			XL_DEBUG(EN_PRINT_ERROR, "call recvfrom() failed, err: %s", strerror(errno)); \
			if (errno == EINTR) \
			{ \
				continue; \
			} \
			goto ERR; \
		} \
	} while (0)

#define SAFE_SENDTO(sock, buf, buf_len, src_addr) \
	do \
	{ \
		socklen_t addr_len_ = sizeof(struct sockaddr); \
		if (-1 == sendto(sock, buf, buf_len, 0, (const struct sockaddr *)src_addr, addr_len_)) \
		{ \
			XL_DEBUG(EN_PRINT_ERROR, "call sendto() failed, err: %s", strerror(errno)); \
			if (errno == EINTR) \
			{ \
				continue; \
			} \
			goto ERR; \
		} \
	} while (0)


/**
 * 获取字符串格式的网络类型
 * @param network_type	网络类型
 * 
 */
const char *get_string_network_type(int network_type);

int set_sock_opt(int sock, int flag);

void generate_server_addr(struct sockaddr_in *paddr);

void generate_peer_addr(struct sockaddr_in *paddr, const struct p2p_msg_device_info_t *pdevice_info);

const char *get_string_cmd(int cmd);

#ifdef __cplusplus
};
#endif
#endif
