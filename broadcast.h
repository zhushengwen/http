#include"mythread.h"

#include "stdafx.h"  
#include <WinSock2.h>  
#include <stdio.h>  
#include <iostream>  
using namespace std;  
#pragma comment(lib, "ws2_32.lib")  
#include <algorithm> 
#include <map>
#ifndef MY_H_FILE
#define MY_H_FILE 
#include "common.h"
#endif
#ifndef MY_H_SOCKET
#define MY_H_SOCKET
#include "HttpSocket.h"
#endif
#pragma once 
extern bool ReceiveThreadIsCreated;
const int MAX_BUF_LEN = 255;  
map<IPP,DateTime> idmap;
CRITICAL_SECTION  g_cs;//�ٽ�������
//vector<IPP> GetOnLine( ) 
//{  
//	vector<IPP> vip;
//	InitializeCriticalSection(&g_cs);
//	EnterCriticalSection(&g_cs);//�ȴ��ٽ������������Ȩ 
//	map<IPP,DateTime> ::iterator m1_Iter;
//	for ( m1_Iter = idmap.begin( ); m1_Iter != idmap.end( ); m1_Iter++ )
//	{
//		if(DateTime(GetTime())-(*m1_Iter).second>10)
//		{
//			idmap.erase(m1_Iter);
//		}else vip.push_back((*m1_Iter).first);
//	}
//	LeaveCriticalSection(&g_cs);//�ͷ�ָ���ٽ�����������Ȩ 
//	DeleteCriticalSection(&g_cs);
//	return vip;
//} 
vector<IPP> AddOrUpDateIPOrGetOnline(const IPP ip=0,bool isGetOnLine=true)
{
	vector<IPP> vip;
	InitializeCriticalSection(&g_cs);
	EnterCriticalSection(&g_cs);//�ȴ��ٽ������������Ȩ 
	if(isGetOnLine)
	{
		map<IPP,DateTime> ::iterator m1_Iter;
		for ( m1_Iter = idmap.begin( ); m1_Iter != idmap.end( );  )
		{
			if(DateTime(GetTime())-(*m1_Iter).second>10)
			{
				idmap.erase(m1_Iter++);
			}
			else 
			{
				vip.push_back((*m1_Iter).first);
				++m1_Iter;
			}
		}
	}
	else
	{
		char tempb[16]={0};
		IPP ip1(CHttpSocket::GetIP(tempb));
		if(ip1!=ip)
		{
			idmap[ip]=DateTime(GetTime());
		}
	}
	LeaveCriticalSection(&g_cs);//�ͷ�ָ���ٽ�����������Ȩ 
	DeleteCriticalSection(&g_cs);
	return vip;
}
void SendB()  
{  
	WORD wVersionRequested;  
	WSADATA wsaData;  
	int err;  

	// ����socket api  
	wVersionRequested = MAKEWORD( 2, 2 );  
	err = WSAStartup( wVersionRequested, &wsaData );  
	if ( err != 0 )  
	{  
		return ;  
	}  

	if ( LOBYTE( wsaData.wVersion ) != 2 ||  
		HIBYTE( wsaData.wVersion ) != 2 )  
	{  
		WSACleanup( );  
		return ;   
	}  

	// ����socket  
	SOCKET connect_socket;  
	connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  
	if(INVALID_SOCKET == connect_socket)  
	{  
		//err = WSAGetLastError();  
		//printf("\"socket\" error! error code is %d\n", err);  
		return ;  
	}  

	SOCKADDR_IN sin;  
	sin.sin_family = AF_INET;  
	sin.sin_port = htons(3779);  
	sin.sin_addr.s_addr = INADDR_BROADCAST;  

	bool bOpt = true;  
	//���ø��׽���Ϊ�㲥����  
	setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));  

	int nAddrLen = sizeof(SOCKADDR);  
	int nLoop = 0;  
	while(1)  
	{  
		nLoop++;  

		char tempb[16]={0};

		IPP ip(CHttpSocket::GetIP(tempb));
		//string ips=ip;
		// ��������  
		int address=ip.GetValue();
		int nSendSize = sendto(connect_socket, (const char *)&address, 4, 0, (SOCKADDR*)&sin, nAddrLen);  
		if(SOCKET_ERROR == nSendSize)  
		{  
			//err = WSAGetLastError();  
			//printf("\"sendto\" error!, error code is %d\n", err);  
			return ;  
		}  
		//return;
		Sleep(5*1000);  
	}  

	return ;  
} 
void RecvB()
{  
	WORD wVersionRequested;  
	WSADATA wsaData;  
	int err;  

	// ����socket api  
	wVersionRequested = MAKEWORD( 2, 2 );  
	err = WSAStartup( wVersionRequested, &wsaData );  
	if ( err != 0 )  
	{  
		return ;  
	}  

	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )  
	{  
		WSACleanup( );  
		return ;   
	}  

	// ����socket  
	SOCKET connect_socket;  
	connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  
	if(INVALID_SOCKET == connect_socket)  
	{  
		err = WSAGetLastError();  
		//printf("\"socket\" error! error code is %d\n", err);  
		return ;  
	}  

	// �������׽���  
	SOCKADDR_IN sin;  
	sin.sin_family = AF_INET;  
	sin.sin_port = htons(3779);  
	sin.sin_addr.s_addr = 0;  

	// �����������ϵĹ㲥��ַ��������  
	SOCKADDR_IN sin_from;  
	sin_from.sin_family = AF_INET;  
	sin_from.sin_port = htons(3779);  
	sin_from.sin_addr.s_addr = INADDR_BROADCAST;  

	//���ø��׽���Ϊ�㲥���ͣ�  
	bool bOpt = true;  
	setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));  

	// ���׽���  
	err = bind(connect_socket, (SOCKADDR*)&sin, sizeof(SOCKADDR));  
	if(SOCKET_ERROR == err)  
	{  
		err = WSAGetLastError();  
		//printf("\"bind\" error! error code is %d\n", err);  
		return ;  
	}  

	int nAddrLen = sizeof(SOCKADDR);  
	char buff[4] = "";  
	int nLoop = 0;  

	while(1)  
	{  
		// ��������  
		int nSendSize = recvfrom(connect_socket, buff, 4, 0, (SOCKADDR*)&sin_from, &nAddrLen);  
		if(SOCKET_ERROR == nSendSize)  
		{  
			err = WSAGetLastError();  
			//printf("\"recvfrom\" error! error code is %d\n", err);  
			return ;  
		}  

		IPP ip(*(unsigned *)buff);
		AddOrUpDateIPOrGetOnline(ip,false);
		//idmap[ip]=DateTime(GetTime());
		//string temp=idmap[ip].ToString();
		//sort(idmap.begin(),idmap.end());
		//printf("Recv: %s:%s\n",temp.c_str(), ip.ToString().c_str());  

	}  

	return ;  
}
/*
void DiffImg();
void CastThread()
{
	while(true)
	{

		DiffImg();
		//remove(path.c_str());
		Sleep(1000);
	}
}
void DiffImg()
{
	string time=GetTime();
	string path1=GetNowPath(".jpg",time);
	CastImage(path1);

	ImgDb imgdb;
	static string Rtime;
	if(Rtime=="")
	{
		imgdb.CreateImgDiffTab("DIFFNR");
		Rtime=time;
		return;
	}
	FILE* stream = fopen(path1.c_str(), "rb");
	string path0=GetNowPath(".jpg",Rtime);
	FILE* stream0=fopen(path0.c_str(), "rb");
	int ch=1;
	ZBase64 zBase;
	IAS ias;
	bool difest=false;
	int discount=0;
	bool succ=true;
	if (stream && stream0)
	{
		while(ch!=0)
		{
			char buff[1]={0};
			char buff0[1]={0};
			ch = fread(buff,1,1,stream);
			ias++;
			if(ias>130*1024)
			{
				succ=false;
				break;
			}
			if(ch!=0)
			{
				ch=fread(buff0,1,1,stream0);
				if(ch!=0 && *buff==*buff0)continue;
			}else
			{
				break;
			}
			imgdb.InsertDiffItem("DIFFNR",time,Rtime,ias);
			break;
		}
		fclose(stream);
		fclose(stream0);
		if(!succ)
		{
			Rtime=time;
		}
	}
	return;
}
*/

#define M 512000
void SendFile(SOCKET ss,const string& file_path,int sl)
{
	FILE *fp;
	long i;									//�ļ��ֵĵĿ���
	string tips=":Error!";
	int nResult=0;
	if( (fp=fopen(file_path.c_str(),"rb")) == NULL)   //���ļ�
	{
		printf("�ļ����������%s!\n",file_path.c_str());
		nResult= send(ss,tips.c_str(),tips.length()+1,0);
		return;
	}
	string sendpath=file_path.substr(sl);
	nResult= send(ss,sendpath.c_str(),sendpath.length()+1,0);  //�����ļ���
	char mess[M];
	nResult= recv(ss,mess,M,0);

	long readLength;						//���ļ���ȡ�ĳ���
	long sendLength;						//�����ļ��ĳ���
	int k=1;								//���ڴ��͵�ģ���
	long total=0;

	fseek(fp,0L,2);
	long fileLength=ftell(fp);				//ȡ���ļ��ĳ���
	fseek(fp,0L,0);

	char Length[100];
	ltoa(fileLength,Length,10);
	nResult=send(ss,Length,strlen(Length)+1,0);		//�����ļ���С���ͻ���
	nResult=recv(ss,mess,M,0);
	tips=mess;
	if(tips=="Error!")return;
	if( 0 == fileLength%M )					//���ļ����зֿ�
	{
		i=fileLength/M;
	}
	else
	{
		i=fileLength/M+1;
	}
	while( k <= i )
	{	
		Sleep(100);			//�˷ǳ���Ҫ�������˴���ʱ�䣬��֤�����ߵ�ͬ��
		readLength=fread(mess,sizeof(char),M,fp);
		sendLength=send(ss,mess,readLength,0);
		total+=sendLength;
		//printf("\r�� %d ģ��   ��ȡ��%ldB  ���ͣ�%ldB",k,readLength,sendLength);
		//printf("  ����: %2.2f%c  ģ�飺%ld / %ld",100*(total*1.0/fileLength),37,k,i);
		k++;
	}
	fclose(fp);
	if(i==0)
	{
		tips="OK!";
		nResult=send(ss,tips.c_str(),tips.length()+1,0); 
	}
	nResult=recv(ss,mess,M,0);
	//if(i!=0)	
	//	nResult=recv(ss,mess,M,0);
	//else
	//{
	//	tips="OK!";
	//	nResult=send(ss,tips.c_str(),tips.length()+1,0); 
	//	nResult=recv(ss,mess,M,0);
	//}

}
void SendDir(SOCKET sc,const string& dir_path,int sl=0)
{
	vector<string> vs1=ListFile(dir_path);
	if(sl==0)
	{
		string spath=get_file_shortname(dir_path);
		sl=dir_path.length()-spath.length();
	}
	for(vector<string>::iterator it1  = vs1.begin(); it1 != vs1.end(); ++it1)  
	{ 
		SendFile(sc,*(it1),sl);
	}
	vs1=ListFile(dir_path,false);
	for(vector<string>::iterator it1  = vs1.begin(); it1 != vs1.end(); ++it1)  
	{ 
		SendDir(sc,*(it1),sl);
	}

}
void SendFileThread(void * arg)
{
	SendFileBlock * sfb=(SendFileBlock *)arg;
	IPP ip=sfb->ip;
	string path=sfb->filepath;
	delete sfb;
	string ips=ip.ToString();
	SOCKET sc=SocketConnect(ips,8003);
	if(sc==NULL)return;
	if(folder_exists(path))
	{
		string tips="GET :SENDDIRS ";
		int nResult=send(sc,tips.c_str(),tips.length()+1,0); 
		char ConForm[100];
		nResult=recv(sc,ConForm,100,0);
		if(nResult!=-1)SendDir(sc,path);
	}
	closesocket(sc);
}
void ReceiveFile(void* arg)
{
	SOCKET s_d=(SOCKET)arg;
	char mess[M];
	struct sockaddr_in addrClient;
	int len = sizeof(addrClient);
	if(!getpeername(s_d, (struct sockaddr *)&addrClient, &len))
	{
		printf( "�Է�IP��%s ", inet_ntoa(addrClient.sin_addr));
		printf( "�Է�PORT��%d ", ntohs(addrClient.sin_port));
	}
	while(1)
	{
	char fileName[255];
	int nResult=-1;
	nResult=recv(s_d,fileName,255,0);       //�����ļ���
	if(nResult<=0)break;
	string tips=fileName;
	if(tips==":Error!")continue;
	nResult=send(s_d,tips.c_str(),tips.length()+1,0); 

	nResult=recv(s_d,mess,M,0);   
	long fileLength=atol(mess);      //ȡ�Ĵ������ļ��ĳ���

	long i;	                              //����ļ�����

	if( 0 == fileLength%M )				  //���ļ���N��λ���зֿ�
	{
		i=fileLength/M;
	}
	else
	{
		i=fileLength/M+1;
	}

	FILE *fp;							//���ļ�
	int k=1;					    	//��ǰ���ڽ����ļ��Ŀ���
	long receiveLength;					//�����ļ��ĳ���
	long writeLength;					//��д���ļ����ַ�����
	string file_save=combine_path(get_desk_path(inet_ntoa(addrClient.sin_addr)),fileName);
	string save_path=get_file_path(file_save);
	if(!folder_exists(save_path))
		create_folder(save_path);
	fp=fopen(file_save.c_str(),"wb");
	if(fp!=NULL)
	{
		tips="OK!";
	}
	else
	{
		tips="Error!";
	}
	nResult=send(s_d,tips.c_str(),tips.length()+1,0); 
		if(fp!=NULL) 
		{		
				//��ͻ��˿�ʼ���ͣ�ȷ��ͬ����ʼ
			long total=0;						//�ѽ����ļ��ĳ���
			while(k<=i)                         //ѭ���Ľ����ļ�
			{
				receiveLength=recv(s_d,mess,M,0);
				if( receiveLength== -1 )        //�жϷ�����Ƿ�ر�
				{
				printf("\n������ѶϿ�\n");
				return;
				}
				writeLength=fwrite(mess,sizeof(char),receiveLength,fp);
				printf("\r�� %d ģ��   ���գ�%ldB  д�룺%ldB",k,receiveLength,writeLength);
				total+=writeLength;
				printf("  ����: %2.2f%c  ģ�飺%ld / %ld",100*(total*1.0/fileLength),37,k,i);
				k++;
			}
		fclose(fp);
		printf("\n�ļ�%s�������!\n",fileName);
		
		if(i==0)
			nResult=recv(s_d,mess,M,0);
		tips="OK!";
		nResult=send(s_d,tips.c_str(),tips.length()+1,0); 
		//if(i!=0)
		//	{
		//tips="OK!";
		//nResult=send(s_d,tips.c_str(),tips.length()+1,0); 
		//	}
		//	else 
		//	{
		//		nResult=recv(s_d,mess,M,0);
		//		tips="OK!";
		//		nResult=send(s_d,tips.c_str(),tips.length()+1,0); 
		//	}
		}
	}
}
void ReceiveFileThread()
{
	SOCKET s=SocketListen(8003);
	if(s==NULL)
	{
		ReceiveThreadIsCreated=false;
		return;
	}
	ReceiveThreadIsCreated=true;

	SOCKADDR_IN addrClient;
	int len=sizeof(SOCKADDR);
	printf("\n�ȴ��ͻ��˵�����");
	char mess[M];
	while(1)
	{
		SOCKADDR_IN addrClient;
		SOCKET s_d=accept(s,(sockaddr*)&addrClient,&len); //5���׽��ֵȴ�����:�����������ˣ� 
		Thread t((ThreadFun)ReceiveFile,(void*)s_d);
		t.Start();
	}
}
void StartThreadSendBroadCast()
{
	Thread t((ThreadFun)SendB,(void*)NULL);
	t.Start();
}
void StartThreadRecvBroadCast()
{
	Thread t((ThreadFun)RecvB,(void*)NULL);
	t.Start();
}
/*
void StartThreadImageCast()
{
	Thread t((ThreadFun)CastThread,(void*)NULL);
	t.Start();
}*/
void StartReceiveFileThread()
{
	Thread t((ThreadFun)ReceiveFileThread,(void*)NULL);
	t.Start();
}
void StartSendFileThread(SendFileBlock* sbf)
{
	Thread t((ThreadFun)SendFileThread,(void*)sbf);
	t.Start();
}