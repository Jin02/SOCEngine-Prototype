#pragma once

namespace SOC_System {
	using namespace Lock;

	// need to Spin lock. current lock is slow.
class Counter
{
public:
	Counter() : m_count(0)
	{}
	Counter(SOC_LONG value) : m_count(value)
	{}
	virtual ~Counter() {}
	SOC_LONG Get() { return m_count; }

public:
	void operator = (SOC_LONG Value);
	SOC_LONG operator ++ () // prefix ++
	{
#if defined(_WIN64) || defined(_WIN32)
		return InterlockedIncrement((unsigned long long*)&m_count);
#else
		static CriticalSectionLock lock;
		{
			TYPED_SCOPE_LOCK(lock);
			++m_count;
		}

		return m_count;
#endif
	}
	SOC_LONG operator ++ (int) // postfix ++
	{
#if defined(_WIN64) || defined(_WIN32)
		return InterlockedIncrement((unsigned long long*)&m_count) -1;
#else
		static CriticalSectionLock lock;
		{
			TYPED_SCOPE_LOCK(lock);
			++m_count;
		}

		return m_count -1;
#endif
	}

	SOC_LONG operator -- ()
	{
#if defined(_WIN64) || defined(_WIN32)
		return InterlockedDecrement((unsigned long long*)&m_count);
#else
		static CriticalSectionLock lock;
		{
			TYPED_SCOPE_LOCK(lock);
			--m_count;
		}

		return m_count;
#endif
	}

	SOC_LONG operator -- (int)
	{
#if defined(_WIN64) || defined(_WIN32)
		return InterlockedDecrement((unsigned long long*)&m_count) +1;
#else
		static CriticalSectionLock lock;
		{
			TYPED_SCOPE_LOCK(lock);
			--m_count;
		}

		return m_count +1;
#endif
	}

	void operator += (SOC_LONG value)
	{
#if defined(_WIN64) || defined(_WIN32)
		InterlockedExchangeAdd((unsigned long long*)&m_count, value);
#else
		static CriticalSectionLock lock;
		{
			TYPED_SCOPE_LOCK(lock);
			m_count += value;
		}
#endif
	}

	void operator -= (SOC_LONG value)
	{
#if defined(_WIN64) || defined(_WIN32)
		InterlockedExchangeSubtract((unsigned long long*)&m_count, value);
#else
		static CriticalSectionLock lock;
		{
			TYPED_SCOPE_LOCK(lock);
			m_count -= value;
		}
#endif
	}

	bool operator != (SOC_LONG value) const { return (m_count != value); }
	operator SOC_LONG () const { return m_count; }

private:
	SOC_LONG m_count;
};

}