/****************************************************************************
*Author : Gal Dagan
*Date : 26.09.21
*Reviewer : Liel Sananes
*
*Description : Implementation of DHCP module.
*
*Lab : ol108
****************************************************************************/
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "dhcp.h"

#define UNUSED(x) (void)(x)

int main(void)
{
	ip_byte_t network_ip = {192, 168, 4, 16};
	unsigned int netmask = 0xfffffff0; /* Netmask = 28 */
	ip_byte_t req_ip = {192, 168, 4, 31};
	ip_byte_t wrong_ip = {255, 168, 4, 0};
	ip_byte_t ret_ip = {0, 0, 0, 0};
	dhcp_t *dhcp = DHCPCreate(network_ip, netmask);
	
	printf("CountFree: %lu\n", DHCPCountFree(dhcp));
	
	DHCPAllocateIP(dhcp, ret_ip, req_ip);
	printf("%u.%u.%u.%u\n", ret_ip[0], ret_ip[1], ret_ip[2], ret_ip[3]); /* xxx.17 */
	
	printf("CountFree: %lu\n", DHCPCountFree(dhcp));
	
	DHCPFreeIP(dhcp, ret_ip);
	printf("CountFree: %lu\n", DHCPCountFree(dhcp));
	
	assert(DHCP_WRONG_NETWORK_IP == DHCPAllocateIP(dhcp, ret_ip, wrong_ip));
	
	req_ip[3] = 19;
	DHCPAllocateIP(dhcp, ret_ip, req_ip);
	printf("%u.%u.%u.%u\n", ret_ip[0], ret_ip[1], ret_ip[2], ret_ip[3]); /* xxx.19 */
	printf("CountFree: %lu\n", DHCPCountFree(dhcp));

	/* test for taken ip */
	DHCPAllocateIP(dhcp, ret_ip, req_ip);
	printf("%u.%u.%u.%u\n", ret_ip[0], ret_ip[1], ret_ip[2], ret_ip[3]); /* xxx.20 */
	printf("CountFree: %lu\n", DHCPCountFree(dhcp));

	DHCPAllocateIP(dhcp, ret_ip, NULL);
	printf("%u.%u.%u.%u\n", ret_ip[0], ret_ip[1], ret_ip[2], ret_ip[3]); /* xxx.18 */
	printf("CountFree: %lu\n", DHCPCountFree(dhcp));

	DHCPDestroy(dhcp);

	return (0);
}



