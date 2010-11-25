#ifndef		__MYCOMMTHREAD_H__
#define		__MYCOMMTHREAD_H__


//	��� Ŭ����	CCommThread 

// ��Ʈ���� �б� :
//   ��Ʈ�� �� �Ŀ� ��Ʈ�� �ڷᰡ �����ϸ� WM_COMM_READ �޽����� ���� 
//   �����쿡 ���޵ȴ�. ON_MESSAGE ��ũ�θ� �̿�, �Լ��� �����ϰ�
//   m_ReadData String�� ����� �����͸� �̿� �б�
// ��Ʈ�� ���� : WriteComm(buff, 30)�� ���� ���ۿ� �� ũ�⸦ �ǳ׸� �ȴ�.

class	CCommThread
{
public:
	void CommPortBufferClear();

	void ProcessRcvData( BYTE *byBuf, int nSize );

	BOOL bInitComPort( CString strPortName, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );

	CCommThread();
	~CCommThread();
	//--------- ȯ�� ���� -----------------------------------------//
	HANDLE		m_hComm;				// ��� ��Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1 ..)
	BOOL		m_bConnected;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ��.
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch�Լ� Thread �ڵ�.
	WORD		m_wPortID;				// WM_COMM_READ�� �Բ� ������ �μ�.

	//--------- �ܺ� ��� �Լ� ------------------------------------//
	BOOL	OpenPort(CString sPortName, DWORD dwBaud, WORD wParam);
	void	ClosePort();
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);

	//--------- ���� ��� �Լ� ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);

private :


};

// Thread�� ����� �Լ� 
DWORD	ThreadWatchComm(CCommThread* pComm);

#endif