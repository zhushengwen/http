// Tools.h: interface for the Tools class.
//
//////////////////////////////////////////////////////////////////////
#ifndef MY_H_FILE
#define MY_H_FILE 
#include "common.h"
#endif
#pragma comment(lib, "CxImage.lib")
#pragma once 
#include "broadcast.h"

extern bool ReceiveThreadIsCreated;
void ListenBroser()
{
	
	delete_folder(get_tempdir());
	FavIcon();
	WORD wVersion=MAKEWORD(1,1); 
	WSADATA wsData; 
	int nResult= WSAStartup(wVersion,&wsData); //1、启动Winsock
	if(nResult !=0) 
	{ 
		printf("启动Winsock失败!\n"); 
	} 

	SOCKET s=socket(AF_INET,SOCK_STREAM,IPPROTO_IP); //2.创建套接字
	if(s==INVALID_SOCKET) 
	{ 
		printf("创建套接字失败!\n"); 
	} 

	SOCKADDR_IN addr; 
	addr.sin_family=AF_INET; 
	addr.sin_port=htons(8003); //保证字节顺序 
	addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	nResult=bind(s,(sockaddr*)&addr,sizeof(sockaddr)); //3、套接字的绑定
	if(nResult==SOCKET_ERROR) 
	{ 
		printf("套接字的绑定失败!\n");
	} 

	nResult=listen(s,5); //最多5个连接， 套接字的监听
	if(nResult==SOCKET_ERROR) 
	{ 
		printf("套接字的监听(服务器端)失败!\n"); 
	} 


	int len=sizeof(SOCKADDR);
	printf("\n等待客户端的连接\n");

	int a=0;
	//while(a<5)										//开始的一个小动态图形
	//{
	//	for(int b=0;b<8;b++)
	//	{
	//		Sleep(100);
	//		printf(" . ");
	//	}
	//	Sleep(100);
	//	system("cls");
	//	printf("\n\r等待客户端的连接");
	//	a++;
	//}
	int count=0;
	while(1)
	{
		cout<<++count<<endl;
		SOCKADDR_IN addrClient;
		SOCKET s_d=accept(s,(sockaddr*)&addrClient,&len); //5、套接字等待连接:：（服务器端） 
		cout<<inet_ntoa(addrClient.sin_addr)<<endl;


		if(s_d==INVALID_SOCKET) 
		{ 
			printf("套接字等待连接(服务器端)失败!\n");
			closesocket(s_d);
			MessageBox(NULL,"套接字创建失败！","请检查端口",MB_OK);
			return;
		}	
		char mess[1024]={0}; 
		nResult=recv(s_d,mess,1023,0); 	
		char *success="成功登陆服务器!";

		string path=ExtractStr(mess,"GET"," "," ");
		split spp(path);
		
		if(path=="/upimg")
		{
			string heads="<html><head><meta http-equiv=\"refresh\" content=\"3\"><title>2013 ZhuShengWen</title><script type='text/javascript'>function bbimg(o) {\r\n            var zoom = parseInt(o.style.zoom, 10) || 100;\r\n            zoom += event.wheelDelta / 12;\r\n            if (zoom > 0) o.style.zoom = zoom + '%';\r\n            return false;\r\n        };</script></head><body><img id='img' onmousewheel='return bbimg(this)' /> <script>var img=document.getElementById(\"img\").src=\"";
			string tails="\"</script></body></html>";
			string path1=GetNowPath(".jpg");
			CastImage(path1);
			
			string encode="/"+split(path1,"\\").replace("/");

			long slong=encode.length() + heads.length()+tails.length();
			SendHeader("", "",slong, " 200 OK",s_d);
			SendToBrowser(heads.c_str(),s_d,heads.length());

			SendToBrowser(encode.c_str(),s_d,encode.length());

			SendToBrowser(tails.c_str(),s_d,tails.length());
			
		}
		else 
		if(path=="/base64")
		{
			string heads="<html><head><meta http-equiv=\"refresh\" content=\"2\"><title>2013 ZhuShengWen</title><script type='text/javascript'>function bbimg(o) {\r\n            var zoom = parseInt(o.style.zoom, 10) || 100;\r\n            zoom += event.wheelDelta / 12;\r\n            if (zoom > 0) o.style.zoom = zoom + '%';\r\n            return false;\r\n        };</script></head><body><img id='img'onmousewheel='return bbimg(this)' /><script>var img=document.getElementById(\"img\").src=\"data:image/jpeg;base64,";
			string tails="\"</script></body></html>";
			string path1=GetNowPath(".jpg");
			CastImage(path1);
		//	path1="1.jpg";
			ZBase64 zBase;
			long size=0;//得到图像大小

			
			FILE* stream = fopen(path1.c_str(), "rb");
			int ch=1;


			if (stream)
			{
				size = filelength(fileno(stream));
				long slong=((size+2)/3)*3*8/6 +4*((size+2)/3)*3*8/6/76 + heads.length()+tails.length();
				SendHeader("", "",slong, " 200 OK",s_d);
				SendToBrowser(heads.c_str(),s_d,heads.length());

				while(ch!=0)
				{
					char buff[912]={0};
					ch = fread(buff,1,912,stream);
					string encode=zBase.Encode((unsigned char *)buff,ch);//  CConvert::enBase64(string(buff,ch));
					SendToBrowser(encode.c_str(),s_d,encode.length());
				}
				fclose(stream);

				SendToBrowser(tails.c_str(),s_d,tails.length());
			}
		}else
			
			if(path.find("cast.jpg")!=-1)
		{
			string path1=GetNowPath(".jpg");
			CastImage(path1);
			SendFileToWeb(path1,s_d,"image/jpg",true);
			//remove(path1.c_str());
		}
		else if(spp.startwith("IP:"))
		{
			spp.setchars(":");
			if(spp.getcount()==2)
			{
				IPP ipc(spp.at(1));
				string deskp=get_desk_path(ipc.ToString());
				create_folder(deskp);
				//创建接收线程（标志）
				if(!ReceiveThreadIsCreated)
				{
					ReceiveThreadIsCreated=true;
					//StartReceiveFileThread();
				}
			}
		}
		else if(path==":SENDDIRS")
		{
			string tips="RECVDIRS ";
			int nResult=send(s_d,tips.c_str(),tips.length()+1,0);
			if(nResult!=-1)
			{
				Thread t((ThreadFun)ReceiveFile,(void*)s_d);
				t.Start();
				continue;
			}
		}
		else
		{
			IniIcon();
			strCoding sCd;
			path=sCd.UrlUTF8Decode(path.c_str());
			path=split(path,"\\").replace("/");
			path=split(path).trim("/",1);
			if(path=="desktop")
			{
				path=get_desk_path();
			}
			//OutputDebugString(path.c_str());

			//LPCSTR pBuffer=.pt;
			string temp_path=combine_path(get_tempdir(),path);
			if(file_exists(path))
			{
				SendFileToWeb(path,s_d);
			}
			else if(file_exists(temp_path))
			{
				if(split(temp_path.c_str()).endwith(".png"))
				{
					SendFileToWeb(temp_path,s_d,"image/png",true);
				}else
				if(split(temp_path.c_str()).endwith(".css"))
				{
					SendFileToWeb(temp_path,s_d,"text/css",true);
				}else
					SendFileToWeb(temp_path,s_d);
			}
			else 
			{
				bool c=true;
				if(path=="?")
				{
					SendFileToWeb(get_curr_path(),s_d);
					c=false;
				}

				split sppd(path);
				bool a=sppd.endwith("/");
				bool b=folder_exists(path);
				if( a&&b&&c )
				{
					path=sppd.trim("/",2);
					if( !TestSendCreateDirCommand(inet_ntoa(addrClient.sin_addr)))
					{
					SendFileToWeb(get_curr_path(),s_d);
					c=false;
					}
					else
					{
						//创建线程

						//发送文件
						//文件名为 path；
						SendFileBlock* sfb=new	SendFileBlock();
						sfb->filepath=path;
						sfb->ip=IPP(inet_ntoa(addrClient.sin_addr));
						string destin="文件正在输送。。。";
						StartSendFileThread(sfb);
						//SendHeader("", "",destin.length(), " 200 OK",s_d);
						//SendToBrowser(destin.c_str(), s_d);
					}
					
				}
				if(c)
				{
				string destin;
				string sender;
				string header_schema="<a {TA} tag='host' title=\"按空格固定图像\"  href=\"http://%s:8003/\"><b>%s:</b>%s</a>&nbsp;";
				//onmouseover='aa(this,event || window.event)' onmouseout='bb(this)'
				vector<IPP> vip=AddOrUpDateIPOrGetOnline();

				int ih=1;
				split sh(header_schema,"{TA}");
				string header;
				string temp;
				char bc[10]={0};
				CString str;

				temp=sh.replace("class=\"home\"");
				char tempb[16]={0};
				IPP ip1(CHttpSocket::GetIP(tempb));
				str.Format(temp.c_str(),ip1.ToString().c_str(),itoa(ih,bc,10),ip1.ToString().c_str());
				header+=str.GetBuffer();
				ih++;

				for(vector<IPP>::iterator it1  = vip.begin(); it1 != vip.end(); ++it1)  
				{ 

					IPP ipv(inet_ntoa(addrClient.sin_addr));
					string strip=ipv.ToString();
					if(ipv==*it1)
					{
						temp=sh.replace("class=\"forign\"");
					}
					else 
					{
						temp=sh.replace("");
					}
					str.Format(temp.c_str(),(*it1).ToString().c_str(),itoa(ih,bc,10),(*it1).ToString().c_str());
					header+=str.GetBuffer();
					ih++;
				}



				string info_schema="<tr %s><td nowrap><a href=\"/%s\" class=\"dl\" ><img src=\"/%s\" /></a><a  href=\"/%s\">%s</a></td><td nowrap align=\"right\">%s</td><td nowrap>%s</td></tr>";
				DataBlock db=FindResourcePoint(IDR_HTML1,RT_HTML);
				string pBuffer(db.pt,db.size);
				int tri=0;
				if(path!="" && (folder_exists(path) || ListFile(path,false).size()!=0 || ListFile(path).size()!=0))
				{
					string title="<td><a href=\"/\" class=\"dl\" ><img src=\"/drive.png\"/></a>";
					split sp2(path.c_str(),"/");
					if(sp2.getcount()==1)sp2.setchars("\\");
					sp2.trim("\\/");
					int lc=sp2.getcount();
					string path1;
					for(int j=0;j<lc;j++)
					{
						if(j!=0)
						{
							path1+="/";
							title+="/";
						}
						path1+=sp2.at(j);
						title+="<a  href=\"/"+path1+"\">"+sp2.at(j)+"</a>";
					}
					title+="</td>";
					//split sp(pBuffer.c_str());
					vector<string> vs1=ListFile(path,false);
					
					for(vector<string>::iterator it1  = vs1.begin(); it1 != vs1.end(); ++it1)  
					{ 
						//cout<<"\t"<<*(it1)<<endl;
						//tosend+=*(it1);
						CString str;
						string temp=get_file_shortname(*(it1)).c_str();
						str.Format(info_schema.c_str(),
							tri%2?"class=\"d\"":"",
							(*(it1)).c_str(),"folder.png",
							(*(it1)).c_str(),
							temp.c_str(),
							(string("<a style='color:#014133' href=\"/") +(*(it1)).c_str()+ "/\" >下载</a>").c_str(),
							get_filetime(*it1).c_str());
						sender+= str.GetBuffer();
						tri++;
					}
					vs1=ListFile(path);
					for(vector<string>::iterator it1  = vs1.begin(); it1 != vs1.end(); ++it1)  
					{ 
						//cout<<"\t"<<*(it1)<<endl;
						//tosend+=*(it1);
						CString str;
						string temp=get_file_shortname(*(it1)).c_str();
						__int64 length=get_filesize(*(it1));
						char lenc[20]={0};
						ultoa(length,lenc,10);
						string str1=lenc;
						for(int ic=str1.length()%3;ic+2<str1.length();ic+=3)
						{
							if(ic!=0)str1.insert(ic+(ic-1)/3,",");	
						}
						str1+=" 字节";

						split spi((*it1).c_str(),".");
						string dot=".file";
						string icon_path="file.png";
						if(spi.getcount()!=1)
						{
							dot="."+spi.at(spi.getcount()-1);
							icon_path=spi.at(spi.getcount()-1)+".ico";
						}
						string dest_path=icon_path;
						if(spi.getcount()!=1)dest_path=spi.at(spi.getcount()-1)+".png";
						dest_path=combine_path(get_tempdir(),dest_path);
						if(!file_exists(dest_path))
							GetIcon(dot,combine_path(get_tempdir(),icon_path));
						if(spi.getcount()!=1)icon_path=spi.at(spi.getcount()-1)+".png";
						str.Format(info_schema.c_str(),
							tri%2?"class=\"d\"":"",
							(*(it1)).c_str(),icon_path.c_str(),
							(*(it1)).c_str(),
							temp.c_str(),
							str1.c_str(),
							get_filetime(*it1).c_str());
						sender+= str.GetBuffer();
						tri++;
					}
					destin=split(pBuffer.c_str(),"{TD}").replace(sender.c_str());
					destin=split(destin.c_str(),"{TC}").replace(title.c_str());
					destin=split(destin.c_str(),"{TB}").replace(header.c_str());
				}//destin=sCd.UrlUTF8(destin.c_str());
				else
				{
					vector<string> vs=GetComputerDisk();//ListFile("c:\\");
					for(vector<string>::iterator it  = vs.begin(); it != vs.end(); ++it)  
					{ 

						CString str;
						string temp=split((*it).c_str(),"\\").replace("");
						str.Format(info_schema.c_str(),
							tri%2?"class=\"d\"":"",
							(*(it)).c_str(),"drive.png",
							(*(it)).c_str(),
							temp.c_str(),
							"",
							"");
						sender+= str.GetBuffer();
						tri++;
					}
					destin=split(pBuffer.c_str(),"{TD}").replace(sender.c_str());
					destin=split(destin.c_str(),"{TC}").replace("");
					destin=split(destin.c_str(),"{TB}").replace(header.c_str());
				}

				SendHeader("", "",destin.length(), " 200 OK",s_d);
				SendToBrowser(destin.c_str(), s_d);
				}
			}
		}
		closesocket(s_d);
	}
}


