#pragma once

namespace DuiLib
{
#define EVENT_TIME_ID	100

	class CChrysanthemumLoadingUI : public CControlUI
	{
	public:
		CChrysanthemumLoadingUI(void);
		~CChrysanthemumLoadingUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		void	DoInit();
		bool	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void	DoEvent(TEventUI& event);		
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	SetVisible(bool bVisible = true );

		virtual void SetPos(RECT rc, bool bNeedInvalidate = true);

		void SetTimeInterval(const int& nValue);
		int GetTimeInterval();
		void SetNumberOfSpoke(const int& nValue);
		int GetNumberOfSpoke();
		void SetSpokeThickness(const int& nValue);
		int GetSpokeThickness();
		void SetOuterCircleRadius(const int& nValue);
		int GetOuterCircleRadius();
		void SetInnerCircleRadius(const int& nValue);
		int GetInnerCircleRadius();

		DWORD GetBkColor() const;
		void SetBkColor(DWORD dwBackColor);
		DWORD GetSpokeColor() const;
		void SetSpokeColor(DWORD dwSpokeColor);

		void SetTextColor(DWORD dwTextColor);
		DWORD GetTextColor() const;
		void SetFont(int index);
		int GetFont() const;
		RECT GetTextPadding() const;
		void SetTextPadding(RECT rc);
		void SetTextRenderingAlias(int nTextRenderingAlias);
		TextRenderingHint GetTextRenderingAlias();
		bool IsMultiLine();
		void SetMultiLine(bool bMultiLine = true);

	protected:
		void InitChrysanthemumLoading();
		void PaintBitMap();
	
	protected:
		int		m_nTimeInterval;			//ʱ���� ��λms
		int		m_nNumberOfSpoke;			//��������	
		int		m_nSpokeThickness;			//������ϸ
		int		m_nProgressValue;			//
		int		m_nOuterCircleRadius;		//��Ȧ�뾶 ��λ������
		int		m_nInnerCircleRadius;		//��Ȧ�뾶
		PointF	m_CenterPoint;				//����Բ��
		DWORD	m_dwSpokeColor;				//������ɫ

		Color*	m_pColors;					//��������ɫ
		double* m_pAngles;					//�����ĽǶ�

		UINT	m_uTextStyle;
		DWORD	m_dwTextColor;
		int		m_iFont;
		RECT	m_rcTextPadding;
		TextRenderingHint		m_TextRenderingAlias;	//���ֿ���ݼ���

		Bitmap* m_pgdipBmp;
	};
}	// namespace DuiLib
