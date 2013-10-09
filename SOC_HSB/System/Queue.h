#pragma once

namespace SOC_System {

template<typename T>
T MakePowerOf2(T n)
{
	if (n < 2 || (n & (n - 1)) == 0)
	{
		return n;
	}

	int i = 0;
	for (; n > 0; (n = n >> 1), ++i);

	return (static_cast<T>(1) << i);
}
    
    // MAX_SIZE have to be power of 2.
template <typename T, unsigned int MAX_SIZE>
class MWSRQueue
{
private:
	volatile unsigned long m_first;
	volatile unsigned long m_size;
	unsigned long m_last;
	T*	m_pData[MAX_SIZE];

public:
	MWSRQueue()
		: m_first(0)
		, m_size(0)
		, m_last(0)
	{
#if defined(_WIN32) || defined(_WIN64)
		_ASSERT_EXPR(((MAX_SIZE & (MAX_SIZE - 1)) == 0), _T("MAX_SIZE is must be power of 2"));
#else
        assert(((MAX_SIZE & (MAX_SIZE - 1)) == 0));
#endif
		memset(m_pData, 0, sizeof(m_pData));
	}

	void Push(T* pData)
	{
		static Lock::CriticalSectionLock lock;

		unsigned long idx = 0;
		unsigned long next = 0;
#if defined(_WIN32) || defined(_WIN64)
		unsigned long size = InterlockedIncrement(&m_size);
#else
		unsigned long size = 0;
        {
            TYPED_SCOPE_LOCK(lock);
            ++m_size;
        }
#endif
		if (size > MAX_SIZE)
		{
			return;
		}

#if !defined(_WIN32) && !defined(_WIN64)
        unsigned long temp = 0;
#endif
		do
		{
			idx = m_first; // recheck or check
			next = (idx + 1) & (MAX_SIZE -1);
#if defined(_WIN32) || defined(_WIN64)
		} while(InterlockedCompareExchange(&m_first, next, idx) != idx);
#else
        {
            TYPED_SCOPE_LOCK(lock);
            temp = m_first;
            if (m_first == idx)
                m_first = next;
        }
        
		} while(temp != idx);
#endif
		if (idx < MAX_SIZE)
		{
			m_pData[idx] = pData;
		}
	}

	T* Pop(long* popIndex)
	{
		if (m_first == m_last)
			return nullptr;
		if (m_last >= MAX_SIZE)
			return nullptr;

		static Lock::CriticalSectionLock lock;

		T* pData = nullptr;
		while ((pData = m_pData[m_last]) == nullptr) // like a simple spin lock
		{
#if defined(_WIN32) || defined(_WIN64)
            Sleep(0);
#else
            sleep(0);
#endif
		}

		m_pData[m_last] = nullptr;
		if (popIndex != nullptr)
		{			
			*popIndex = m_last;
		}

		m_last = (++m_last) & (MAX_SIZE -1);
        
#if defined(_WIN32) || defined(_WIN64)
		InterlockedDecrement(&m_size);
#else
        {
            TYPED_SCOPE_LOCK(lock);
            --m_size;
        }
#endif
		return pData;
	}

public:
	bool IsEmpty() { return (m_size == 0); }
	long GetCurSize() { return m_size; }
};

}