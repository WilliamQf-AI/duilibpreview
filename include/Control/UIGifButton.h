#pragma once

namespace DuiLib
{
	class CGifButtonUI :	public CGifAnimUI
	{
	public:
		CGifButtonUI(void);
		~CGifButtonUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);
		bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		void SetBkImageDest(const RECT& rcDest);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		CDuiString GetCursor();
		void SetCursor(LPCTSTR pStrCursor);

		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;

		void DrawFrame( HDC hDC );		// ����GIFÿ֡

		UINT GetButtonState() {return m_uButtonState;};
	protected:
		UINT m_uButtonState;
		CDuiString m_sCursor;			//�����״ arrow/hand ����������Ҫ���
		DWORD	m_dwHotTextColor;
		RECT m_rcBkImageDest;
	};
}