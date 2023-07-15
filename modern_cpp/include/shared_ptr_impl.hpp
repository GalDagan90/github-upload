#ifndef __SHARED_PTR_IMPL_H_RD5678_ILRD__
#define __SHARED_PTR_IMPL_H_RD5678_ILRD__

#include <atomic>

struct ControlBlock
{
	std::atomic<unsigned long> m_counter{0};
};


#endif