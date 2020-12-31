#ifndef GifAnimUI_h__
#define GifAnimUI_h__

#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	class DUILIB_API CGifAnimUI : public CLabelUI
	{
	public:
		CGifAnimUI(void);
		~CGifAnimUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		virtual void	DoEvent(TEventUI& event);
		virtual void	SetVisible(bool bVisible = true );
		virtual void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		virtual void	SetBkImage(const CDuiString& strImage);

		virtual void PaintBkImage(HDC hDC);

		virtual void	SetAutoPlay(bool bIsAuto = true );
		virtual bool	IsAutoPlay() const;
		virtual void	SetAutoSize(bool bIsAuto = true );
		virtual bool	IsAutoSize() const;
		virtual void	Play();
		virtual void	Pause();
		virtual void	Stop();

		virtual void    OnTimer( UINT_PTR idEvent );
		virtual bool	LoadFromFile(LPCTSTR pstrGifPath);
		virtual bool	LoadFromMemory( LPVOID pBuf,size_t dwSize,DWORD dwMask );
	protected:
		CDuiPtrArray	m_Frames;
		UINT			m_nFramePosition;			// ��ǰ�ŵ��ڼ�֡

		int m_nFrameWidth;
		int m_nFrameHeight;

		int m_nLoopCout;		//���ż��� 0����ѭ��//stbimg���棬����gifʱ�ƺ�û�ж�ȡ���ֵ
		int m_nLoopedNum;		

		CDuiString		m_sBkImage;
		bool			m_bIsAutoPlay;				// �Ƿ��Զ�����gif
		bool			m_bIsAutoSize;				// �Ƿ��Զ�����ͼƬ���ô�С
		bool			m_bIsPlaying;
	};
}

#endif // GifAnimUI_h__
