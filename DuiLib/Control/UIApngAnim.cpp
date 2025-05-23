#include "StdAfx.h"

#include "UIApngAnim.h"

namespace DuiLib
{
	CApngAnimUI::CApngAnimUI(void)
	{
	}


	CApngAnimUI::~CApngAnimUI(void)
	{
		for (int i=0;i<m_Frames.GetSize();i++)
		{
			ApngFrame* pFrame = (ApngFrame*)m_Frames[i];
			delete pFrame;
		}
		m_Frames.Empty();

		if (m_pManager)
			m_pManager->KillTimer( this, EVENT_TIME_ID );
	}

	LPCTSTR CApngAnimUI::GetClass() const
	{
		return DUI_CTR_APNGANIM;
	}

	LPVOID CApngAnimUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcsicmp(pstrName, DUI_CTR_APNGANIM) == 0 ) return static_cast<CApngAnimUI*>(this);
		return CGifAnimUI::GetInterface(pstrName);
	}

	void CApngAnimUI::SetBkImage(const CDuiString& strImage)
	{
		if( m_sBkImage == strImage) return;
		m_sBkImage = strImage;

		Stop();
		//清理
		if ( m_Frames.IsEmpty())
		{
			if( m_diBk.pImageInfo != NULL ) 
				m_diBk.Clear();
		}
		else
		{
			for (int i=0;i<m_Frames.GetSize();i++)
			{
				ApngFrame* pFrame = (ApngFrame*)m_Frames[i];
				delete pFrame;
			}
			m_Frames.Empty();
			m_nFramePosition	=	0;	
		}
		//重新载入
		if (LoadFromFile(strImage.GetData()))
		{
			if (m_bIsAutoSize)
			{
				SetFixedWidth(m_nFrameWidth);
				SetFixedHeight(m_nFrameHeight);
			}

			if (m_bIsAutoPlay && m_Frames.GetSize() > 0)
			{
				Play();
			}
		}
		else
		{
			CGifAnimUI::SetBkImage(strImage);
		}
	}

	void CApngAnimUI::Play()
	{
		if (m_bIsPlaying || m_Frames.IsEmpty())
			return;

		m_nLoopedNum = 0;

		long lPause = ((ApngFrame*)m_Frames[m_nFramePosition])->fram.nDelayMs;
		if ( lPause == 0 ) lPause = 100;
		if (m_pManager)
		{
			m_pManager->SetTimer(this, EVENT_TIME_ID, lPause);
			m_bIsPlaying = true;
		}

		Invalidate();
	}

	void CApngAnimUI::OnTimer( UINT_PTR idEvent )
	{
		m_pManager->KillTimer( this, EVENT_TIME_ID );
		this->Invalidate();

		if (m_nLoopCout>0)
		{
			if (m_nFramePosition==0)
				m_nLoopedNum++;

			if (m_nLoopedNum <= m_nLoopCout)
			{
				if (m_Frames.GetSize() > 0)
					m_nFramePosition = (++m_nFramePosition) % m_Frames.GetSize();

				long lPause = ((ApngFrame*)m_Frames[m_nFramePosition])->fram.nDelayMs;
				if ( lPause == 0 ) lPause = 100;
				m_pManager->SetTimer( this, EVENT_TIME_ID, lPause );
			}
		}
		else
		{
			if (m_Frames.GetSize() > 0)
				m_nFramePosition = (++m_nFramePosition) % m_Frames.GetSize();

			long lPause = ((ApngFrame*)m_Frames[m_nFramePosition])->fram.nDelayMs;
			if ( lPause == 0 ) lPause = 100;
			m_pManager->SetTimer( this, EVENT_TIME_ID, lPause );
		}
	}

	void CApngAnimUI::PaintBkImage(HDC hDC)
	{
		//绘制当前帧
		if (m_Frames.IsEmpty())
		{
			CGifAnimUI::PaintBkImage(hDC);
			return;
		}
		else
		{
			ApngFrame* pAFram = (ApngFrame*)m_Frames[m_nFramePosition];

			RECT rcDest = m_rcItem;
			if( m_diBk.rcDestOffset.left != 0 || m_diBk.rcDestOffset.top != 0 ||
				m_diBk.rcDestOffset.right != 0 || m_diBk.rcDestOffset.bottom != 0 ) {
					rcDest.left = m_rcItem.left + m_diBk.rcDestOffset.left;
					rcDest.top = m_rcItem.top + m_diBk.rcDestOffset.top;
					rcDest.right = m_rcItem.left + m_diBk.rcDestOffset.right;
					if( rcDest.right > m_rcItem.right ) rcDest.right = m_rcItem.right;
					rcDest.bottom = m_rcItem.top + m_diBk.rcDestOffset.bottom;
					if( rcDest.bottom > m_rcItem.bottom ) rcDest.bottom = m_rcItem.bottom;
			}

			CRenderEngine::DrawImage(hDC,pAFram->fram.hBmpFrame,rcDest,m_rcPaint,m_diBk.rcBmpPart,m_diBk.rcScale9,true, m_diBk.uFade, m_diBk.bHole, m_diBk.bTiledX, m_diBk.bTiledY);
		}

		if (m_bIsAutoPlay && m_bIsPlaying==false)
		{
			if (m_pManager)
			{
				long lPause = ((ApngFrame*)m_Frames[m_nFramePosition])->fram.nDelayMs;
				if (lPause == 0) lPause = 100;
				m_pManager->SetTimer(this, EVENT_TIME_ID, lPause);
				m_bIsPlaying = true;
			}
		}
	}

	void CApngAnimUI::BlendOver(COLORREF* dst, COLORREF* src,UINT32 dstWidth, UINT32 dstHeight,UINT32 xoffset, UINT32 yoffset, UINT32 width, UINT32 height)
	{
		COLORREF* pDstBits = NULL;
		COLORREF* pSrcBits = NULL;
		BYTE aDst = 0,aSrc = 0;
		BYTE rDst = 0,rSrc = 0;
		BYTE gDst = 0,gSrc = 0;
		BYTE bDst = 0,bSrc = 0;
		RECT rcSrc = {xoffset,yoffset,xoffset+width,yoffset+height};
		for (UINT32 y=0;y<dstHeight;y++)
		{
			for (UINT32 x=0;x<dstWidth;x++)
			{
				pDstBits = dst + y * dstWidth + x;
				POINT pt = {x,y};
				if (PtInRect(&rcSrc,pt))
				{
					pSrcBits = src + (y-yoffset) * width + (x-xoffset);

					aSrc = (BYTE)((*pSrcBits) >> 24);
					if (aSrc > 0)
					{
						if (aSrc==255)
						{
							*pDstBits = *pSrcBits;
						}
						else
						{
							aDst = (BYTE)((*pDstBits) >> 24);
							if (aDst==0)
							{
								*pDstBits = *pSrcBits;
							}
							else
							{
								int u = aSrc * 255;
								int v = (255 - aSrc) * aDst;
								int al = u + v;
								bDst = (BYTE)(*pDstBits);			bSrc = (BYTE)(*pSrcBits);
								gDst = (BYTE)((*pDstBits) >> 8);	gSrc = (BYTE)((*pSrcBits) >> 8);
								rDst = (BYTE)((*pDstBits) >> 16);	rSrc = (BYTE)((*pSrcBits) >> 16);
								bDst = (bSrc * u + bDst * v) / al;
								gDst = (gSrc * u + gDst * v) / al;
								rDst = (rSrc * u + rDst * v) / al;
								aDst = al / 255;

								*pDstBits = RGB(rDst, gDst, bDst) + ((DWORD)aDst << 24);
							}
						}
					}
				}//PtInRect						
			}//x
		}//y
	}
	void CApngAnimUI::BlendSource(COLORREF* dst, COLORREF* src,UINT32 dstWidth, UINT32 dstHeight,UINT32 xoffset, UINT32 yoffset, UINT32 width, UINT32 height)
	{
		COLORREF* pDstBits = NULL;
		COLORREF* pSrcBits = NULL;
		RECT rcSrc = {xoffset,yoffset,xoffset+width,yoffset+height};
		for (UINT32 y=0;y<dstHeight;y++)
		{
			for (UINT32 x=0;x<dstWidth;x++)
			{
				pDstBits = dst + y * dstWidth + x;
				POINT pt = {x,y};
				if (PtInRect(&rcSrc,pt))
				{
					pSrcBits = src + (y-yoffset) * width + (x-xoffset);
					*pDstBits = *pSrcBits;
				}								
			}
		}
	}

	HBITMAP CApngAnimUI::CreateFramBmp(LPBYTE pImage,UINT32 dstWidth, UINT32 dstHeight,DWORD dwMask)
	{
		BITMAPINFO bmi;
		::ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = dstWidth;
		bmi.bmiHeader.biHeight = 0-dstHeight;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = (dstWidth*32+31)/32*4*dstHeight;
		LPBYTE pDest = NULL;
		HBITMAP hBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
		if( !hBitmap )
			return NULL;			

		for( int i = 0; i < dstWidth * dstHeight; i++ ) 
		{
			pDest[i*4 + 3] = pImage[i*4 + 3];		
			if( pDest[i*4 + 3] < 255 )
			{
				pDest[i*4] = (BYTE)(DWORD(pImage[i*4 + 2])*pImage[i*4 + 3]/255);
				pDest[i*4 + 1] = (BYTE)(DWORD(pImage[i*4 + 1])*pImage[i*4 + 3]/255);
				pDest[i*4 + 2] = (BYTE)(DWORD(pImage[i*4])*pImage[i*4 + 3]/255); 
			}
			else
			{
				pDest[i*4] = pImage[i*4 + 2];
				pDest[i*4 + 1] = pImage[i*4 + 1];
				pDest[i*4 + 2] = pImage[i*4]; 
			}

			if( *(DWORD*)(&pDest[i*4]) == dwMask ) {
				pDest[i*4] = (BYTE)0;
				pDest[i*4 + 1] = (BYTE)0;
				pDest[i*4 + 2] = (BYTE)0; 
				pDest[i*4 + 3] = (BYTE)0;					
			}
		}
		return hBitmap;
	}
	void CApngAnimUI::BlendZero(COLORREF* dst, UINT32 dstWidth, UINT32 dstHeight,UINT32 xoffset, UINT32 yoffset, UINT32 width, UINT32 height)
	{
		COLORREF* pDstBits = NULL;
		RECT rcSrc = {xoffset,yoffset,xoffset+width,yoffset+height};
		for (UINT32 y=0;y<dstHeight;y++)
		{
			for (UINT32 x=0;x<dstWidth;x++)
			{
				pDstBits = dst + y * dstWidth + x;
				POINT pt = {x,y};
				if (PtInRect(&rcSrc,pt))
				{			
					*pDstBits = 0;
				}								
			}
		}
	}

	bool CApngAnimUI::LoadFromMemory( LPVOID pBuf,size_t dwSize,DWORD dwMask )
	{
		stbi_apng_frame* pstbfram = NULL;
		int nFramsNum = 0;
		size_t frame_offset = 0;
		bool bFirstHidden = false;
		int first = bFirstHidden ? 1 : 0;
		unsigned char* pData = CRenderEngine::ParseApngFromMemory((const unsigned char *)pBuf,dwSize,(void**)&pstbfram,&m_nFrameWidth,&m_nFrameHeight,&nFramsNum,&m_nLoopCout,&bFirstHidden);
		if (pData)
		{
			UINT32 uImgSize = m_nFrameWidth*m_nFrameHeight*4;
			LPBYTE pView = (LPBYTE)malloc(sizeof(BYTE)*uImgSize);
			memset(pView,0,uImgSize);
			LPBYTE pLast = (LPBYTE)malloc(sizeof(BYTE)*uImgSize);
			memset(pLast,0,uImgSize);

			for (int iFram=0;iFram<nFramsNum;iFram++)
			{
				ApngFrame *pAFram = new ApngFrame;
				pAFram->stbFram = pstbfram[iFram];
				pAFram->fram.nFrameId = iFram;
				if (pAFram->stbFram.delay_den==0)
					pAFram->fram.nDelayMs = (pAFram->stbFram.delay_num / 100.0f) * 1000.0f;
				else
					pAFram->fram.nDelayMs = ((float) pAFram->stbFram.delay_num / pAFram->stbFram.delay_den) * 1000.0f;

				LPBYTE pFramBit = pData + frame_offset;
				frame_offset += pAFram->stbFram.width * pAFram->stbFram.height * 4;

				//预绘制
				if (iFram == first)
				{
					pAFram->stbFram.blend_op = 0;//STBI_APNG_blend_op_source
					if (pAFram->stbFram.dispose_op==2)//STBI_APNG_dispose_op_previous
						pAFram->stbFram.dispose_op=1;//STBI_APNG_dispose_op_background
				}

				if (pAFram->stbFram.dispose_op == 2)//STBI_APNG_dispose_op_previous
					memcpy(pLast,pView,uImgSize);

				if (pAFram->stbFram.blend_op == 1)//STBI_APNG_blend_op_over
				{
					BlendOver((COLORREF*)pView,(COLORREF*)pFramBit,m_nFrameWidth,m_nFrameHeight,pAFram->stbFram.x_offset,pAFram->stbFram.y_offset,pAFram->stbFram.width,pAFram->stbFram.height);
				}
				else
				{
					BlendSource((COLORREF*)pView,(COLORREF*)pFramBit,m_nFrameWidth,m_nFrameHeight,pAFram->stbFram.x_offset,pAFram->stbFram.y_offset,pAFram->stbFram.width,pAFram->stbFram.height);
				}
				pAFram->fram.hBmpFrame = CreateFramBmp(pView,m_nFrameWidth,m_nFrameHeight,dwMask);
				if (pAFram->fram.hBmpFrame == NULL)
				{
					CRenderEngine::FreeApngFromMemory(pData);
					for (int i=0;i<m_Frames.GetSize();i++)
					{
						ApngFrame* pFrame = (ApngFrame*)m_Frames[i];
						delete pFrame;
					}
					m_Frames.Empty();
					free(pView);
					free(pLast);
					return false;
				}
				m_Frames.Add(pAFram);

				if (pAFram->stbFram.dispose_op == 2)//STBI_APNG_dispose_op_previous
				{
					memcpy(pView,pLast,uImgSize);
				}
				else if (pAFram->stbFram.dispose_op == 1)//STBI_APNG_dispose_op_background
				{
					BlendZero((COLORREF*)pView,m_nFrameWidth,m_nFrameHeight,pAFram->stbFram.x_offset,pAFram->stbFram.y_offset,pAFram->stbFram.width,pAFram->stbFram.height);
				}
			}
			free(pView);
			free(pLast);

			CRenderEngine::FreeApngFromMemory(pData);
			return true;
		}		

		return false;
	}
}
