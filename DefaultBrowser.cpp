#include "stdafx.h"
#include <windows.h>
#include <shlobj.h>
#include <combaseapi.h>
#include <emmintrin.h>
#include <stdarg.h>
#include <vector>

void set(wchar_t * ExePath, wchar_t * Name, wchar_t * Name2, wchar_t * html)
{
	HMODULE result = LoadLibraryW(L"MxTool.dll");
	PVOID v8 = GetProcAddress(result, "MxTool_RemoveDefaultBrowser");
	//MxTool_RemoveDefaultBrowser(ExePath, Name, html, true);
	//if (v8)
	//	((void(__cdecl *)(wchar_t *, wchar_t*, wchar_t*, signed int))v8)(ExePath, Name, html, 1);
	PVOID v9 = GetProcAddress(result, "MxTool_SetDefaultBrowser");
	wchar_t *dll = L"C:\\Program Files\\Maxthon\\Bin\\MxRsc.dll";
	if (v9)
		((void(__cdecl *)(int, wchar_t *, wchar_t*, wchar_t*, wchar_t *, int, int, wchar_t *, int, int))v9)(1, ExePath, Name, Name2, dll, 1, 0, html, 1, 0);
}

void SetDefaultBrowser(wchar_t * lpValueName)
{
	// Included because they aren't defined in mingw for some reason.
	auto hRet = 0;
	auto ifA = false;
	auto cInit = CoInitializeEx(NULL, NULL);
	auto BrowserStruct = 0;
	IApplicationAssociationRegistration* pAAR = nullptr;/*ppv*/
	IApplicationAssociationRegistration* pAAR2 = nullptr;/*v11*/
	IApplicationAssociationRegistration* pAAR3 = nullptr;/*v10*/
	IApplicationAssociationRegistration* pAAR4 = nullptr;/*v9*/

	typedef struct MyStruct
	{
		std::wstring name;
		int Value;
	};

	std::vector <MyStruct> name = {
		{ L".html",0},
		{ L".htm",0 },
		{ L".mht",0 },
		{ L".mhtml",0 },
		{ L".shtml",0 },
		{ L".xht",0 },
		{ L".xhtml",0 },
		{ L"http",1 },
		{ L"https",1 },
		{ L"ftp",0 },
	};

	const GUID CLSID_ApplicationAssociationRegistration =
	{ 0x591209C7, 0x767B, 0x42B2,{ 0x9f,0xba,0x44,0xee,0x46,0x15,0xf2,0xc7 } };

	const IID IID_ApplicationAssociationRegistration =
	{ 0x4E530B0A, 0xE611, 0x4C77,{ 0xa3,0xac,0x90,0x31,0xd0,0x22,0x28,0x1b } };

	const IID IID2_ApplicationAssociationRegistration =
	{ 0x229D59E2, 0xF94A, 0x402E,{ 0x9a,0x9f,0x3b,0x84,0xa1,0xac,0xed,0x77 } };

	const IID IID3_ApplicationAssociationRegistration =
	{ 0x1c5c9d10, 0x1225, 0x4c97,{ 0x8c,0x51,0x98,0xe1,0xb6,0xf0,0xd4,0xe0 } };

	auto hr = CoCreateInstance(CLSID_ApplicationAssociationRegistration,
		NULL,
		3,
		IID_ApplicationAssociationRegistration,
		(void**)&pAAR);

	if (hr >= 0 && pAAR)
	{
		hr = pAAR->QueryInterface(IID2_ApplicationAssociationRegistration, (void**)&pAAR2);
		if (!FAILED(hr)) {
			//combase.ObjectStublessClient6
			hr = (*(int(__stdcall **)(PVOID, PVOID))(*(DWORD *)pAAR2 + 0x18))(pAAR2, lpValueName);
			if (!FAILED(hr)) {
				std::vector<MyStruct>::iterator it;
				for (it = name.begin(); it != name.end(); it++) {
					hr = (*(int(__stdcall **)(PVOID, PVOID, const wchar_t*, DWORD))(*(DWORD *)pAAR2 + 0x14))(pAAR2, lpValueName, it->name.c_str(), it->Value);
				}
				ifA = true;
			}
		}

		if (!ifA) {
			hr = pAAR->QueryInterface(IID3_ApplicationAssociationRegistration, (void**)&pAAR2);
			if (!FAILED(hr)) {
				
				std::vector<MyStruct>::iterator it;
				for (it = name.begin(); it != name.end(); it++) {
					hr = (*(int(__stdcall **)(PVOID, PVOID, const wchar_t*, DWORD))(*(DWORD *)pAAR2 + 0x14))(pAAR2, lpValueName, it->name.c_str(), it->Value);
					if (FAILED(hr)) {
						auto i = 0;
					}
				}
				ifA = true;
			}
		}
		if (pAAR2)
			pAAR2->Release();
		pAAR2 = 0;
	}
//	const GUID CLSID_ApplicationAssociationRegistration =
//	{ 0x591209C7, 0x767B, 0x42B2,{ 0x9f,0xba,0x44,0xee,0x46,0x15,0xf2,0xc7 } };
//
//	const IID IID_ApplicationAssociationRegistration =
//	{ 0x4E530B0A, 0xE611, 0x4C77,{ 0xa3,0xac,0x90,0x31,0xd0,0x22,0x28,0x1b } };
//
//	const IID IID2_ApplicationAssociationRegistration =
//	{ 0x229D59E2, 0xF94A, 0x402E,{ 0x9a,0x9f,0x3b,0x84,0xa1,0xac,0xed,0x77 } };
//
//	const IID IID3_ApplicationAssociationRegistration =
//	{ 0xC7225171, 0xB9A7, 0x4CF7,{ 0x86,0x1F,0x85,0xAB,0x7B,0xA3,0xC5,0xB2 } };
//
//	auto hr = CoCreateInstance(CLSID_ApplicationAssociationRegistration,
//		NULL,
//		3,
//		IID_ApplicationAssociationRegistration,
//		(void**)&pAAR);
//	if (hr >= 0 && pAAR)
//	{
//		hr = pAAR->QueryInterface(IID2_ApplicationAssociationRegistration, (void**)&pAAR2);
//		if (!FAILED(hr)) {
//			//combase.ObjectStublessClient6
//			hr = (*(int(__stdcall **)(PVOID, PVOID))(*(DWORD *)pAAR2 + 0x18))(pAAR2, lpValueName);
//			if (!FAILED(hr)) {
//				ifA = true;
//			}
//			if (pAAR2)
//				pAAR2->Release();
//			pAAR2 = 0;
//		}
//
//		if (!ifA) {
//			hr = pAAR->QueryInterface(IID3_ApplicationAssociationRegistration, (void**)&pAAR3);
//			if (!FAILED(hr)) {
//				hr = (*(int(__stdcall **)(PVOID, PVOID))(*(DWORD *)pAAR3 + 0x18))(pAAR3, lpValueName);
//				if (!FAILED(hr)) {
//					ifA = true;
//				}
//				hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 0x14))(pAAR, 1, lpValueName, &hRet);
//				if (pAAR3)
//					(*(void(__stdcall **)(PVOID))(*(DWORD *)pAAR3 + 0x08))(pAAR3);
//				pAAR3 = 0;
//			}
//		}
//
//		if (!ifA) {
//			hr = pAAR->QueryInterface(IID3_ApplicationAssociationRegistration, (void**)&pAAR4);
//			if (!FAILED(hr)) {
//				hr = (*(int(__stdcall **)(PVOID, PVOID))(*(DWORD *)pAAR4 + 0x14))(pAAR4, lpValueName);
//				if (!FAILED(hr)) {
//					ifA = true;
//				}
//				hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 0x14))(pAAR, 1, lpValueName, &hRet);
//				if (pAAR4)
//					(*(void(__stdcall **)(PVOID))(*(DWORD *)pAAR4 + 0x08))(pAAR4);
//				pAAR4 = 0;
//			}
//		}
//		//combase.#4_ObjectStublessClient5
//		hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 0x14))(pAAR, 1, lpValueName, &hRet);
//		if (pAAR)
//			pAAR->Release();
//		pAAR = 0;
//	}
	if (cInit >= 0)
		CoUninitialize();
	return;
}

int main()
{
	LoadLibraryW(L"clbcatq.dll");
	/*wchar_t *ExePath = L"C:\\Program Files\\Maxthon\\Bin\\Maxthon.exe";
	wchar_t *Name = L"Maxthon3";
	wchar_t *Name2 = L"Maxthon Cloud Browser";
	wchar_t *html = L"Max3.Association.HTML";
	set(ExePath, Name, Name2, html);*/
	wchar_t *ExePath = L"C:\\Program Files\\2345Soft\\2345Explorer\\2345Explorer.exe";
	wchar_t *Name = L"2345Explorer";
	wchar_t *Name2 = L"2345º”ÀŸ‰Ø¿¿∆˜";
	wchar_t *html = L"open";
	//set(ExePath, Name, Name2, html);
	//MessageBox(0, L"", L"", 0);
	//SHChangeNotify(0x8000000, 0, 0, 0);
	//SHChangeNotify(0x8000000, 0x3000u, 0, 0);
	SetDefaultBrowser(L"QQBrowser"/*L"2345Explorer"*/);
    return 0;
}

