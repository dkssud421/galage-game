/*==========================================================================
 *
 *  Copyright (C) 1995-1997 Microsoft Corporation. All Rights Reserved.
 *
 *  File:       ddutil.cpp
 *  Content:    Routines for loading bitmap and palettes from resources
 *
 ***************************************************************************/
#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <ddraw.h>
#include "ddutil.h"

extern "C" IDirectDrawSurface7 *DDLoadBitmap(IDirectDraw7 *pdd, LPCTSTR szBitmap, int dx, int dy )
{
    HBITMAP              hbm;
    BITMAP               bm;
	//Dierct Draw Surface Description
    DDSURFACEDESC2       ddsd;//그래픽 카드 메모리 표면 정보를 담는 구조체 
    IDirectDrawSurface7 *pdds;
    //dx, dy= 0 이면 원래 비트맵 소스의 폭과 높이를 사용하겠다는 의미 
	//LoadImage : 비트맵 파일의 핸들을 hbm 이 되게 함 
    hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, dx, dy, LR_CREATEDIBSECTION);
    if (hbm == NULL) hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, dx, dy, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
   	if (hbm == NULL) return NULL;
    //hbm 을 이용하여 비트맵 정보를 bm에 넣었다.
    GetObject(hbm, sizeof(bm), &bm);

	//그래픽 카드에 표면을 만들기 위해 표면 정보를 정한다 
	//0으로 초기화 
    ZeroMemory(&ddsd, sizeof(ddsd));
	//구조체 ddsd의 아래 데이터들에  비트맵 정보 데이터를 넣는다 
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH;
    //OFFSCREEN : 그래픽 카드 메모리 주면 및 후면 외의 다른 표면 
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;   
	ddsd.dwWidth  = bm.bmWidth;//비트맵 폭 
    ddsd.dwHeight = bm.bmHeight;

	//pdd : DirectDraw  최상위 데이터 즉 총괄 지휘자 
	//pdd의 멤버 함수 CreateSurface 을 이용하여 표면 정보  ddsd에 맞는 표면 pdds을 생성 
	//offscreen 표면 주소가 pdds 이 되었다.
    if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK) return NULL;

	//이제 비트맵을 위에서 만든 offscreen (시작 주소가  pdds)에 카피해 넣는다   
    DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

	//hbm이 차지하고 있는 메모리 해제 
    DeleteObject(hbm);

	//메모리 offscreen 시작 주소를 반환  
    return pdds;
}

HRESULT DDReLoadBitmap(IDirectDrawSurface7 *pdds, LPCSTR szBitmap)
{
    HBITMAP	hbm;
    HRESULT hr;

    hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), szBitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    if (hbm == NULL) hbm = (HBITMAP) LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    if (hbm == NULL)
    {
        OutputDebugString("handle is null\n");
        return E_FAIL;
    }
    hr = DDCopyBitmap(pdds, hbm, 0, 0, 0, 0);

    if (hr != DD_OK)
    {
        OutputDebugString("ddcopybitmap failed\n");
    }
    DeleteObject(hbm);
    return hr;
}

extern "C" HRESULT DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int x, int y, int dx, int dy)
{
    HDC                 hdcImage;
    HDC                 hdc;
    BITMAP              bm;
    DDSURFACEDESC2      ddsd;
    HRESULT             hr;

    if (hbm == NULL || pdds == NULL) return E_FAIL;

    pdds->Restore();

    hdcImage = CreateCompatibleDC(NULL);

    if ( !hdcImage )
	{
		OutputDebugString(TEXT("createcompatible dc failed\n"));
	}
    SelectObject(hdcImage, hbm);
    GetObject(hbm, sizeof(bm), &bm);
	
	dx = dx == 0 ? bm.bmWidth  : dx;
    dy = dy == 0 ? bm.bmHeight : dy;
 
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    pdds->GetSurfaceDesc(&ddsd);

    if ((hr = pdds->GetDC(&hdc)) == DD_OK)
    {
		StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, dx, dy, SRCCOPY);
		pdds->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);

    return hr;
}

extern "C" IDirectDrawPalette *DDLoadPalette(IDirectDraw7 * pdd, LPCSTR szBitmap)
{
    IDirectDrawPalette     *ddpal;
    int                     i;
    int                     n;
    int                     fh;
    HRSRC                   h;
    LPBITMAPINFOHEADER      lpbi;
    PALETTEENTRY            ape[256];
    RGBQUAD                *prgb;

    for (i = 0; i < 256; i++)
    {
        ape[i].peRed = (BYTE) (((i >> 5) & 0x07) * 255 / 7);
        ape[i].peGreen = (BYTE) (((i >> 2) & 0x07) * 255 / 7);
        ape[i].peBlue = (BYTE) (((i >> 0) & 0x03) * 255 / 3);
        ape[i].peFlags = (BYTE) 0;
    }

    if (szBitmap && (h = FindResource(NULL, szBitmap, RT_BITMAP)))
    {
        lpbi = (LPBITMAPINFOHEADER) LockResource(LoadResource(NULL, h));
        if (!lpbi)
            OutputDebugString("lock resource failed\n");
        prgb = (RGBQUAD *) ((BYTE *) lpbi + lpbi->biSize);
        if (lpbi == NULL || lpbi->biSize < sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (lpbi->biBitCount > 8)
            n = 0;
        else if (lpbi->biClrUsed == 0)
            n = 1 << lpbi->biBitCount;
        else
            n = lpbi->biClrUsed;

        for (i = 0; i < n; i++)
        {
            ape[i].peRed = prgb[i].rgbRed;
            ape[i].peGreen = prgb[i].rgbGreen;
            ape[i].peBlue = prgb[i].rgbBlue;
            ape[i].peFlags = 0;
        }
    }
    else if (szBitmap && (fh = _lopen(szBitmap, OF_READ)) != -1)
    {
        BITMAPFILEHEADER        bf;
        BITMAPINFOHEADER        bi;

        _lread(fh, &bf, sizeof(bf));
        _lread(fh, &bi, sizeof(bi));
        _lread(fh, ape, sizeof(ape));
        _lclose(fh);

        if (bi.biSize != sizeof(BITMAPINFOHEADER))
            n = 0;
        else if (bi.biBitCount > 8)
            n = 0;
        else if (bi.biClrUsed == 0)
            n = 1 << bi.biBitCount;
        else
            n = bi.biClrUsed;

        for (i = 0; i < n; i++)
        {
            BYTE        r = ape[i].peRed;

            ape[i].peRed = ape[i].peBlue;
            ape[i].peBlue = r;
        }
    }
    pdd->CreatePalette(DDPCAPS_8BIT, ape, &ddpal, NULL);
    return ddpal;
}

extern "C" DWORD DDColorMatch(IDirectDrawSurface7 *pdds, COLORREF rgb)
{
    COLORREF rgbT;
    HDC hdc;
    DWORD dw = CLR_INVALID;
    DDSURFACEDESC2 ddsd;
    HRESULT hres;

    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
		rgbT = GetPixel(hdc, 0, 0);
		SetPixel(hdc, 0, 0, rgb);
		pdds->ReleaseDC(hdc);
    }

    ddsd.dwSize = sizeof(ddsd);
    while ((hres = pdds->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING);

    if (hres == DD_OK)
    {
		dw  = *(DWORD *)ddsd.lpSurface;
		dw &= (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;
		pdds->Unlock(NULL);
    }

    if (rgb != CLR_INVALID && pdds->GetDC(&hdc) == DD_OK)
    {
		SetPixel(hdc, 0, 0, rgbT);
		pdds->ReleaseDC(hdc);
    }
    return dw;
}

extern "C" HRESULT DDSetColorKey(IDirectDrawSurface7 *pdds, COLORREF rgb)
{
    DDCOLORKEY ddck;

    ddck.dwColorSpaceLowValue  = DDColorMatch(pdds, rgb);
    ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
    return pdds->SetColorKey(DDCKEY_SRCBLT, &ddck);
}
