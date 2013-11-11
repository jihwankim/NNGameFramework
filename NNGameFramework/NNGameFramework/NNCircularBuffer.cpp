#include "NNCircularBuffer.h"
#include <assert.h>
#include <memory>


bool NNCircularBuffer::Peek(char* destbuf, size_t bytes) const
{
	assert( m_Buffer != nullptr ) ;

	if( m_ARegionSize + m_BRegionSize < bytes )
		return false ;

	size_t cnt = bytes ;
	size_t aRead = 0 ;

	/// A, B ���� �Ѵ� �����Ͱ� �ִ� ���� A���� �д´�
	if ( m_ARegionSize > 0 )
	{
		aRead = (cnt > m_ARegionSize) ? m_ARegionSize : cnt ;
		memcpy(destbuf, m_ARegionPointer, aRead) ;
		cnt -= aRead ;
	}

	/// �б� �䱸�� �����Ͱ� �� �ִٸ� B �������� �д´�
	if ( cnt > 0 && m_BRegionSize > 0 )
	{
		assert(cnt <= m_BRegionSize) ;

		/// ������ ���� �� �б�
		size_t bRead = cnt ;

		memcpy(destbuf+aRead, m_BRegionPointer, bRead) ;
		cnt -= bRead ;
	}

	assert( cnt == 0 ) ;

	return true ;

}

bool NNCircularBuffer::Read(char* destbuf, size_t bytes)
{
	assert( m_Buffer != nullptr ) ;

	if( m_ARegionSize + m_BRegionSize < bytes )
		return false ;

	size_t cnt = bytes ;
	size_t aRead = 0 ;


	/// A, B ���� �Ѵ� �����Ͱ� �ִ� ���� A���� �д´�
	if ( m_ARegionSize > 0 )
	{
		aRead = (cnt > m_ARegionSize) ? m_ARegionSize : cnt ;
		memcpy(destbuf, m_ARegionPointer, aRead) ;
		m_ARegionSize -= aRead ;
		m_ARegionPointer += aRead ;
		cnt -= aRead ;
	}
	
	/// �б� �䱸�� �����Ͱ� �� �ִٸ� B �������� �д´�
	if ( cnt > 0 && m_BRegionSize > 0 )
	{
		assert(cnt <= m_BRegionSize) ;

		/// ������ ���� �� �б�
		size_t bRead = cnt ;

		memcpy(destbuf+aRead, m_BRegionPointer, bRead) ;
		m_BRegionSize -= bRead ;
		m_BRegionPointer += bRead ;
		cnt -= bRead ;
	}

	assert( cnt == 0 ) ;

	/// A ���۰� ����ٸ� B���۸� �� ������ ���� A ���۷� ���� 
	if ( m_ARegionSize == 0 )
	{
		if ( m_BRegionSize > 0 )
		{
			if ( m_BRegionPointer != m_Buffer )
				memmove(m_Buffer, m_BRegionPointer, m_BRegionSize) ;

			m_ARegionPointer = m_Buffer ;
			m_ARegionSize = m_BRegionSize ;
			m_BRegionPointer = nullptr ;
			m_BRegionSize = 0 ;
		}
		else
		{
			/// B�� �ƹ��͵� ���� ��� �׳� A�� ����ġ
			m_BRegionPointer = nullptr ;
			m_BRegionSize = 0 ;
			m_ARegionPointer = m_Buffer ;
			m_ARegionSize = 0 ;
		}
	}

	return true ;
}




bool NNCircularBuffer::Write(const char* data, size_t bytes)
{
	assert( m_Buffer != nullptr ) ;

	/// Read�� �ݴ�� B�� �ִٸ� B������ ���� ����
	if( m_BRegionPointer != nullptr )
	{
		if ( GetBFreeSpace() < bytes )
			return false ;

		memcpy(m_BRegionPointer + m_BRegionSize, data, bytes) ;
		m_BRegionSize += bytes ;

		return true ;
	}

	/// A�������� �ٸ� ������ �뷮�� �� Ŭ ��� �� ������ B�� �����ϰ� ���
	if ( GetAFreeSpace() < GetSpaceBeforeA() )
	{
		AllocateB() ;

		if ( GetBFreeSpace() < bytes )
			return false ;

		memcpy(m_BRegionPointer + m_BRegionSize, data, bytes) ;
		m_BRegionSize += bytes ;

		return true ;
	}
	/// A������ �� ũ�� �翬�� A�� ����
	else
	{
		if ( GetAFreeSpace() < bytes )
			return false ;

		memcpy(m_ARegionPointer + m_ARegionSize, data, bytes) ;
		m_ARegionSize += bytes ;

		return true ;
	}
}



void NNCircularBuffer::Remove(size_t len)
{
	size_t cnt = len ;
	
	/// Read�� ���������� A�� �ִٸ� A�������� ���� ����

	if ( m_ARegionSize > 0 )
	{
		size_t aRemove = (cnt > m_ARegionSize) ? m_ARegionSize : cnt ;
		m_ARegionSize -= aRemove ;
		m_ARegionPointer += aRemove ;
		cnt -= aRemove ;
	}

	// ������ �뷮�� �� ������� B���� ���� 
	if ( cnt > 0 && m_BRegionSize > 0 )
	{
		size_t bRemove = (cnt > m_BRegionSize) ? m_BRegionSize : cnt ;
		m_BRegionSize -= bRemove ;
		m_BRegionPointer += bRemove ;
		cnt -= bRemove ;
	}

	/// A������ ������� B�� A�� ����ġ 
	if ( m_ARegionSize == 0 )
	{
		if ( m_BRegionSize > 0 )
		{
			/// ������ ��� ���̱�
			if ( m_BRegionPointer != m_Buffer )
				memmove(m_Buffer, m_BRegionPointer, m_BRegionSize) ;
	
			m_ARegionPointer = m_Buffer ;
			m_ARegionSize = m_BRegionSize ;
			m_BRegionPointer = nullptr ;
			m_BRegionSize = 0 ;
		}
		else
		{
			m_BRegionPointer = nullptr ;
			m_BRegionSize = 0 ;
			m_ARegionPointer = m_Buffer ;
			m_ARegionSize = 0 ;
		}
	}
}


