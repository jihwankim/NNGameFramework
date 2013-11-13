
#include "PacketHandler.h"
#include <stdio.h>
#include <windows.h>
#include <assert.h>

LoginHandler::LoginHandler()
	: m_LoginCheck(false), m_NewLoginCheck(false)
{

}
LoginHandler::~LoginHandler()
{

}
void LoginHandler::HandlingPacket( short packetType, NNCircularBuffer* circularBuffer, NNPacketHeader* header )
{
	switch ( packetType )
	{
	case PKT_SC_LOGIN:
		{
			if ( circularBuffer->Read((char*)&m_LoginResultPacket, header->m_Size) )
			{
				// ��Ŷó��
				if ( m_LoginResultPacket.m_PlayerId == -1  )
				{
					/// ���� �ɸ��� �α��� ���д�.
					PostQuitMessage(-1) ;
				}

				m_LoginCheck = true;

				printf("LOGIN SUCCESS ClientId[%d] \n", m_LoginResultPacket.m_PlayerId) ;
			}
			else
			{
				assert(false) ;
			}
		}
		break;
	case PKT_SC_LOGIN_BROADCAST:
		{
			if ( circularBuffer->Read((char*)&m_LoginBroadcastResultPacket, header->m_Size) )
			{
				// ��Ŷó��
				if ( m_LoginBroadcastResultPacket.m_PlayerId == -1  )
				{
					/// ���� �ɸ��� �α��� ���д�.
					PostQuitMessage(-1) ;
				}

				m_LoginCheck = true;

				printf("NEW LOGIN SUCCESS ClientId[%d] \n", m_LoginBroadcastResultPacket.m_PlayerId) ;
			}
			else
			{
				assert(false) ;
			}
		}
		break;
	}
}
