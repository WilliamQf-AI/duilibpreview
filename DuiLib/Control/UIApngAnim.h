#ifndef ApngAnimUI_h__
#define ApngAnimUI_h__

#ifdef SUPPORT_APNG

#pragma once

//�޸�Ϊ�� ��Ŀ����>������>����>���������� �����ã��������Խ�libpng.lib�ϲ���duilib.lib��,�ⲿ������duilib.lib����ʹ��
//#pragma comment(lib,"../apng/static/libpng.lib")

namespace DuiLib
{
#define EVENT_TIME_ID	100

	typedef struct tagApngFrame
	{
		tagApngFrame()
		{
			nFrameID=0;
			pframeBits = NULL;
			nDelayMS=0;
		};
		~tagApngFrame()
		{
			if (pframeBits)
			{
				free(pframeBits);
				pframeBits = NULL;
			}
		}
		void mallocByte(DWORD dwSize)
		{
			pframeBits = (BYTE*)malloc(dwSize);
		};
		void freeByte()
		{
			if (pframeBits)
			{
				free(pframeBits);
				pframeBits = NULL;
			}
		};

		UINT32 nFrameID;            //id
		BYTE* pframeBits;				//rgb
		UINT32 nDelayMS;            //
	}ApngFrame;

	class DUILIB_API CApngAnimUI : public CControlUI
	{
	public:
		CApngAnimUI(void);
		~CApngAnimUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoInit() override;
		bool	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void	DoEvent(TEventUI& event);
		void	SetVisible(bool bVisible = true );
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	SetBkImage(const CDuiString& strImage);
		CDuiString GetBkImage();

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		void	Play();
		void	Pause();
		void	Stop();

		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetDisabledTextColor(DWORD dwTextColor);
		DWORD GetDisabledTextColor() const;
		void SetFont(int index);
		int GetFont() const;
		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		void SetTextRenderingAlias(int nTextRenderingAlias);
		TextRenderingHint GetTextRenderingAlias();
		bool IsMultiLine();
		void SetMultiLine(bool bMultiLine = true);

	protected:
		void	InitApngImage();
		void	DeleteApng();
		void    OnTimer( UINT_PTR idEvent );
		void	DrawFrame( HDC hDC );		// ����ÿ֡
		void	LoadApngFromFile(LPCTSTR pstrApngPath);
		void	LoadApngFromMemory( LPVOID pBuf,size_t dwSize );
		void BlendOver(BYTE** rows_dst, BYTE** rows_src, UINT32 x, UINT32 y, UINT32 w, UINT32 h);
	protected:
		CDuiPtrArray	m_ApngImage;

		Bitmap* m_pgdipBmp; 
		int m_nPngWidth;
		int m_nPngHeight;
		DWORD m_nRowSize;	//һ�����ش�С,����m_nPngHeight����һ֡ͼƬ�����ش�С
		UINT m_nPlays;		//���ż���
		UINT32 m_nChannels;     //1,3,4

		UINT			m_nFramePosition;			// ��ǰ�ŵ��ڼ�֡
		CDuiString		m_sBkImage;
		bool			m_bIsAutoPlay;				// �Ƿ��Զ�����
		bool			m_bIsAutoSize;				// �Ƿ��Զ�����ͼƬ���ô�С
		bool			m_bIsPlaying;

		UINT	m_uTextStyle;
		DWORD	m_dwTextColor;
		DWORD	m_dwDisabledTextColor;
		int		m_iFont;
		RECT	m_rcTextPadding;
		TextRenderingHint		m_TextRenderingAlias;	//���ֿ���ݼ���
	};
}

#endif	//SUPPORT_APNG
#endif // ApngAnimUI_h__
