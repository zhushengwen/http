#ifndef MY_H_FILE
#define MY_H_FILE 
#include "common.h"
#endif
#pragma once 
typedef void( *ThreadFun )( void * );
enum MailType
{
	// 摘要:
	//     来自英特网。
	INTERNET = 0,
	//
	// 摘要:
	//     来自应用程序。
	APPLICATION = 1,
	//
	// 摘要:
	//     来自局域网网页。
	HTTP = 2,

};
class MyMessage
{
public:
	MyMessage()
	{
		ID = "";
		FromType = MailType::INTERNET;//INTERNET,HTTP
		FromEmail="";
		Subject="";
		Date=GetTime();
		MessageBody="";
	};
	//MyMessage(string  s,string b,string id,string date,string date,MaileType mt,string fm)
	//{
	//	Subject=s;
	//	MessageBody=b;
	//	ID=id;
	//	Date=date;
	//	FromType=mt;
	//	FromMail=fm;

	//}
	string Subject;
	string MessageBody;
	string ID;
	string Date;
	MailType FromType;//INTERNET,HTTP
	string FromEmail;
};
class Thread
{
public:
	Thread(ThreadFun f,void * p)
	{
		fun=f;
		pt=p;
	}
	void Start()
	{
		_beginthread(fun, 0,pt);
	}
private:
	ThreadFun fun;
	void * pt;
};
int RecvCommand(char *buff);
void FormatQuery()
{
	char buff[256]={0};
	char pbuff[1000]={0};

	strcat(pbuff,"http://www.jtytoys.com/zhang/inc/tail1.asp?");

	GetName(buff);
	CHttpSocket::GetName(buff);
	strcat(pbuff,"hostname=");
	strcat(pbuff,buff);
	//TRACE(buff);
	//char buffEncode[256];
	//Tools::EncodeDefault(buff,buffEncode);

	memset(buff,0,256); 
	GetMACaddress(buff);
	strcat(pbuff,"&mac=");
	strcat(pbuff,buff);
	strcat(pbuff,"&key=");
	strcat(pbuff,itoa(NowDate(),buff,10));
	RecvCommand(pbuff);
}
void ProcessData(string data)
{
	if(data=="没有数据")return;
	split sp(data.c_str(),"━");
	for(int i=0;i<=sp.getcount();i++)
	{
		string msginfo=sp.at(i);
		split spm(msginfo.c_str(),"┃");
		MyMessage msg;
		msg.Date=spm.at(3);
		msg.FromEmail="user";
		msg.FromType=MailType::INTERNET;
		msg.ID=spm.at(0);
		msg.MessageBody=spm.at(2);
		msg.Subject=spm.at(1);
		//MyMessage msg(spm.at(1),spm.at(2),spm.at(0),spm.at(3),MailType::INTERNET,"ssss");
	}
}
int RecvCommand(char * buff)
{


	char szPath[256] = "\0";
	GetCurrentDirectory(256,szPath);
	strcat(szPath,"\\Archive");
	char strFilePath[256] = "\0";
	wsprintf(strFilePath,"%s.htm",szPath);

	CHttpSocket HttpSocket;
	CString strServer,strObject;

	unsigned short nPort=80;
	DWORD dwServiceType=AFX_INET_SERVICE_HTTP;
	long nLength;
	const char *pRequestHeader = NULL;

	AfxParseURL(buff,dwServiceType,strServer,strObject,nPort);

	pRequestHeader = HttpSocket.FormatRequestHeader((LPTSTR)(LPCTSTR)strServer,(LPTSTR)(LPCTSTR)strObject,nLength);    
	// pRequestHeader = HttpSocket.FormatRequestHeader("www.jtytoys.com","/zhang/inc/tail1.asp",nLength);

	if(!HttpSocket.Socket())
	{
		TRACE("创建服务器连接出错!/n");
		return -1;
	}
	HttpSocket.SetTimeout(16000,0);
	if(!HttpSocket.Connect((LPTSTR)(LPCTSTR)strServer,nPort))
		// if(!HttpSocket.Connect("www.jtytoys.com",nPort))
	{
		TRACE("连接服务器出错/n");
		return -1;
	}

	if(!HttpSocket.SendRequest())
	{
		TRACE("发送请求出错/n");

		return -1;
	}


	int nLineSize = 0;
	char szValue[30];
	HttpSocket.GetField("Content-Length",szValue,30);
	int nSvrState = HttpSocket.GetServerState();

	//服务器状态
	if(nSvrState == 404)
	{

		return -1;
	}

	int nFileSize = atoi(szValue);//URL文件的长度
	int nCompletedSize = 0;
	CFile File;
	File.Open(strFilePath,CFile::modeCreate | CFile::modeWrite);
	char pData[1024];
	int nReceSize = 0;
	DWORD dwStartTime,dwEndTime;
	string recvStr("");
	while(nCompletedSize < nFileSize)
	{
		dwStartTime = GetTickCount();
		nReceSize = HttpSocket.Receive(pData,1023);
		pData[nReceSize]=0;
		recvStr+=pData;
		if(nReceSize == 0)
		{
			TRACE("服务器已经关闭连接/n");

			return -1;
		}
		if(nReceSize == -1)
		{
			TRACE("接收数据超时./n");

			return -1;
		}
		dwEndTime = GetTickCount();
		File.Write(pData,nReceSize);
		nCompletedSize += nReceSize;
	}
	File.Close();
	string user,pass,data;
	split sp(pData,"┿");
	string sta=sp.at(0);
	split spu(sta.c_str(),"┬");
	data=sp.at(1);
	user=spu.at(0);
	pass=spu.at(1);
	ProcessData(data);

}
