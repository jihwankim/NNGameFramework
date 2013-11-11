#pragma once


class NNCircularBuffer
{
public:

	NNCircularBuffer(size_t capacity) : m_BRegionPointer(nullptr), m_ARegionSize(0), m_BRegionSize(0)
	{
		m_Buffer = new char[capacity] ;
		m_BufferEnd = m_Buffer + capacity ;
		m_ARegionPointer = m_Buffer ;
	}

	~NNCircularBuffer()
	{
		delete [] m_Buffer ;
	}

	bool Peek(char* destbuf, size_t bytes) const ;
	bool Read(char* destbuf, size_t bytes) ;
	bool Write(const char* data, size_t bytes) ;


	/// ������ ù�κ� len��ŭ ������
	void Remove(size_t len) ;

	size_t GetFreeSpaceSize()
	{
		if ( m_BRegionPointer != nullptr )
			return GetBFreeSpace() ;
		else
		{
			/// A ���ۺ��� �� ���� �����ϸ�, B ���۷� ����ġ
			if ( GetAFreeSpace() < GetSpaceBeforeA() )
			{
				AllocateB() ;
				return GetBFreeSpace() ;
			}
			else
				return GetAFreeSpace() ;
		}
	}

	size_t GetStoredSize() const
	{
		return m_ARegionSize + m_BRegionSize ;
	}

	size_t GetContiguiousBytes() const 
	{
		if ( m_ARegionSize > 0 )
			return m_ARegionSize ;
		else
			return m_BRegionSize ;
	}

	/// ���Ⱑ ������ ��ġ (������ ���κ�) ��ȯ
	void* GetBuffer() const
	{
		if( m_BRegionPointer != nullptr )
			return m_BRegionPointer + m_BRegionSize ;
		else
			return m_ARegionPointer + m_ARegionSize ;
	}

	


	/// Ŀ��(aka. IncrementWritten)
	void Commit(size_t len)
	{
		if ( m_BRegionPointer != nullptr )
			m_BRegionSize += len ;
		else
			m_ARegionSize += len ;
	}

	/// ������ ù�κ� ����
	void* GetBufferStart() const
	{
		if ( m_ARegionSize > 0 )
			return m_ARegionPointer ;
		else
			return m_BRegionPointer ;
	}


private:

	void AllocateB()
	{
		m_BRegionPointer = m_Buffer ;
	}

	size_t GetAFreeSpace() const
	{ 
		return (m_BufferEnd - m_ARegionPointer - m_ARegionSize) ; 
	}

	size_t GetSpaceBeforeA() const
	{ 
		return (m_ARegionPointer - m_Buffer) ; 
	}


	size_t GetBFreeSpace() const
	{ 
		if (m_BRegionPointer == nullptr)
			return 0 ; 

		return (m_ARegionPointer - m_BRegionPointer - m_BRegionSize) ; 
	}

private:

	char*	m_Buffer ;
	char*	m_BufferEnd ;

	char*	m_ARegionPointer ;
	size_t	m_ARegionSize ;

	char*	m_BRegionPointer ;
	size_t	m_BRegionSize ;

} ;
