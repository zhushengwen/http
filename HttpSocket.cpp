// HttpSocket.cpp: implementation of the CHttpSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HttpSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAXHEADERSIZE 1024
#define I_HOSTNAME 0
#define I_IP 1
int FindPos(const char * res,const char *dest,int sta=0)
{
	int nPos = -1;
	//   nPos = strRespons.Find(szSession,0);
	const char * temp=strstr(res+sta,dest);
	if(temp!=NULL)
		return temp-res-sta;
	else 
		return -1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHttpSocket::CHttpSocket()
{
	m_s=NULL;
	m_phostent=NULL;
	m_port=80;
	m_bConnected=FALSE;
	for(int i=0;i<256;i++)
		m_ipaddr[i]='\0';
	memset(m_requestheader,0,MAXHEADERSIZE);
	memset(m_ResponseHeader,0,MAXHEADERSIZE);
	m_nCurIndex = 0;        //
	m_bResponsed = FALSE;
	m_nResponseHeaderSize = -1;
}
char* CHttpSocket::GetIP(char* buff)
{
	return GetInfo(buff,I_IP);
}
char* CHttpSocket::GetName(char* buff)
{
	return GetInfo(buff,I_HOSTNAME);
}
char* CHttpSocket::GetInfo(char* buff,int i_type)
{
	WSADATA     _wsaData         =   {0};
	char        _HostName[1024]  =   {0};
	struct hostent* _HostList    =   NULL;
	struct in_addr  _addr        =   {0};

	int         _Result          =   0;
	int          i               =   0;

	_Result = WSAStartup(MAKEWORD(2,2),&_wsaData);
	if(_Result == SOCKET_ERROR)
	{
		TRACE("WSAStartup failed\r\n");
		return buff;
	}


	_Result = gethostname(_HostName,sizeof(_HostName));
	if(_Result == SOCKET_ERROR)
	{
		TRACE("Gethostname failed\r\n");

	}

	_HostList = gethostbyname(_HostName);


	TRACE("Hostname:%s\r\n",_HostName);
	char ip[20]={0};
	for(i=0;_HostList->h_addr_list[i] != NULL;i++)
	{
		memcpy(&(_addr.S_un.S_addr),(void *)_HostList->h_addr_list[i],4);   // ====++++++
		
		TRACE("IP:%s\r\n",inet_ntoa(_addr));

		strcpy(ip,inet_ntoa(_addr));

		break;
	}

	//��ס��������һ�������׺���
	if(WSACleanup() == SOCKET_ERROR)
	{
		TRACE("WSACleanup failed\r\n");

	}
	if(i_type==I_HOSTNAME)
	{
		strcpy(buff,_HostName);
		return buff;
	}
	if(i_type==I_IP)
	{
		strcpy(buff,ip);
		return buff;
	}
}
CHttpSocket::~CHttpSocket()
{
	CloseSocket();
	WSACleanup();
}

BOOL CHttpSocket::Socket()
{
	if(m_bConnected)return FALSE;

	struct protoent *ppe; 
	ppe=getprotobyname("http"); 
	WORD wVersion=MAKEWORD(1,1); 
	WSADATA wsData; 
	int nResult= WSAStartup(wVersion,&wsData); //1������Winsock
	if(nResult !=0) 
	{ 
		TRACE("����Winsockʧ��!\n"); 
	} 

	if ( LOBYTE( wsData.wVersion ) != 1 ||

		HIBYTE( wsData.wVersion ) != 1 ) {

			/* Tell the user that we could not find a usable */

			/* WinSock DLL.                                  */

			WSACleanup( );
			return false; 
	}


	///����SOCKET����
	m_s=socket(PF_INET,SOCK_STREAM,IPPROTO_IP);
	if(m_s==INVALID_SOCKET)
	{
		//TRACE("socket()����ִ��ʧ��!\n");
		return FALSE;
	}

	return TRUE;

}

BOOL CHttpSocket::Connect(char *szHostName,int nPort)
{
	if(szHostName==NULL)
		return FALSE;

	///���Ѿ�����,���ȹر�
	if(m_bConnected)
	{
		CloseSocket();
	}

	///����˿ں�
	m_port=nPort;

	///�������������IP��ַ
	m_phostent=gethostbyname(szHostName);
	if(m_phostent==NULL)
	{
		TRACE("gethostbyname()����ִ��ʧ��!\n");
		return FALSE;
	}    
	///����
	struct sockaddr_in ip_addr;
	memcpy(&ip_addr.sin_addr.s_addr,m_phostent->h_addr_list[0],m_phostent->h_length);///h_addr_list[0]��4���ֽ�,ÿ���ֽ�8λ
	char str[40]={0};
	sprintf(str,"%s",inet_ntoa(ip_addr.sin_addr));
	struct sockaddr_in destaddr;
	memset((void *)&destaddr,0,sizeof(destaddr)); 
	destaddr.sin_family=AF_INET;
	destaddr.sin_port=htons(80);
	destaddr.sin_addr=ip_addr.sin_addr;

	///����������IP��ַ�ַ���
	sprintf(m_ipaddr,"%d.%d.%d.%d",
		destaddr.sin_addr.S_un.S_un_b.s_b1,
		destaddr.sin_addr.S_un.S_un_b.s_b2,
		destaddr.sin_addr.S_un.S_un_b.s_b3,
		destaddr.sin_addr.S_un.S_un_b.s_b4);

	if(connect(m_s,(struct sockaddr*)&destaddr,sizeof(destaddr))!=0)
	{
		//    TRACE(NULL,"connect()����ִ��ʧ��!","����",MB_OK);
		return FALSE;
	}
	///�����Ѿ����ӵı�־
	m_bConnected=TRUE;
	return TRUE;
}

//������������URL���HTTP����ͷ
const char *CHttpSocket::FormatRequestHeader(char *pServer,char *pObject, long &Length,
											 char *pCookie,char *pReferer,long nFrom,
											 long nTo,int nServerType)
{
	char szPort[10];
	char szTemp[20];
	sprintf(szPort,"%d",m_port);
	memset(m_requestheader,'\0',1024);

	///��1��:����,�����·��,�汾
	strcat(m_requestheader,"GET ");
	strcat(m_requestheader,pObject);
	strcat(m_requestheader," HTTP/1.1");
	strcat(m_requestheader,"\r\n");

	///��2��:����
	strcat(m_requestheader,"Host:");
	strcat(m_requestheader,pServer);
	strcat(m_requestheader,"\r\n");

	///��3��:
	if(pReferer != NULL)
	{
		strcat(m_requestheader,"Referer:");
		strcat(m_requestheader,pReferer);
		strcat(m_requestheader,"\r\n");        
	}

	///��4��:���յ���������
	strcat(m_requestheader,"Accept:*/*");
	strcat(m_requestheader,"\r\n");

	///��5��:���������
	strcat(m_requestheader,"User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
	strcat(m_requestheader,"\r\n");

	///��6��:��������,����
	strcat(m_requestheader,"Connection:Keep-Alive");
	strcat(m_requestheader,"\r\n");

	///��7��:Cookie.
	if(pCookie != NULL)
	{
		strcat(m_requestheader,"Set Cookie:0");
		strcat(m_requestheader,pCookie);
		strcat(m_requestheader,"\r\n");
	}

	///��8��:�����������ʼ�ֽ�λ��(�ϵ������Ĺؼ�)
	if(nFrom > 0)
	{
		strcat(m_requestheader,"Range: bytes=");
		_ltoa(nFrom,szTemp,10);
		strcat(m_requestheader,szTemp);
		strcat(m_requestheader,"-");
		if(nTo > nFrom)
		{
			_ltoa(nTo,szTemp,10);
			strcat(m_requestheader,szTemp);
		}
		strcat(m_requestheader,"\r\n");
	}

	///���һ��:����
	strcat(m_requestheader,"\r\n");

	///���ؽ��
	Length=strlen(m_requestheader);
	return m_requestheader;
}

///��������ͷ
BOOL CHttpSocket::SendRequest(const char *pRequestHeader, long Length)
{
	if(!m_bConnected)return FALSE;

	if(pRequestHeader==NULL)
		pRequestHeader=m_requestheader;
	if(Length==0)
		Length=strlen(m_requestheader);

	if(send(m_s,pRequestHeader,Length,0)==SOCKET_ERROR)
	{
		TRACE("send()����ִ��ʧ��!\n");
		return FALSE;
	}
	int nLength;
	GetResponseHeader(nLength);
	return TRUE;
}

long CHttpSocket::Receive(char* pBuffer,long nMaxLength)
{
	if(!m_bConnected)return NULL;

	///��������
	long nLength;
	nLength=recv(m_s,pBuffer,nMaxLength,0);

	if(nLength <= 0)
	{
		//MessageBox(NULL,"recv()����ִ��ʧ��!","����",MB_OK);
		CloseSocket();
	}
	return nLength;
}

///�ر��׽���
BOOL CHttpSocket::CloseSocket()
{
	if(m_s != NULL)
	{
		if(closesocket(m_s)==SOCKET_ERROR)
		{
			// TRACE("closesocket()����ִ��ʧ��!\n");
			return FALSE;
		}
	}
	m_s = NULL;
	m_bConnected=FALSE;
	return TRUE;
}

int CHttpSocket::GetRequestHeader(char *pHeader, int nMaxLength) const
{
	int nLength;
	if(int(strlen(m_requestheader))>nMaxLength)
	{
		nLength=nMaxLength;
	}
	else
	{
		nLength=strlen(m_requestheader);
	}
	memcpy(pHeader,m_requestheader,nLength);
	return nLength;
}

//���ý��ջ��߷��͵��ʱ��
BOOL CHttpSocket::SetTimeout(int nTime, int nType)
{
	if(nType == 0)
	{
		nType = SO_RCVTIMEO;
	}
	else
	{
		nType = SO_SNDTIMEO;
	}

	DWORD dwErr;
	dwErr=setsockopt(m_s,SOL_SOCKET,nType,(char*)&nTime,sizeof(nTime)); 
	if(dwErr)
	{
		TRACE("setsockopt()����ִ��ʧ��!\n");
		return FALSE;
	}
	return TRUE;
}

//��ȡHTTP����ķ���ͷ
const char* CHttpSocket::GetResponseHeader(int &nLength)
{
	if(!m_bResponsed)
	{
		char c = 0;
		int nIndex = 0;
		BOOL bEndResponse = FALSE;
		while(!bEndResponse && nIndex < MAXHEADERSIZE)
		{
			recv(m_s,&c,1,0);
			m_ResponseHeader[nIndex++] = c;
			if(nIndex >= 4)
			{
				if(m_ResponseHeader[nIndex - 4] == '\r' && m_ResponseHeader[nIndex - 3] == '\n'
					&& m_ResponseHeader[nIndex - 2] == '\r' && m_ResponseHeader[nIndex - 1] == '\n')
					bEndResponse = TRUE;
			}
		}
		m_nResponseHeaderSize = nIndex;
		m_bResponsed = TRUE;
	}

	nLength = m_nResponseHeaderSize;
	return m_ResponseHeader;
}

//����HTTP��Ӧͷ�е�һ��.
int CHttpSocket::GetResponseLine(char *pLine, int nMaxLength)
{
	if(m_nCurIndex >= m_nResponseHeaderSize)
	{
		m_nCurIndex = 0;
		return -1;
	}

	int nIndex = 0;
	char c = 0;
	do 
	{
		c = m_ResponseHeader[m_nCurIndex++];
		pLine[nIndex++] = c;
	} while(c != '\n' && m_nCurIndex < m_nResponseHeaderSize && nIndex < nMaxLength);

	return nIndex;
}

int CHttpSocket::GetField(const char *szSession, char *szValue, int nMaxLength)
{
	//ȡ��ĳ����ֵ
	if(!m_bResponsed) return -1;

	// CString strRespons;
	// strRespons = m_ResponseHeader;
	int nPos = -1;
	//   nPos = strRespons.Find(szSession,0);
	nPos=FindPos(m_ResponseHeader,szSession);
	// 	if(temp!=NULL)
	// 	nPos=temp-m_ResponseHeader;
	if(nPos != -1)
	{
		nPos += strlen(szSession);
		nPos += 2;
		int nCr = FindPos(m_ResponseHeader,"\r\n",nPos);
		//CString strValue = strRespons.Mid(nPos,nCr - nPos);
		strncpy(szValue,m_ResponseHeader+nPos,nCr);
		*(szValue+nCr)=0;
		return nCr;//nCr - nPos
	}
	else
	{
		return -1;
	}
}


int CHttpSocket::GetServerState()
{
	//��û��ȡ����Ӧͷ,����ʧ��
	if(!m_bResponsed) return -1;

	char szState[3];
	szState[0] = m_ResponseHeader[9];
	szState[1] = m_ResponseHeader[10];
	szState[2] = m_ResponseHeader[11];

	return atoi(szState);
}


