// MyCmd.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
#include <tchar.h>

//尝试提交
int main(int argc, char* argv[])
{
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	TCHAR szCmdline[256] = _T("/c calc");

	BOOL bRet = CreateProcessA(
	    _T("C:\\Windows\\SysWOW64\\cmd.exe"),
		szCmdline,
		NULL,
		NULL,
		TRUE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si,
		&pi
	);

	return 0;
}
