/****************************************************************************
*Author : Gal Dagan
*Date : 26.09.21
*Reviewer : Liel Sananes
*
*Description : Implementation of DHCP module.
*
*Lab : ol108
****************************************************************************/

#include <stdlib.h>	/* malloc(), free() */
#include <assert.h> /* assert() */

#include "dhcp.h"

#define WORD_SIZE sizeof(size_t)
#define LEFT_CHILD(x) (((x) * 2) + 1)
#define RIGHT_CHILD(x) (((x) * 2) + 2)
#define PARENT(x) ( ((x) - 1) / 2 )
#define ON 1
#define ROOT_INDEX 0

typedef unsigned char *trie_t;
typedef unsigned int uint;

struct dhcp
{
	trie_t trie;
    uint network_ip;
	uint netmask;
};

typedef enum taken_status
{
    EMPTY = 0,
    FULL
} state_t;

/****************************************************************************
 *                        Static Function Decleration                       *
 ****************************************************************************/
static size_t GetTrieSize(uint netmask);
static size_t GetTrieHeight(uint netmask);
static uint GetUIntNetIP(const ip_byte_t network_ip);
static void AllocFixedIP(dhcp_t *dhcp);
static void RecAllocSubnetIP(dhcp_t *dhcp, uint *subnet_id, 
                size_t index, size_t trie_height);
static void JumpToNextFree(dhcp_t *dhcp, size_t curr_index, size_t taken_index,
                size_t curr_height, uint *subnet_id);
static void GetIPFromUint(ip_byte_t returned_ip, uint result_ip); 
static size_t InternalCountFree(const dhcp_t *dhcp);
static void RecFreeIP(dhcp_t *dhcp, uint to_free, size_t index, size_t trie_height);
static size_t GetNextIndex(size_t curr_index, uint bit);
static void UpdateParentStatus(dhcp_t *dhcp, size_t index);
static uint SetNRightMostBitsOff(uint num, size_t n);

/****************************************************************************/

dhcp_t *DHCPCreate(ip_byte_t network_ip, unsigned int netmask)
{
    size_t trie_size = 0;
    dhcp_t *new_dhcp = NULL;

    assert(NULL != network_ip);

    new_dhcp = (dhcp_t *)malloc(sizeof(dhcp_t));
    if (NULL == new_dhcp)
    {
        return NULL;
    }

    trie_size = GetTrieSize(netmask);
    new_dhcp->trie = (trie_t)calloc(trie_size, sizeof(char));
    if (NULL == new_dhcp->trie)
    {
        free(new_dhcp);
        new_dhcp = NULL;
        return NULL;
    }

    new_dhcp->netmask = netmask;
    new_dhcp->network_ip = GetUIntNetIP(network_ip);
    AllocFixedIP(new_dhcp);

    return new_dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp);

    free(dhcp->trie);
    dhcp->trie = NULL;
    free(dhcp);
    dhcp = NULL;
}

dhcp_status_t DHCPAllocateIP(dhcp_t *dhcp, ip_byte_t returned_ip, 
                            const ip_byte_t requested_ip)
{
    uint req_network_ip = 0;
    uint subnet_id = 0;

    assert(NULL != dhcp);
    assert(NULL != returned_ip);

    if (FULL == dhcp->trie[ROOT_INDEX])
    {
        return DHCP_NO_FREE_IP;
    }

    req_network_ip = (NULL == requested_ip)? 
                    (dhcp->network_ip) : (GetUIntNetIP(requested_ip));

    if (dhcp->network_ip != (req_network_ip & dhcp->netmask))
    {
        return DHCP_WRONG_NETWORK_IP;
    }
    
    subnet_id = req_network_ip & (~dhcp->netmask);
    RecAllocSubnetIP(dhcp, &subnet_id, ROOT_INDEX, GetTrieHeight(dhcp->netmask));
    GetIPFromUint(returned_ip, dhcp->network_ip | subnet_id);

    return DHCP_SUCCESS;
}

void DHCPFreeIP(dhcp_t *dhcp, ip_byte_t ip_to_free)
{
    uint req_ip = 0;
    uint subnet_id = 0;

    assert(NULL != dhcp);
    assert(NULL != ip_to_free);

    req_ip = GetUIntNetIP(ip_to_free);
    if (dhcp->network_ip == (dhcp->netmask & req_ip))
    {
        subnet_id = req_ip & (~dhcp->netmask);
        RecFreeIP(dhcp, subnet_id, ROOT_INDEX, GetTrieHeight(dhcp->netmask));
    }
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    assert(NULL != dhcp);

    return InternalCountFree(dhcp);
}

/****************************************************************************
 *                        Static Function                                   *
 ****************************************************************************/
static size_t GetTrieSize(uint netmask)
{
    size_t num_set_bits = GetTrieHeight(netmask);
    uint base = 2;

    return ((base << (num_set_bits )) - 1);
}

static size_t GetTrieHeight(uint netmask)
{
    uint comp_num = ~netmask;
    size_t num_set_bits = 0;

    for (;0 != comp_num; comp_num &= (comp_num - 1), ++num_set_bits)
    {
        /* empty loop */    
    }

    return num_set_bits;
}

static uint GetUIntNetIP(const ip_byte_t network_ip)
{
    int i = 0;
    uint ip_as_uint = 0;

    for (; i < IP_BYTE_SIZE; ++i)
    {
        ip_as_uint <<= WORD_SIZE;
        ip_as_uint |= ((uint)(network_ip[i]));
    }
    
    return ip_as_uint;
}

static void AllocFixedIP(dhcp_t *dhcp)
{
    size_t network_add_index = GetTrieSize(dhcp->netmask) - 1;
    size_t broadcast_add_index = network_add_index / 2;
    size_t server_add_index = broadcast_add_index + 1;
    
    dhcp->trie[network_add_index] = FULL;
    dhcp->trie[broadcast_add_index] = FULL;
    dhcp->trie[server_add_index] = FULL;
    UpdateParentStatus(dhcp, PARENT(broadcast_add_index));
}

static void RecAllocSubnetIP(dhcp_t *dhcp, uint *subnet_id, 
                size_t index, size_t trie_height)
{
    uint mask = 1;
    char bit = 0;
    size_t next_index = 0;

    if (0 == trie_height)
    {
        dhcp->trie[index] = FULL;

        return;
    }

    bit = mask & (*subnet_id >> (trie_height - 1));
    next_index = GetNextIndex(index, bit);
    if (EMPTY == dhcp->trie[next_index])
    {
        RecAllocSubnetIP(dhcp, subnet_id, next_index, trie_height - 1);
    }
    else
    {
        JumpToNextFree(dhcp, index, next_index, trie_height, subnet_id);
    }

    UpdateParentStatus(dhcp, index);
}

static void JumpToNextFree(dhcp_t *dhcp, size_t curr_index, size_t taken_index,
                            size_t curr_height, uint *subnet_id)
{
    uint mask = 1;
    uint comp_mask = ~(dhcp->netmask);

    if(curr_height > GetTrieHeight(dhcp->netmask))
    {
        *subnet_id = 0;
        RecAllocSubnetIP(dhcp, subnet_id, 0, GetTrieHeight(dhcp->netmask));
    }
    else if (taken_index == RIGHT_CHILD(curr_index))
    {
        *subnet_id |= (mask << (curr_height - 1));
        comp_mask = SetNRightMostBitsOff(comp_mask, curr_height - 1);
        *subnet_id &= comp_mask;
        RecAllocSubnetIP(dhcp, subnet_id, curr_index, curr_height);
    }
    else
    {
        JumpToNextFree(dhcp, PARENT(curr_index), curr_index, curr_height + 1,subnet_id);
    }
}                            

static void RecFreeIP(dhcp_t *dhcp, uint to_free, size_t index, size_t trie_height)
{
    uint mask = 1;
    char bit = 0;
    size_t next_index = 0;

    if (0 == trie_height)
    {
        dhcp->trie[index] = EMPTY;

        return;
    }

    dhcp->trie[index] = EMPTY;

    bit = mask & (to_free >> (trie_height - 1));
    next_index = GetNextIndex(index, bit);
    RecFreeIP(dhcp, to_free, next_index, trie_height - 1);

    UpdateParentStatus(dhcp, index);
}

static void GetIPFromUint(ip_byte_t returned_ip, uint result_ip)
{
    int i = IP_BYTE_SIZE - 1;
    uint mask = 0xff;

    for (;i >= 0; --i, result_ip >>= WORD_SIZE)
    {
        returned_ip[i] = result_ip & mask;
    }
}

static size_t InternalCountFree(const dhcp_t *dhcp)
{
    size_t trie_size = GetTrieSize(dhcp->netmask);
    size_t index = trie_size / 2;
    size_t counter = 0;

    for (; index < trie_size; ++index)
    {
        if (0 == dhcp->trie[index])
        {
            ++counter;
        }
    }

    return counter;
}

static size_t GetNextIndex(size_t curr_index, uint bit)
{
    return (ON == bit)? LEFT_CHILD(curr_index) : RIGHT_CHILD(curr_index);
}

static void UpdateParentStatus(dhcp_t *dhcp, size_t index)
{
    if (FULL == dhcp->trie[LEFT_CHILD(index)] &&
        FULL == dhcp->trie[RIGHT_CHILD(index)] )
    {
        dhcp->trie[index] = FULL;
    }
    else
    {
        dhcp->trie[index] = EMPTY;
    }
}

static uint SetNRightMostBitsOff(uint num, size_t n)
{
    size_t i = 0;

    for (; i < n; ++i)
    {
        uint mask = (uint)1 << i;
        num |= mask;
        num ^= mask;
    }

    return num;
}