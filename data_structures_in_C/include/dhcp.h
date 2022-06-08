/****************************************************************************
*Author : Gal Dagan
*Date : 26.09.21
*Reviewer : Liel Sananes
*
*Description : Implementation of DHCP module.
*
*Lab : ol108
****************************************************************************/
#ifndef __DHCP_H_OL108_ILRD__
#define __DHCP_H_OL108_ILRD__

#include <stddef.h> /* size_t */

#define IP_BYTE_SIZE 4

typedef enum dhcp_status
{
	DHCP_SUCCESS = 0,
	DHCP_NO_FREE_IP = 1,
	DHCP_WRONG_NETWORK_IP = 2
} dhcp_status_t;

typedef struct dhcp dhcp_t;
typedef unsigned char ip_byte_t[IP_BYTE_SIZE];

/**
 * @Description: Creates DHCP module. 
 * @Parameters: network_ip - An array of 4 numbers to be used as the network ip.
 * 				netmask - netmask.
 * @Return: dhcp_t pointer to the DHCP module. NULL, if creation has failed.
 * @Complexity: O(1)
 * @Note: Illegal netmask will cause undefined behavior.
 * @Note: Illegal netmask is any number that doesn't have contiguous ON bit followed
 * 			by contigous OFF bits. Netmask that is greater than 0xfffffffd 
 * 			(netmask of /31) will also cause undefined behaivor. 
**/
dhcp_t *DHCPCreate(ip_byte_t network_ip, unsigned int netmask);

/**
 * @Description: Destroys a given DHCP module. 
 * @Parameters: dhcp - pointer to DHCP module.
 * @Return: void.
 * @Complexity: O(1)
**/
void DHCPDestroy(dhcp_t *dhcp);

/**
 * @Description: Allocates requested IP address.
 * @Parameters: dhcp - pointer to DHCP module.
 * 				returned_ip - ip_byte_t array for the alloceted ip.
 * 				requested_ip - ip_byte_t array for ip address to be allocated.
 * @Return: dhcp_status_t of the allocation proccess.
 * @Complexity: O(n)
 * @Note: If requested_ip is taken, the next available address will be allocated.
**/
dhcp_status_t DHCPAllocateIP(dhcp_t *dhcp, ip_byte_t returned_ip, 
							const ip_byte_t requested_ip);

/**
 * @Description: Frees given IP address.
 * @Parameters: dhcp - pointer to DHCP module.
 * 				ip_to_free - ip_byte_t array for ip to be freed.
 * @Return: void.
 * @Complexity: O(log n)
**/
void DHCPFreeIP(dhcp_t *dhcp, ip_byte_t ip_to_free);

/**
 * @Description: Returns the number of available IPs in the current module.
 * @Parameters: dhcp - pointer to DHCP module.
 * @Return: Number of available IPs in the current DHCP module.
 * @Complexity: O(n)
**/
size_t DHCPCountFree(const dhcp_t *dhcp);

#endif /* __DHCP_H_OL108_ILRD__ */
