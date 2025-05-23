#include "StdAfx.h"
#include "UIScrollLabel.h"

namespace DuiLib
{
	extern Color ARGB2Color(DWORD dwColor);

	CScrollLabelUI::CScrollLabelUI(void)
	{
		m_bIsAutoPlay =	true;
		m_bIsPlaying =	false;
		m_bHorScroll = true;
		m_nTimeInterval = 40;
		m_nStepPix = 1;
		m_ptTextCurPos.x=m_ptTextCurPos.y=0;

		m_hCloneDc = NULL;
		m_hTextBitmap = NULL;
		m_nTextBmpW = 0;
		m_nTextBmpH = 0;
	}


	CScrollLabelUI::~CScrollLabelUI(void)
	{
		if (m_pManager)
			m_pManager->KillTimer( this, EVENT_TIME_ID );

		if (m_hTextBitmap)
		{
			DeleteBitmap(m_hTextBitmap);
			DeleteDC(m_hCloneDc);
			m_hTextBitmap = NULL;
			m_hCloneDc = NULL;
		}
	}

	LPCTSTR CScrollLabelUI::GetClass() const
	{
		return DUI_CTR_SCROLLLABEL;
	}

	LPVOID CScrollLabelUI::GetInterface( LPCTSTR pstrName )
	{
		if( _tcsicmp(pstrName, DUI_CTR_SCROLLLABEL) == 0 ) return static_cast<CScrollLabelUI*>(this);
		return CLabelUI::GetInterface(pstrName);
	}

	void CScrollLabelUI::DoInit()
	{
		if (m_bIsAutoPlay)
		{
			Play();
		}
	}

	void CScrollLabelUI::DoEvent( TEventUI& event )
	{
		if( event.Type == UIEVENT_TIMER && event.wParam == EVENT_TIME_ID)
		{
			Invalidate();
		}
		else
			CLabelUI::DoEvent(event);
	}

	void CScrollLabelUI::SetVisible(bool bVisible /* = true */)
	{
		CLabelUI::SetVisible(bVisible);
		if (bVisible && m_bIsAutoPlay)
			Play();
		else
			Stop();
	}

	void CScrollLabelUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if( _tcsicmp(pstrName, _T("autoplay")) == 0 ) {
			SetAutoPlay(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if( _tcsicmp(pstrName, _T("hor")) == 0 ) {
			SetHorScroll(_tcscmp(pstrValue, _T("true")) == 0);
		}
		else if (_tcsicmp(pstrName, _T("timeinterval")) == 0)
		{
			SetTimeInterval(_ttoi(pstrValue));
		}
		else if (_tcsicmp(pstrName, _T("steppix")) == 0)
		{
			SetStepPix(_ttoi(pstrValue));
		}
		else
			CLabelUI::SetAttribute(pstrName, pstrValue);
	}

	void CScrollLabelUI::SetText(const CDuiString& strText)
	{
		CLabelUI::SetText(strText);

		if (m_bHorScroll)
		{
			m_ptTextCurPos.x = m_rcItem.right-m_rcTextPadding.right;
			m_ptTextCurPos.y = m_rcItem.top+m_rcTextPadding.top;				
		}
		else
		{			
			m_ptTextCurPos.x = m_rcItem.left+m_rcTextPadding.left;			
			m_ptTextCurPos.y = m_rcItem.bottom-m_rcTextPadding.bottom;				
		}
		if (m_hTextBitmap)
		{
			DeleteBitmap(m_hTextBitmap);
			m_hTextBitmap = NULL;
		}
		if (GetText().IsEmpty()==false && m_bIsPlaying==false)
		{
			Play();
		}
	}
	void CScrollLabelUI::SetTextColor(DWORD dwTextColor)
	{
		CLabelUI::SetTextColor(dwTextColor);
		if (m_hTextBitmap)
		{
			DeleteBitmap(m_hTextBitmap);
			m_hTextBitmap = NULL;
		}
	}
	void CScrollLabelUI::SetTextPadding(RECT rc)
	{
		CLabelUI::SetTextPadding(rc);

		if (m_bHorScroll)
		{
			m_ptTextCurPos.x = m_rcItem.right-m_rcTextPadding.right;
			m_ptTextCurPos.y = m_rcItem.top+m_rcTextPadding.top;				
		}
		else
		{			
			m_ptTextCurPos.x = m_rcItem.left+m_rcTextPadding.left;			
			m_ptTextCurPos.y = m_rcItem.bottom-m_rcTextPadding.bottom;				
		}
		
		if (m_hTextBitmap)
		{
			DeleteBitmap(m_hTextBitmap);
			m_hTextBitmap = NULL;
		}
	}

	void CScrollLabelUI::CreateTextBitmap(HDC hDC)
	{
		if (m_hCloneDc==NULL)
			m_hCloneDc = ::CreateCompatibleDC(hDC);
		
		if (m_hTextBitmap)
		{
			DeleteBitmap(m_hTextBitmap);
			m_hTextBitmap = NULL;
		}

		RECT rcText = {m_rcItem.left+m_rcTextPadding.left,m_rcItem.top+m_rcTextPadding.top,m_rcItem.right-m_rcTextPadding.right,m_rcItem.bottom-m_rcTextPadding.bottom};
		RECT rcShow = {0};		
		if (IsHorScroll())
		{
			SIZE sizeText = CRenderEngine::EstimateTextSize(m_pManager->GetPaintDC(),m_pManager,GetText().GetData(),GetFont(),GetTextStyle(),m_bShowHtml,GetEnabledEffect(),0,rcText.bottom-rcText.top);
			rcShow.right = sizeText.cx;
			rcShow.bottom = sizeText.cy;
		}
		else
		{
			SIZE sizeText = CRenderEngine::EstimateTextSize(m_pManager->GetPaintDC(),m_pManager,GetText().GetData(),GetFont(),GetTextStyle(),m_bShowHtml,GetEnabledEffect(),rcText.right-rcText.left);
			rcShow.right = sizeText.cx;
			rcShow.bottom = sizeText.cy;
		}
		m_nTextBmpW = rcShow.right;
		m_nTextBmpH = rcShow.bottom;

		BITMAPINFO bmi;
		::ZeroMemory(&bmi, sizeof(BITMAPINFO));
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = rcShow.right;
		bmi.bmiHeader.biHeight = 0-rcShow.bottom;
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32;
		bmi.bmiHeader.biCompression = BI_RGB;
		bmi.bmiHeader.biSizeImage = (rcShow.right*32+31)/32*4*rcShow.bottom;
		COLORREF* pDest = NULL;
		m_hTextBitmap = ::CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pDest, NULL, 0);
		if( !m_hTextBitmap )
			return ;

		HBITMAP hOld = SelectBitmap(m_hCloneDc,m_hTextBitmap);
		
		{
			if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
			if( m_dwDisabledTextColor == 0 ) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

			RECT rc = rcShow;
			if(!GetEnabledEffect())
			{
				if( m_sText.IsEmpty() ) return;
				int nLinks = 0;
				if( IsEnabled() ) {
					if( m_bShowHtml )
						CRenderEngine::DrawHtmlText(m_hCloneDc, m_pManager, rc, m_sText.GetData(), m_dwTextColor, \
						NULL, NULL, nLinks, m_iFont, m_uTextStyle);
					else
						CRenderEngine::DrawText(m_hCloneDc, m_pManager, rc, m_sText.GetData(), m_dwTextColor, \
						m_iFont, m_uTextStyle);
				}
				else {
					if( m_bShowHtml )
						CRenderEngine::DrawHtmlText(m_hCloneDc, m_pManager, rc, m_sText.GetData(), m_dwDisabledTextColor, \
						NULL, NULL, nLinks, m_iFont, m_uTextStyle);
					else
						CRenderEngine::DrawText(m_hCloneDc, m_pManager, rc, m_sText.GetData(), m_dwDisabledTextColor, \
						m_iFont, m_uTextStyle);
				}

				for( LONG y = 0; y < rc.bottom; ++y ) 
				{
					for( LONG x = rc.left; x < rc.right; ++x ) 
					{
						COLORREF* pOffscreenBits = pDest + y * (rc.right-rc.left) + x;						
						if (*pOffscreenBits!=0)
						{
							//A=(BYTE)((*pOffscreenBits) >> 24);
							//if (A==0)
							//	A=255;
							BYTE A=255;
							BYTE R = (BYTE)((*pOffscreenBits) >> 16);
							BYTE G = (BYTE)((*pOffscreenBits) >> 8);
							BYTE B = (BYTE)(*pOffscreenBits);
							*pOffscreenBits = RGB(B, G, R) + ((DWORD)A << 24);
						}
					}
				}
			}
			else
			{
				Font	nFont(m_hCloneDc,m_pManager->GetFont(GetFont()));
				Graphics nGraphics(m_hCloneDc);
				nGraphics.SetTextRenderingHint(GetTextRenderingAlias());

				StringFormat format;
				StringAlignment sa = StringAlignment::StringAlignmentNear;
				if ((m_uTextStyle & DT_VCENTER) != 0) sa = StringAlignment::StringAlignmentCenter;
				else if( (m_uTextStyle & DT_BOTTOM) != 0) sa = StringAlignment::StringAlignmentFar;
				format.SetLineAlignment((StringAlignment)sa);
				sa = StringAlignment::StringAlignmentNear;
				if ((m_uTextStyle & DT_CENTER) != 0) sa = StringAlignment::StringAlignmentCenter;
				else if( (m_uTextStyle & DT_RIGHT) != 0) sa = StringAlignment::StringAlignmentFar;
				format.SetAlignment((StringAlignment)sa);
				if ((m_uTextStyle & DT_SINGLELINE) != 0) format.SetFormatFlags(StringFormatFlagsNoWrap);

				RectF nRc((float)rc.left,(float)rc.top,(float)rc.right-rc.left,(float)rc.bottom-rc.top);
				RectF nShadowRc = nRc;
				nShadowRc.X += m_ShadowOffset.X;
				nShadowRc.Y += m_ShadowOffset.Y;

				int nGradientLength	= GetGradientLength();

				if(nGradientLength == 0)
					nGradientLength = (rc.bottom-rc.top);

				LinearGradientBrush nLineGrBrushA(Point(GetGradientAngle(), 0),Point(0,nGradientLength),ARGB2Color(GetTextShadowColorA()),ARGB2Color(GetTextShadowColorB() == -1?GetTextShadowColorA():GetTextShadowColorB()));
				LinearGradientBrush nLineGrBrushB(Point(GetGradientAngle(), 0),Point(0,nGradientLength),ARGB2Color(GetTextColor()),ARGB2Color(GetTextColor1() == -1?GetTextColor():GetTextColor1()));

				if (GetEnabledLuminous())
				{
					// from http://bbs.csdn.net/topics/390346428
					int iFuzzyWidth = (int)(nRc.Width/GetLuminousFuzzy());
					if (iFuzzyWidth < 1) iFuzzyWidth = 1;
					int iFuzzyHeight = (int)(nRc.Height/GetLuminousFuzzy());
					if (iFuzzyHeight < 1) iFuzzyHeight = 1;
					RectF nTextRc(0.0f, 0.0f, nRc.Width, nRc.Height);

					Bitmap Bit1((INT)nRc.Width, (INT)nRc.Height);
					Graphics g1(&Bit1);
					g1.SetSmoothingMode(SmoothingModeAntiAlias);
					g1.SetTextRenderingHint(GetTextRenderingAlias());
					g1.SetCompositingQuality(CompositingQualityAssumeLinear);
					Bitmap Bit2(iFuzzyWidth, iFuzzyHeight);
					Graphics g2(&Bit2);
					g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
					g2.SetPixelOffsetMode(PixelOffsetModeNone);

					FontFamily ftFamily;
					nFont.GetFamily(&ftFamily);

					CDuiString sText1 = m_sText;
					CPaintManagerUI::ProcessMultiLanguageTokens(sText1);
#ifdef _UNICODE				
					LPCWSTR pstrText = sText1.GetData();
#else
					int iLen = _tcslen(sText1.GetData());
					LPWSTR pWideText = new WCHAR[iLen + 1];
					::ZeroMemory(pWideText, (iLen + 1) * sizeof(WCHAR));
					::MultiByteToWideChar(CP_ACP, 0, sText1.GetData(), -1, pWideText, iLen);
					LPCWSTR pstrText = pWideText;
#endif
					g1.DrawString(pstrText,wcslen(pstrText),&nFont,nRc,&format,&nLineGrBrushB);
#ifndef _UNICODE
					delete[] pWideText;
#endif	
					g2.DrawImage(&Bit1, 0, 0, (int)iFuzzyWidth, (int)iFuzzyHeight);
					g1.Clear(Color(0));
					g1.DrawImage(&Bit2, (int)m_ShadowOffset.X, (int)m_ShadowOffset.Y, (int)nRc.Width, (int)nRc.Height);

					nGraphics.DrawImage(&Bit1, nRc.X, nRc.Y);
				}

				if(GetEnabledStroke() && GetStrokeColor() > 0)
				{
					LinearGradientBrush nLineGrBrushStroke(Point(GetGradientAngle(),0),Point(0,rc.bottom-rc.top+2),ARGB2Color(GetStrokeColor()),ARGB2Color(GetStrokeColor()));

					CDuiString sText1 = m_sText;
					CPaintManagerUI::ProcessMultiLanguageTokens(sText1);
#ifdef _UNICODE			
					LPCWSTR pstrText = sText1.GetData();
#else
					int iLen = _tcslen(sText1.GetData());
					LPWSTR pWideText = new WCHAR[iLen + 1];
					::ZeroMemory(pWideText, (iLen + 1) * sizeof(WCHAR));
					::MultiByteToWideChar(CP_ACP, 0, sText1.GetData(), -1, pWideText, iLen);
					LPCWSTR pstrText = pWideText;			
#endif
					int nLen = wcslen(pstrText);
					nRc.Offset(-1,0);
					nGraphics.DrawString(pstrText,nLen,&nFont,nRc,&format,&nLineGrBrushStroke);
					nRc.Offset(2,0);
					nGraphics.DrawString(pstrText,nLen,&nFont,nRc,&format,&nLineGrBrushStroke);
					nRc.Offset(-1,-1);
					nGraphics.DrawString(pstrText,nLen,&nFont,nRc,&format,&nLineGrBrushStroke);
					nRc.Offset(0,2);
					nGraphics.DrawString(pstrText,nLen,&nFont,nRc,&format,&nLineGrBrushStroke);
					nRc.Offset(0,-1);
#ifndef _UNICODE
					delete[] pWideText;
#endif	
				}

				CDuiString sText1 = m_sText;
				CPaintManagerUI::ProcessMultiLanguageTokens(sText1);
#ifdef _UNICODE			
				LPCWSTR pstrText = sText1.GetData();
#else
				int iLen = _tcslen(sText1.GetData());
				LPWSTR pWideText = new WCHAR[iLen + 1];
				::ZeroMemory(pWideText, (iLen + 1) * sizeof(WCHAR));
				::MultiByteToWideChar(CP_ACP, 0, sText1.GetData(), -1, pWideText, iLen);
				LPCWSTR pstrText = pWideText;			
#endif
				int nLen = wcslen(pstrText);
				if(GetEnabledShadow() && (GetTextShadowColorA() > 0 || GetTextShadowColorB() > 0))
					nGraphics.DrawString(pstrText,nLen,&nFont,nShadowRc,&format,&nLineGrBrushA);

				nGraphics.DrawString(pstrText,nLen,&nFont,nRc,&format,&nLineGrBrushB);
#ifndef _UNICODE
				delete[] pWideText;
#endif
			}
		}//m_hTextBitmap绘制完成

#if 0
		Gdiplus::Bitmap srcBmp(m_hTextBitmap,NULL);
		//srcBmp.FromHBITMAP(m_hTextBitmap);
		UINT  num = 0 ,size = 0;
		// size of the image encoder array in bytes
		ImageCodecInfo* pImageCodecInfo = NULL;
		if(GetImageEncodersSize(&num, &size)!= Ok || size == 0 )
			return ;  // Failure
		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if(pImageCodecInfo == NULL)
			return ;  // Failure
		if(GetImageEncoders(num, size, pImageCodecInfo)!= Ok )
			return ;

		for(UINT j = 0; j < num; ++j)
		{
			if( wcsicmp(pImageCodecInfo[j].MimeType, L"image/png") == 0 ) //保存为png格式
			{
				srcBmp.Save(L"d://dui.png",&(pImageCodecInfo[j].Clsid));
				break;
			}
		}
#endif
	
	}

	void CScrollLabelUI::SetHorScroll(bool bIsHor)
	{
		if (m_bHorScroll != bIsHor)
		{
			m_bHorScroll = bIsHor;
			if (m_bHorScroll)
			{
				m_ptTextCurPos.x = m_rcItem.right-m_rcTextPadding.right;
				m_ptTextCurPos.y = m_rcItem.top+m_rcTextPadding.top;				
			}
			else
			{			
				m_ptTextCurPos.x = m_rcItem.left+m_rcTextPadding.left;			
				m_ptTextCurPos.y = m_rcItem.bottom-m_rcTextPadding.bottom;				
			}

			if (m_hTextBitmap)
			{
				DeleteBitmap(m_hTextBitmap);
				m_hTextBitmap = NULL;
			}
		}
	}

	void CScrollLabelUI::SetTimeInterval(const int& nValue)
	{
		if (nValue<1 || m_nTimeInterval==nValue)
			return;

		m_nTimeInterval = nValue;
		if (m_bIsPlaying && m_pManager)
		{
			m_pManager->KillTimer(this,EVENT_TIME_ID);
			m_pManager->SetTimer(this,EVENT_TIME_ID,m_nTimeInterval);
		}
	}
	int CScrollLabelUI::GetTimeInterval()
	{
		return m_nTimeInterval;
	}
	void CScrollLabelUI::SetStepPix(const int& nValue)
	{
		m_nStepPix = nValue;
	}
	int CScrollLabelUI::GetStepPix()
	{
		return m_nStepPix;
	}
	bool CScrollLabelUI::IsHorScroll() const
	{
		return m_bHorScroll;
	}

	void CScrollLabelUI::SetAutoPlay(bool bIsAuto)
	{
		if (m_bIsAutoPlay != bIsAuto)
		{
			m_bIsAutoPlay = bIsAuto;
			if (m_bIsAutoPlay)
				Play();
			else
				Stop();
		}
	}

	bool CScrollLabelUI::IsAutoPlay() const
	{
		return m_bIsAutoPlay;
	}

	void CScrollLabelUI::Play()
	{
		if (m_bIsPlaying || GetText().IsEmpty())
			return;

		if ( m_nTimeInterval > 0 )
			m_pManager->SetTimer( this, EVENT_TIME_ID, m_nTimeInterval );

		m_bIsPlaying = true;
		Invalidate();
	}

	void CScrollLabelUI::Pause()
	{
		if (!m_bIsPlaying)
			return;

		m_pManager->KillTimer(this, EVENT_TIME_ID);
		this->Invalidate();
		m_bIsPlaying = false;
	}

	void CScrollLabelUI::Stop()
	{
		if (!m_bIsPlaying)
			return;

		m_pManager->KillTimer(this, EVENT_TIME_ID);
		m_ptTextCurPos.x = m_rcItem.left+m_rcTextPadding.left;
		m_ptTextCurPos.y = m_rcItem.top+m_rcTextPadding.top;
		Invalidate();
		m_bIsPlaying = false;
	}

	void CScrollLabelUI::PaintText(HDC hDC)
	{
		//绘制当前帧
		if (GetText().IsEmpty() || IsAutoPlay()==false)
		{
			CLabelUI::PaintText(hDC);
			return;
		}
		else
		{
			if (m_hTextBitmap==NULL)
				CreateTextBitmap(hDC);

#if 0

			Gdiplus::Bitmap srcBmp(m_hTextBitmap,NULL);
			//srcBmp.FromHBITMAP(m_hTextBitmap);
			UINT  num = 0 ,size = 0;
			// size of the image encoder array in bytes
			ImageCodecInfo* pImageCodecInfo = NULL;
			if(GetImageEncodersSize(&num, &size)!= Ok || size == 0 )
				return ;  // Failure
			pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
			if(pImageCodecInfo == NULL)
				return ;  // Failure
			if(GetImageEncoders(num, size, pImageCodecInfo)!= Ok )
				return ;

			for(UINT j = 0; j < num; ++j)
			{
				if( wcsicmp(pImageCodecInfo[j].MimeType, L"image/png") == 0 ) //保存为png格式
				{
					srcBmp.Save(L"d://dui3.png",&(pImageCodecInfo[j].Clsid));
					break;
				}
			}

			RECT rcText = {m_rcItem.left+m_rcTextPadding.left,m_rcItem.top+m_rcTextPadding.top,m_rcItem.right-m_rcTextPadding.right,m_rcItem.bottom-m_rcTextPadding.bottom};
			typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);
			static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");
			BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
			lpAlphaBlend(hDC,rcText.left,rcText.top,m_nTextBmpW,m_nTextBmpH,m_hCloneDc,0,0,m_nTextBmpW,m_nTextBmpH,bf);
			return;

#else


			RECT rcText = {m_rcItem.left+m_rcTextPadding.left,m_rcItem.top+m_rcTextPadding.top,m_rcItem.right-m_rcTextPadding.right,m_rcItem.bottom-m_rcTextPadding.bottom};
			RECT rcShow = {0};
			if (IsHorScroll())
			{
				if (m_ptTextCurPos.x==0&&m_ptTextCurPos.y==0)
					rcShow.left = rcText.right-GetStepPix();
				else
					rcShow.left = m_ptTextCurPos.x-GetStepPix();
				rcShow.right = rcShow.left + m_nTextBmpW;
				rcShow.top = rcText.top;
				rcShow.bottom = rcText.bottom;
				if (GetStepPix() > 0)
				{//向左步进
					if (rcShow.right < rcText.left)
					{
						rcShow.left = rcText.right;
						rcShow.right = rcShow.left + m_nTextBmpW;
					}
				}	
				else
				{
					if (rcShow.left>rcText.right)
					{//向右步进
						rcShow.right = rcText.left;
						rcShow.left = rcShow.right-m_nTextBmpW;						
					}
				}
				
				
				m_ptTextCurPos.x = rcShow.left;
				m_ptTextCurPos.y = rcShow.top;
			}
			else
			{
				rcShow.left = rcText.left;				
				rcShow.right = rcText.right;
				rcShow.top = m_ptTextCurPos.y-GetStepPix();
				rcShow.bottom = rcShow.top + m_nTextBmpH;
				if (GetStepPix() > 0)
				{//向上步进
					if (rcShow.bottom < rcText.top)
					{
						rcShow.top = rcText.bottom;
						rcShow.bottom = rcText.bottom+m_nTextBmpH;
					}
				}
				else
				{
					if (rcShow.top>rcText.bottom)
					{//向下步进
						rcShow.bottom = rcText.top;
						rcShow.top = rcShow.bottom-m_nTextBmpH;						
					}
				}
				
				m_ptTextCurPos.x = rcShow.left;
				m_ptTextCurPos.y = rcShow.top;
			}

			RECT rcIntersect = {0};
			if (IntersectRect(&rcIntersect,&rcShow,&rcText) == FALSE)
				return;

			typedef BOOL (WINAPI *LPALPHABLEND)(HDC, int, int, int, int,HDC, int, int, int, int, BLENDFUNCTION);
			static LPALPHABLEND lpAlphaBlend = (LPALPHABLEND) ::GetProcAddress(::GetModuleHandle(_T("msimg32.dll")), "AlphaBlend");
			BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
			lpAlphaBlend(hDC,rcIntersect.left,rcIntersect.top,rcIntersect.right-rcIntersect.left,rcIntersect.bottom-rcIntersect.top,m_hCloneDc,rcIntersect.left-rcShow.left,rcIntersect.top-rcShow.top,rcIntersect.right-rcIntersect.left,rcIntersect.bottom-rcIntersect.top,bf);
#endif
		}
	}
}