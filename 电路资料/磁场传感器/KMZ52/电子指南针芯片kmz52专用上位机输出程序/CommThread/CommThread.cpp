#include "stdafx.h"
#include "CommThread.h"


//--- Ŭ���� ������
CCommThread::CCommThread()
{

	//--> �ʱ�� �翬��..��Ʈ�� ������ ���� ���¿��߰���?
	m_bConnected = FALSE;

}

CCommThread::~CCommThread()
{

}


// ��Ʈ sPortName�� dwBaud �ӵ��� ����.
// ThreadWatchComm �Լ����� ��Ʈ�� ���� ������ �� MainWnd�� �˸���
// ���� WM_COMM_READ�޽����� ������ ���� ���� wPortID���� ���� �޴´�.
BOOL CCommThread::OpenPort(CString sPortName, DWORD dwBaud, WORD wPortID)
{
	// Local ����.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	// ���� �ʱ�ȭ....?
	m_bConnected = FALSE;

	m_wPortID	= wPortID; // COM1-> 0, COM2->1,,,,,

	// overlapped structure ���� �ʱ�ȭ.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	//--> Read �̺�Ʈ ������ ����..
	if ( !(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) ) 	
	{
		return FALSE;
	}
	

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	//--> Write �̺�Ʈ ������ ����..
	if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}
	
	//--> ��Ʈ�� ����..
	m_sPortName = sPortName;

	//--> ��������...RS 232 ��Ʈ ����..
	m_hComm = CreateFile( m_sPortName, 
						  GENERIC_READ | GENERIC_WRITE, 0, NULL,
						  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
						  NULL);

	//--> ��Ʈ ���⿡ �����ϸ�..
	if (m_hComm == (HANDLE) -1)
	{
		return FALSE;
	}


	//===== ��Ʈ ���� ����. =====

	// EV_RXCHAR event ����...�����Ͱ� ������.. ���� �̺�Ʈ�� �߻��ϰԲ�..
	SetCommMask( m_hComm, EV_RXCHAR);	

	// InQueue, OutQueue ũ�� ����.
	SetupComm( m_hComm, 4096, 4096);	

	// ��Ʈ ����.
	PurgeComm( m_hComm,					
			   PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout ����.
	timeouts.ReadIntervalTimeout = 10;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	
	SetCommTimeouts( m_hComm, &timeouts);

	// dcb ����.... ��Ʈ�� ��������..��� ����ϴ� DCB ����ü�� ����..
	dcb.DCBlength = sizeof(DCB);

	//--> ���� ������ �� �߿���..
	GetCommState( m_hComm, &dcb);	
	
	//--> ���巹��Ʈ�� �ٲٰ�..
	dcb.BaudRate = dwBaud;
	
	//--> Data 8 Bit
	dcb.ByteSize = 8;

	//--> Noparity
	dcb.Parity = 0;

	//--> 1 Stop Bit
	dcb.StopBits = 0;


	//--> ��Ʈ�� ��..����������.. �����غ���..
	if( !SetCommState( m_hComm, &dcb) )	
	{
		return FALSE;
	}


	// ��Ʈ ���� ������ ����.
	m_bConnected = TRUE;

	//--> ��Ʈ ���� ������ ����.
	m_hThreadWatchComm = CreateThread( NULL, 0, 
									   (LPTHREAD_START_ROUTINE)ThreadWatchComm, 
									   this, 0, &dwThreadID);

	//--> ������ ������ �����ϸ�..
	if (! m_hThreadWatchComm)
	{
		//--> ���� ��Ʈ�� �ݰ�..
		ClosePort();
		return FALSE;
	}

	return TRUE;
}
	

BOOL CCommThread::bInitComPort( CString strPortName, 
							    DWORD dwBaud, 
							    BYTE byData, 
							    BYTE byStop, 
							    BYTE byParity )
{

	//--> ���� ��Ʈ�� ���� ������.. �ݰ� �� �����ϱ� ���ؼ�..
	if( m_bConnected )	
	{
		ClosePort();
	}

	// Local ����.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	// ���� �ʱ�ȭ
	m_bConnected = FALSE;
	m_wPortID	= (strPortName == "COM1")  ?  0 : 1; // COM1-> 0, COM2->1,,,,,

	// overlapped structure ���� �ʱ�ȭ.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	//--> Read �̺�Ʈ ������ ����..
	if( !(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) )	
	{
		return FALSE;
	}


	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	//--> Write �̺�Ʈ ������ ����..
	if( !(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) )
	{
		return FALSE;
	}
	
	// ��Ʈ ����
	m_sPortName = strPortName;

	//--> ��������...RS 232 ��Ʈ ����..
	m_hComm = CreateFile( m_sPortName, 
						  GENERIC_READ | GENERIC_WRITE, 0, NULL,
						  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
						  NULL);
	

	//--> ��Ʈ ���⿡ �����ϸ�..
	if (m_hComm == (HANDLE) -1) 
	{
		return FALSE;
	}

	// ��Ʈ ���� ����.

	// EV_RXCHAR event ����
	SetCommMask( m_hComm, EV_RXCHAR );	

	// InQueue, OutQueue ũ�� ����.
	SetupComm( m_hComm, 4096, 4096 );	

	// ��Ʈ ����.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout ����.
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	
	SetCommTimeouts( m_hComm, &timeouts);

	
	// dcb ����.... ��Ʈ�� ��������..��� ����ϴ� DCB ����ü�� ����..
	dcb.DCBlength = sizeof(DCB);

	//--> ���� ������ �� �߿���..
	GetCommState( m_hComm, &dcb);
	
	
	//--> ���ڿ� ���� ��Ʈ ����
	dcb.BaudRate = dwBaud;		//Baud Rate
	
	dcb.ByteSize = byData;		//Data Bit
	
	dcb.StopBits = byStop;		//Stop Bit
	
	dcb.Parity   = byParity;	//Parity


	
	//--> ��Ʈ�� ��..����������.. �����غ���..
	if( !SetCommState( m_hComm, &dcb) )	
	{
		return FALSE;
	}


	// ��Ʈ ���� ������ ����.
	m_bConnected = TRUE;
	
	//--> ��Ʈ ���� ������ ����.
	m_hThreadWatchComm = CreateThread( NULL, 0, 
									   (LPTHREAD_START_ROUTINE)ThreadWatchComm, this, 0, &dwThreadID);
	
	//--> ������ ������ �����ϸ�..
	if (! m_hThreadWatchComm)
	{
		//--> ���� ��Ʈ�� �ݰ�..
		ClosePort();
		return FALSE;
	}

	return TRUE;
}





// ��Ʈ�� �ݴ´�.
void CCommThread::ClosePort()
{
	//--> ������� �ʾ���.
	m_bConnected = FALSE;

	//--> ����ũ ����..
	SetCommMask( m_hComm, 0);
	
	//--> ��Ʈ ����.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	
	//--> �ڵ� �ݱ�.
	CloseHandle( m_hComm);
}

// ��Ʈ�� pBuff�� ������ nToWrite��ŭ ����.
// ������ ������ Byte���� �����Ѵ�.
DWORD CCommThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//--> ��Ʈ�� ������� ���� �����̸�..
	if( !m_bConnected )		
	{
		return 0;
	}


	//--> ���ڷ� ���� ������ ������ nToWrite ��ŭ ����.. �� ������.,dwWrite �� �ѱ�.
	if( !WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		//--> ���� ������ ���ڰ� ������ ���..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ��� Overapped IO��
			// Ư���� ���� ERROR_IO_PENDING ���� �޽����� ���޵ȴ�.
			//timeouts�� ������ �ð���ŭ ��ٷ��ش�.
			while (! GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError( m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError( m_hComm, &dwErrorFlags, &comstat);
		}
	}


	//--> ���� ��Ʈ�� ������ ������ ����..
	return dwWritten;
}


// ��Ʈ�κ��� pBuff�� nToWrite��ŭ �д´�.
// ������ ������ Byte���� �����Ѵ�.
DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//--- system queue�� ������ byte���� �̸� �д´�.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);

	//--> �ý��� ť���� ���� �Ÿ��� ������..
	dwRead = comstat.cbInQue;
	
	if(dwRead > 0)
	{

		//--> ���ۿ� �ϴ� �о���̴µ�.. ����..�о���ΰ��� ���ٸ�..

		if( !ReadFile( m_hComm, pBuff, nToRead, &dwRead, &m_osRead) )
		{
			//--> ���� �Ÿ��� ��������..
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts�� ������ �ð���ŭ ��ٷ��ش�.
				while (! GetOverlappedResult( m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError( m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}


	//--> ���� �о���� ������ ����.
	return dwRead;

}


void CCommThread::ProcessRcvData( BYTE *byBuf, int nSize )
{
	//--> �߰�....
	if( !nSize )		return;


	//--> Clear Buf
	for( int i=1; i<MAX_RECEIVE; i++ )
	{
		g_byRcvBuf[i] = 0;
	}

	//--> Buf Copy..
	for( i=0; i<nSize; i++ )
	{
//		g_byRcvBuf[i + 1] = byBuf[i];
		g_byRcvBuf[i] = byBuf[i];
	}

//	//--> Receive Total Count
//	++g_byRcvBuf[0];


	//--> �޼��� ��Ʈ�� ����ΰ�??
	UINT uiMessagePort = ( !m_wPortID )  ?  (UINT)U_MSG_RECEIVE_COM1 : 
											(UINT)U_MSG_RECEIVE_COM2;

	//--> View ����..�޼��� ����..���ŵ� ��Ʈ�� �޼���..
	::SendMessage( g_hViewWnd, uiMessagePort, 0, nSize );
			 
}

// ��Ʈ�� �����ϰ�, ���� ������ ������
// m_ReadData�� ������ �ڿ� MainWnd�� �޽����� ������ Buffer�� ������
// �о��� �Ű��Ѵ�.

DWORD	ThreadWatchComm(CCommThread* pComm)
{
   DWORD           dwEvent;
   OVERLAPPED      os;
   BOOL            bOk = TRUE;
   BYTE            buff[4096];      // �б� ����
   DWORD           dwRead;  // ���� ����Ʈ��.


   // Event, OS ����.
   memset( &os, 0, sizeof(OVERLAPPED));
   
   //--> �̺�Ʈ ����..
   if( !(os.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)) )
   {
		bOk = FALSE;
   }

   //--> �̺�Ʈ ����ũ..
   if( !SetCommMask( pComm->m_hComm, EV_RXCHAR) )
   {
	   bOk = FALSE;
   }

   //--> �̺�Ʈ��..����ũ ������ ������..
   if( !bOk )
   {
		AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
   }

   // ��Ʈ�� �����ϴ� ����.
   while( pComm->m_bConnected )
   {
        dwEvent = 0;

        // ��Ʈ�� ���� �Ÿ��� �ö����� ��ٸ���.
        WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);
		
		//--> �����Ͱ� ���ŵǾ��ٴ� �޼����� �߻��ϸ�..
        if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
        {
            // ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
            do
            {
				//--> buff �� �޾Ƴ���..
                dwRead = pComm->ReadComm( buff, 4096);
				
				//--> ���۸� �м��ϴ� �ܰ�� �ѱ��.
				pComm->ProcessRcvData( buff, (BYTE)dwRead );
				 

            } while (dwRead);

        }//endif~

   }//endwhile(..)... ��Ʈ�� ClosePort�� ���� ������ m_bConnected �� FALSE�� �Ǿ� ����.

   CloseHandle( os.hEvent);

   //--> ������ ���ᰡ �ǰ���?
   pComm->m_hThreadWatchComm = NULL;

   return TRUE;
}

void CCommThread::CommPortBufferClear()
{
	//--> ��Ʈ ����.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
}
