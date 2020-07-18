// MyCmd.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

//1,创建两个匿名管道，CreatePipe
//2,创建进程 cmd.exe
//3，替换CMD的标准输入和输出
//4,向管道的一端输入命令， 注意\n, 不要0结尾
//5,从管道的另一端获取数据
//6,循环显示

int main(int argc, char* argv[])
{
	while (true)
	{
		

		//创建匿名管道
		HANDLE hCmdReadPipe; //CMD读数据
		HANDLE hMyWritePipe; //MY写数据

		HANDLE hMyReadPipe; //my读数据
		HANDLE hCmdWritePipe; //cmd写数据

		//管道开启继承属性
		SECURITY_ATTRIBUTES sa = { 0 };
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.bInheritHandle = TRUE;

		BOOL bRet = CreatePipe(&hCmdReadPipe, &hMyWritePipe, &sa, 0);

		if (!bRet)
		{
			printf("create pipe error");
			return 0;
		}


		bRet = CreatePipe(&hMyReadPipe, &hCmdWritePipe, &sa, 0);

		if (!bRet)
		{
			printf("create pipe error");
			return 0;
		}


		PROCESS_INFORMATION pi = { 0 };
		STARTUPINFO si = { 0 };
		//TCHAR szCmdline[256] = _T("/c calc"); 
		//替换CMD的标准输入输出
		si.cb = sizeof(STARTUPINFO);

		si.dwFlags = STARTF_USESTDHANDLES; //要先开启句柄替换的标志位，才能替换输入输出
		si.hStdInput = hCmdReadPipe;
		si.hStdOutput = hCmdWritePipe;
		si.hStdError = hCmdWritePipe;

			bRet = CreateProcessA(
			_T("C:\\Windows\\SysWOW64\\cmd.exe"),
			NULL,  //命令行参数
			NULL,  //进程继承属性
			NULL,  //线程继承属性
			TRUE,  //继承开关
			CREATE_NO_WINDOW,  //无窗口
			NULL,  //环境变量
			NULL,  //当前路径，程序双击启动时的路径（相对路径）
			&si,   //传出参数，取地址，传入指针
			&pi   //传出参数
		);

		if (!bRet)
		{
			printf("create process error");
			return 0;
		}


	
		//写文件
		char szBuf[256];

		gets_s(szBuf);
		int nLength = strlen(szBuf);
		if (nLength > 255) {
			printf("stack overflow");
		}

		szBuf[nLength++] = '\n';
		unsigned int nWritedLength = 0;
		//memset(szBuf, 0, 256);

		//memcpy(szBuf, "ipconfig\n", nLength);

		bRet = WriteFile(hMyWritePipe,
			szBuf,
			nLength,
			reinterpret_cast<LPDWORD>(&nWritedLength),
			NULL

		);

		if (!bRet)
		{
			printf(" wirte file error");
			return 0;
		}


		//读文件

		while (true) {

			char szOutBuf[256] = { 0 };
			DWORD nReadBytes = 0;
			bRet = ReadFile(hMyReadPipe,
				szOutBuf,
				255,
				&nReadBytes,
				NULL
			);

			if (!bRet)
			{
				printf("read file error");
				return 0;
			}

			printf(szOutBuf);


		}


	}

	
	


	
	
	return 0;
}
