
#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	class CRotateImageUI : public CControlUI
	{
	public:
		CRotateImageUI();
		~CRotateImageUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void	DoInit();
		void	DoEvent(TEventUI& event);		
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	SetVisible(bool bVisible = true );

		void SetPos(RECT rc, bool bNeedInvalidate = true);

		void	SetTimeInterval(const int& nValue);
		int		GetTimeInterval();
		void	SetEachTimeAngle(const int& nValue);
		int		GetEachTimeAngle();
		void	SetFixAngle(const int& nValue);
		int		GetFixAngle();
		void	SetBkImage(const CDuiString& strImage);
		void	SetStretch(const bool& bStretch);
		bool	GetStretch();
		void	SetAutoStart(const bool& bAutoStart);
		bool	GetAutoStart();
		void	Stop();
		void	Start();
		void	Pause();

		void	PaintBkImage(HDC hDC);	

	protected:
		Gdiplus::Image*	LoadImg(LPCTSTR pstrImgPath);
		void	InitInfo();

	protected:
		int m_nTimeInterval;			//ʱ����(���ٺ�����תһ��) ��λms
		int m_nAngle;					//ÿ����ת�Ƕ�
		bool m_bStretch;				//�Ƿ��Զ�����ͼƬ
		bool m_bIsAutoStart;			//�Ƿ��Զ���ʼ��ת
		int m_nFixAngle;				//�̶���ת�Ƕ�		

		int m_nCurAngle;
		Gdiplus::Image*	m_pBkimage;
		IStream* m_pStream;
		CDuiString m_sBkImage;
		bool m_bIsPlaying;				//�Ƿ�������ת

		PointF m_centerPos;
	};
}