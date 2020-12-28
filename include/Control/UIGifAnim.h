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
		void	DoInit() override;
		void	DoEvent(TEventUI& event);
		void	SetVisible(bool bVisible = true );
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	SetBkImage(const CDuiString& strImage);

		virtual void PaintBkImage(HDC hDC);

		void	SetAutoPlay(bool bIsAuto = true );
		bool	IsAutoPlay() const;
		void	SetAutoSize(bool bIsAuto = true );
		bool	IsAutoSize() const;
		void	Play();
		void	Pause();
		void	Stop();

	protected:
		void    OnTimer( UINT_PTR idEvent );
		bool	LoadFromFile(LPCTSTR pstrGifPath);
		bool	LoadFromMemory( LPVOID pBuf,size_t dwSize,DWORD dwMask );
	protected:
		CDuiPtrArray	m_Frames;
		UINT			m_nFramePosition;			// ��ǰ�ŵ��ڼ�֡

		int m_nFrameWidth;
		int m_nFrameHeight;

		int m_nLoopCout;		//���ż��� 0����ѭ��
		int m_nLoopedNum;		

		CDuiString		m_sBkImage;
		bool			m_bIsAutoPlay;				// �Ƿ��Զ�����gif
		bool			m_bIsAutoSize;				// �Ƿ��Զ�����ͼƬ���ô�С
		bool			m_bIsPlaying;
	};
}

#endif // GifAnimUI_h__
