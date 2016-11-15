#include "stdafx.h"
#include <windows.h>
#include <shlobj.h>
#include <combaseapi.h>
#include <emmintrin.h>
#include <defs.h>
#include <stdarg.h>


//SetAppAsDefault
HRESULT CheckStuff(__out BOOL* pfHasDotHTM)
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	IApplicationAssociationRegistration* pAAR;

	HRESULT hr = CoCreateInstance(CLSID_ApplicationAssociationRegistration,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IApplicationAssociationRegistration),
		(void**)&pAAR);

	if (SUCCEEDED(hr))
	{
		hr = pAAR->QueryAppIsDefault(L".html",
			AT_FILEEXTENSION, AL_EFFECTIVE,
			L"FirefoxHTML",
			pfHasDotHTM);
		hr = pAAR->QueryAppIsDefault(L".html",
			AT_FILEEXTENSION, AL_EFFECTIVE,
			L"Firefox",
			pfHasDotHTM);
		pAAR->Release();
	}
	CoUninitialize();
	return hr;
}

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

//用MMX指令实现的内存拷贝函数，相比于memcpy函数可以提高50％的效率   
void  qmemcpy(void   *dst, void   *src, int   nBytes)
{
#define   CACHEBLOCK   1024   //一个块中有多少QWORDs      
#define   SIZEOFQWORD  8      //一个QWORD几个字节   

	//修改此值有可能实现更高的速度      
	int   nQWORDs = nBytes / SIZEOFQWORD;
	int   n = ((int)(nQWORDs / CACHEBLOCK))*CACHEBLOCK;
	int   m = nBytes - n*SIZEOFQWORD;    //还剩下多少个Bytes   
	if (n)
	{
		_asm   //下面先拷贝整数个块      
		{
			mov   esi, src
			mov   edi, dst
			mov   ecx, n//要拷贝多少个块      
			lea   esi, [esi + ecx * 8]
			lea   edi, [edi + ecx * 8]
			neg   ecx
			mainloop :
			mov   eax, CACHEBLOCK / 16
				prefetchloop :
				mov   ebx, [esi + ecx * 8]   //预读此循环      
				mov   ebx, [esi + ecx * 8 + 64] //预读下循环      
				add   ecx, 16
				dec   eax
				jnz   prefetchloop
				sub   ecx, CACHEBLOCK
				mov   eax, CACHEBLOCK / 8
				writeloop:
			movq   mm0, qword   ptr[esi + ecx * 8]
				movq   mm1, qword   ptr[esi + ecx * 8 + 8]
				movq   mm2, qword   ptr[esi + ecx * 8 + 16]
				movq   mm3, qword   ptr[esi + ecx * 8 + 24]
				movq   mm4, qword   ptr[esi + ecx * 8 + 32]
				movq   mm5, qword   ptr[esi + ecx * 8 + 40]
				movq   mm6, qword   ptr[esi + ecx * 8 + 48]
				movq   mm7, qword   ptr[esi + ecx * 8 + 56]

				movntq   qword   ptr[edi + ecx * 8], mm0
				movntq   qword   ptr[edi + ecx * 8 + 8], mm1
				movntq   qword   ptr[edi + ecx * 8 + 16], mm2
				movntq   qword   ptr[edi + ecx * 8 + 24], mm3
				movntq   qword   ptr[edi + ecx * 8 + 32], mm4
				movntq   qword   ptr[edi + ecx * 8 + 40], mm5
				movntq   qword   ptr[edi + ecx * 8 + 48], mm6
				movntq   qword   ptr[edi + ecx * 8 + 56], mm7
				add   ecx, 8
				dec   eax
				jnz   writeloop
				or ecx, ecx
				jnz   mainloop

				sfence
				emms
		}
	}

	//拷贝不足一个块的数据   
	memcpy((char*)dst + nBytes - m, (char*)src + nBytes - m, m);

}

void *__cdecl memmove_0(void *a1, const void *a2, size_t a3)
{
	char *v3; // esi@1
	size_t v4; // ecx@1
	char *v5; // edi@1
	__m128i v6; // xmm1@14
	__m128i v7; // xmm1@17
	int v8; // esi@17
	__m128i v9; // xmm3@18
	__m128i v10; // xmm0@18
	__m128i v11; // xmm5@18
	int v12; // esi@19
	__m128i v13; // xmm1@20
	int v14; // esi@20
	__m128i v15; // xmm3@21
	__m128i v16; // xmm0@21
	__m128i v17; // xmm5@21
	__m128i v18; // xmm1@23
	int v19; // esi@23
	__m128i v20; // xmm3@24
	__m128i v21; // xmm0@24
	__m128i v22; // xmm5@24
	__m128i v23; // xmm1@27
	int v24; // eax@29
	size_t v25; // ecx@34
	void *result; // eax@48
	char *v27; // esi@52
	char *v28; // edi@52
	size_t v29; // ecx@53
	int v30; // eax@64
	unsigned int v31; // edx@65
	int v32; // ecx@65
	unsigned int j; // edx@65
	__m128i v34; // xmm1@66
	__m128i v35; // xmm2@66
	__m128i v36; // xmm3@66
	__m128i v37; // xmm5@66
	__m128i v38; // xmm6@66
	__m128i v39; // xmm7@66
	unsigned int k; // edx@68
	int v41; // ecx@70
	char v42; // al@71
	unsigned int l; // ecx@71
	int m; // ecx@73
	unsigned int v45; // eax@76
	int v46; // ecx@76
	unsigned int i; // eax@78
	size_t v48; // [sp-8h] [bp-10h]@76

	v3 = (char *)a2;
	v4 = a3;
	v5 = (char *)a1;
	if (a1 > a2 && a1 < (char *)a2 + a3)
	{
		v27 = (char *)a2 + a3 - 4;
		v28 = (char *)a1 + a3 - 4;
		if ((unsigned __int8)v28 & 3)
		{
			if (a3 >= 4)
				JUMPOUT(__CS__, *((DWORD *)&ByteCopyDown_0 + ((unsigned __int8)v28 & 3) + 1));
			switch (a3)
			{
			case 0u:
			TrailDown0_0:
				result = a1;
				break;
			case 1u:
			TrailDown1_0:
				v28[3] = v27[3];
				result = a1;
				break;
			case 2u:
			TrailDown2_0:
				v28[3] = v27[3];
				v28[2] = v27[2];
				result = a1;
				break;
			case 3u:
				goto TrailDown3_0;
			}
		}
		else
		{
			v29 = a3 >> 2;
			if (a3 >> 2 < 8)
			{
				switch (-v29)
				{
				case 0u:
					break;
				}
			}
			else
			{
				qmemcpy(v28, v27, 4 * v29);
				v27 += 4 * v29;
				v28 += 4 * v29;
			}
			switch (a3 & 3)
			{
			case 0u:
				goto TrailDown0_0;
			case 1u:
				goto TrailDown1_0;
			case 2u:
				goto TrailDown2_0;
			case 3u:
				break;
			}
		TrailDown3_0:
			v28[3] = v27[3];
			v28[2] = v27[2];
			v28[1] = v27[1];
			result = a1;
		}
	}
	else
	{
		if (_bittest((const signed __int32 *)&dword_43275798, 1u))
		{
			qmemcpy(a1, a2, a3);
			return a1;
		}
		if (a3 < 0x80)
			goto Dword_align_0;
		if (((unsigned int)a2 ^ (unsigned int)a1) & 0xF || !_bittest((const signed __int32 *)&dword_4326A024, 1u))
		{
			if (_bittest((const signed __int32 *)&dword_43275798, 0))
			{
				if (!((unsigned __int8)a1 & 3))
				{
					if (!((unsigned __int8)a2 & 3))
					{
						if (_bittest((const long *)&v5, 2u))
						{
							v4 = a3 - 4;
							v3 = (char *)a2 + 4;
							*(DWORD *)a1 = *(DWORD *)a2;
							v5 = (char *)a1 + 4;
						}
						if (_bittest((const unsigned int *)&v5, 3u))
						{
							v6 = _mm_loadl_epi64((const __m128i *)v3);
							v4 -= 8;
							v3 += 8;
							_mm_storel_epi64((__m128i *)v5, v6);
							v5 += 8;
						}
						if ((unsigned __int8)v3 & 7)
						{
							if (_bittest((const long *)&v3, 3u))
							{
								v7 = _mm_load_si128((const __m128i *)(v3 - 12));
								v8 = (int)(v3 - 12);
								do
								{
									v9 = _mm_load_si128((const __m128i *)(v8 + 16));
									v4 -= 48;
									v10 = _mm_load_si128((const __m128i *)(v8 + 32));
									v11 = _mm_load_si128((const __m128i *)(v8 + 48));
									v8 += 48;
									_mm_store_si128((__m128i *)v5, _mm_alignr_epi8(v9, v7, 12));
									_mm_store_si128((__m128i *)v5 + 1, _mm_alignr_epi8(v10, v9, 12));
									v7 = v11;
									_mm_store_si128((__m128i *)v5 + 2, _mm_alignr_epi8(v11, v10, 12));
									v5 += 48;
								} while ((signed int)v4 >= 48);
								v12 = v8 + 12;
							}
							else
							{
								v18 = _mm_load_si128((const __m128i *)(v3 - 4));
								v19 = (int)(v3 - 4);
								do
								{
									v20 = _mm_load_si128((const __m128i *)(v19 + 16));
									v4 -= 48;
									v21 = _mm_load_si128((const __m128i *)(v19 + 32));
									v22 = _mm_load_si128((const __m128i *)(v19 + 48));
									v19 += 48;
									_mm_store_si128((__m128i *)v5, _mm_alignr_epi8(v20, v18, 4));
									_mm_store_si128((__m128i *)v5 + 1, _mm_alignr_epi8(v21, v20, 4));
									v18 = v22;
									_mm_store_si128((__m128i *)v5 + 2, _mm_alignr_epi8(v22, v21, 4));
									v5 += 48;
								} while ((signed int)v4 >= 48);
								v12 = v19 + 4;
							}
						}
						else
						{
							v13 = _mm_load_si128((const __m128i *)(v3 - 8));
							v14 = (int)(v3 - 8);
							do
							{
								v15 = _mm_load_si128((const __m128i *)(v14 + 16));
								v4 -= 48;
								v16 = _mm_load_si128((const __m128i *)(v14 + 32));
								v17 = _mm_load_si128((const __m128i *)(v14 + 48));
								v14 += 48;
								_mm_store_si128((__m128i *)v5, _mm_alignr_epi8(v15, v13, 8));
								_mm_store_si128((__m128i *)v5 + 1, _mm_alignr_epi8(v16, v15, 8));
								v13 = v17;
								_mm_store_si128((__m128i *)v5 + 2, _mm_alignr_epi8(v17, v16, 8));
								v5 += 48;
							} while ((signed int)v4 >= 48);
							v12 = v14 + 8;
						}
						while ((signed int)v4 >= 16)
						{
							v23 = _mm_loadu_si128((const __m128i *)v12);
							v4 -= 16;
							v12 += 16;
							_mm_store_si128((__m128i *)v5, v23);
							v5 += 16;
						}
						if (_bittest(&v4, 2u))
						{
							v24 = *(DWORD *)v12;
							v4 -= 4;
							v12 += 4;
							*(DWORD *)v5 = v24;
							v5 += 4;
						}
						if (_bittest(&v4, 3u))
						{
							v4 -= 8;
							_mm_storel_epi64((__m128i *)v5, _mm_loadl_epi64((const __m128i *)v12));
						}
						JUMPOUT(__CS__, *(&off_4321B2D8 + v4));
					}
					goto Dword_align_Ok_0;
				}
				goto CopyLeadUp_0;
			}
		Dword_align_0:
			if (!((unsigned __int8)a1 & 3))
			{
			Dword_align_Ok_0:
				v25 = a3 >> 2;
				if (a3 >> 2 < 8)
				{
					switch (v25)
					{
					case 0u:
						goto LABEL_36;
					case 1u:
						goto UnwindUp1_0;
					case 2u:
						goto UnwindUp2_0;
					case 3u:
						goto UnwindUp3_0;
					case 4u:
						goto UnwindUp4_0;
					case 5u:
						goto UnwindUp5_0;
					case 6u:
						break;
					case 7u:
						*((DWORD *)a1 + v25 - 7) = *((DWORD *)a2 + v25 - 7);
						break;
					}
					*((DWORD *)a1 + v25 - 6) = *((DWORD *)a2 + v25 - 6);
				UnwindUp5_0:
					*((DWORD *)a1 + v25 - 5) = *((DWORD *)a2 + v25 - 5);
				UnwindUp4_0:
					*((DWORD *)a1 + v25 - 4) = *((DWORD *)a2 + v25 - 4);
				UnwindUp3_0:
					*((DWORD *)a1 + v25 - 3) = *((DWORD *)a2 + v25 - 3);
				UnwindUp2_0:
					*((DWORD *)a1 + v25 - 2) = *((DWORD *)a2 + v25 - 2);
				UnwindUp1_0:
					*((DWORD *)a1 + v25 - 1) = *((DWORD *)a2 + v25 - 1);
					v3 = (char *)a2 + 4 * v25;
					v5 = (char *)a1 + 4 * v25;
				}
				else
				{
					qmemcpy(a1, a2, 4 * v25);
					v3 = (char *)a2 + 4 * v25;
					v5 = (char *)a1 + 4 * v25;
				}
			LABEL_36:
				switch (a3 & 3)
				{
				case 0u:
					return a1;
				case 1u:
					*v5 = *v3;
					result = a1;
					break;
				case 2u:
					*v5 = *v3;
					v5[1] = v3[1];
					result = a1;
					break;
				case 3u:
					*v5 = *v3;
					v5[1] = v3[1];
					v5[2] = v3[2];
					result = a1;
					break;
				}
				return result;
			}
		CopyLeadUp_0:
			if (a3 >= 4)
				JUMPOUT(__CS__, *((DWORD *)&CopyUnwindUp_0 + ((unsigned __int8)a1 & 3) + 1));
			JUMPOUT(__CS__, *((DWORD *)&TrailUp0_0 + a3 - 4));
		}
		v30 = (unsigned __int8)a2 & 0xF;
		if ((unsigned __int8)a2 & 0xF)
		{
			v48 = a3 - (16 - v30);
			v45 = 16 - v30;
			v46 = v45 & 3;
			if (v45 & 3)
			{
				do
				{
					*v5++ = *v3++;
					--v46;
				} while (v46);
			}
			for (i = v45 >> 2; i; --i)
			{
				*(DWORD *)v5 = *(DWORD *)v3;
				v3 += 4;
				v5 += 4;
			}
			v4 = v48;
		}
		v31 = v4;
		v32 = v4 & 0x7F;
		for (j = v31 >> 7; j; --j)
		{
			v34 = _mm_load_si128((const __m128i *)v3 + 1);
			v35 = _mm_load_si128((const __m128i *)v3 + 2);
			v36 = _mm_load_si128((const __m128i *)v3 + 3);
			_mm_store_si128((__m128i *)v5, _mm_load_si128((const __m128i *)v3));
			_mm_store_si128((__m128i *)v5 + 1, v34);
			_mm_store_si128((__m128i *)v5 + 2, v35);
			_mm_store_si128((__m128i *)v5 + 3, v36);
			v37 = _mm_load_si128((const __m128i *)v3 + 5);
			v38 = _mm_load_si128((const __m128i *)v3 + 6);
			v39 = _mm_load_si128((const __m128i *)v3 + 7);
			_mm_store_si128((__m128i *)v5 + 4, _mm_load_si128((const __m128i *)v3 + 4));
			_mm_store_si128((__m128i *)v5 + 5, v37);
			_mm_store_si128((__m128i *)v5 + 6, v38);
			_mm_store_si128((__m128i *)v5 + 7, v39);
			v3 += 128;
			v5 += 128;
		}
		if (v32)
		{
			for (k = (unsigned int)v32 >> 4; k; --k)
			{
				_mm_store_si128((__m128i *)v5, _mm_load_si128((const __m128i *)v3));
				v3 += 16;
				v5 += 16;
			}
			v41 = v32 & 0xF;
			if (v41)
			{
				v42 = v41;
				for (l = (unsigned int)v41 >> 2; l; --l)
				{
					*(DWORD *)v5 = *(DWORD *)v3;
					v3 += 4;
					v5 += 4;
				}
				for (m = v42 & 3; m; --m)
					*v5++ = *v3++;
			}
		}
		result = a1;
	}
	return result;
}

int sub_43202680(void *This, char a2, int a3)
{
	void *v3; // esi@1
	const void *v4; // ebx@3
	int result; // eax@6

	v3 = This;
	if (a2 && *((DWORD *)This + 5) >= 8u)
	{
		v4 = *(const void **)This;
		if (a3)
			memmove_0(This, v4, 2 * a3);
		free((void *)v4);
	}
	*((DWORD *)v3 + 4) = a3;
	result = 0;
	*((DWORD *)v3 + 5) = 7;
	*((WORD *)v3 + a3) = 0;
	return result;
}

void * GetBrowserStruct(DWORD This, wchar_t *lpValueName)
{
	size_t StrLen; // ST08_4@3
	void *p; // [sp+4h] [bp-4h]@1

	p = (void *)This;
	*(DWORD *)(This + 16) = 0;
	*(DWORD *)(This + 20) = 0;
	sub_43202680((void *)This, 0, 0);
	if (*lpValueName)
	{
		StrLen = wcslen(lpValueName);
		sub_43210C70(p, StrLen, lpValueName, StrLen);
	}
	else
	{
		sub_43210C70(p, 0, lpValueName, 0);
	}
	return p;
}

void test()
{
	// Included because they aren't defined in mingw for some reason.
	const GUID CLSID_ApplicationAssociationRegistration =
	{ 0x591209C7, 0x767B, 0x42B2,{ 0x9f,0xba,0x44,0xee,0x46,0x15,0xf2,0xc7 } };

	const IID IID_ApplicationAssociationRegistration =
	{ 0x4E530B0A, 0xE611, 0x4C77,{ 0xa3,0xac,0x90,0x31,0xd0,0x22,0x28,0x1b } };

	const IID IID2_ApplicationAssociationRegistration =
	{ 0x229D59E2, 0xF94A, 0x402E,{ 0x9a,0x9f,0x3b,0x84,0xa1,0xac,0xed,0x77 } };

	const IID IID3_ApplicationAssociationRegistration =
	{ 0xC7225171, 0xB9A7, 0x4CF7,{ 0x86,0x1F,0x85,0xAB,0x7B,0xA3,0xC5,0xB2 } };

	auto hRet = 0;
	auto ifA = false;
	auto cInit = CoInitializeEx(NULL, NULL);
	auto BrowserStruct = 0;

	GetBrowserStruct((DWORD)&BrowserStruct, L"2345Explorer");

	IApplicationAssociationRegistration* pAAR = nullptr;/*ppv*/
	IApplicationAssociationRegistration* pAAR2 = nullptr;/*v11*/
	IApplicationAssociationRegistration* pAAR3 = nullptr;/*v10*/
	IApplicationAssociationRegistration* pAAR4 = nullptr;/*v9*/

	HRESULT hr = CoCreateInstance(CLSID_ApplicationAssociationRegistration,
		NULL,
		3,
		IID_ApplicationAssociationRegistration,
		(void**)&pAAR);
	if (hr >= 0 && pAAR)
	{
		//RPCRT4.IUnknown_QueryInterface_Proxy
		//hr = pAAR->QueryInterface(IID2_ApplicationAssociationRegistration, (void**)&pAAR2);
		hr = (**(int(__stdcall ***)(PVOID, PVOID, PVOID))pAAR)(pAAR, (PVOID)&IID2_ApplicationAssociationRegistration, &pAAR2);
		if (!FAILED(hr)) {
			//combase.ObjectStublessClient6
			hr = (*(int(__stdcall **)(PVOID, PVOID))(*(DWORD *)pAAR2 + 0x18))(pAAR2, "2345Explorer");
			if (!FAILED(hr)) {
				ifA = true;
			}
			//RPCRT4.IUnknown_Release_Proxy
			if (pAAR2)
				(*(void(__stdcall **)(PVOID))(*(DWORD *)pAAR2 + 0x08))(pAAR2);
			pAAR2 = 0;
		}

		if (!ifA){
			hr = (**(int(__stdcall ***)(PVOID, PVOID, PVOID))pAAR)(pAAR, (PVOID)&IID3_ApplicationAssociationRegistration, &pAAR3);
			if (!FAILED(hr)) {
				hr = (*(int(__stdcall **)(PVOID, PVOID))(*(DWORD *)pAAR3 + 24))(pAAR3, "2345Explorer");
				if (!FAILED(hr)) {
					ifA = true;
				}
				hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 20))(pAAR, 1, "2345Explorer", &hRet);
				if (pAAR3)
					(*(void(__stdcall **)(PVOID))(*(DWORD *)pAAR3 + 8))(pAAR3);
				pAAR3 = 0;
			}


		}

		if (!ifA) {
			hr = (**(int(__stdcall ***)(PVOID, PVOID, PVOID))pAAR)(pAAR, (PVOID)&IID3_ApplicationAssociationRegistration, &pAAR4);
			if (!FAILED(hr)) {
				hr = (*(int(__stdcall **)(PVOID, PVOID))(*(DWORD *)pAAR4 + 20))(pAAR4, "2345Explorer");
				if (!FAILED(hr)) {
					ifA = true;
				}
				hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 20))(pAAR, 1, "2345Explorer", &hRet);
				if (pAAR4)
					(*(void(__stdcall **)(PVOID))(*(DWORD *)pAAR4 + 8))(pAAR4);
				pAAR4 = 0;
			}
		}
		//combase.#4_ObjectStublessClient5
		hr = (*(int(__stdcall **)(PVOID, DWORD, PVOID, PVOID))(*(DWORD *)pAAR + 0x14))(pAAR, 1, "2345Explorer", &hRet);
		//RPCRT4.IUnknown_Release_Proxy
		if (pAAR)
			(*(void(__stdcall **)(LPVOID))(*(DWORD *)pAAR + 8))(pAAR);
		pAAR = 0;
	}
	
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
	wchar_t *Name2 = L"2345加速浏览器";
	wchar_t *html = L"open";
	//set(ExePath, Name, Name2, html);
	MessageBox(0, L"", L"", 0);
	SHChangeNotify(0x8000000, 0, 0, 0);
	SHChangeNotify(0x8000000, 0x3000u, 0, 0);
	test();
    return 0;
}

