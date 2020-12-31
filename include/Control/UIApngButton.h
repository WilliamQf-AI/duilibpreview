#ifndef ApngButtonUI_h__
#define ApngButtonUI_h__

namespace DuiLib
{
	class DUILIB_API CApngButtonUI : public CApngAnimUI
	{
	public:
		CApngButtonUI(void);
		~CApngButtonUI(void);

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void SetEnabled(bool bEnable = true);
		void DoEvent(TEventUI& event);

		void PaintBkColor(HDC hDC);
		void PaintText(HDC hDC);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		CDuiString GetCursor();
		void SetCursor(LPCTSTR pStrCursor);

		void SetHotBkColor(DWORD dwColor);
		DWORD GetHotBkColor() const;
		void SetHotTextColor(DWORD dwColor);
		DWORD GetHotTextColor() const;
		void SetPushedTextColor(DWORD dwColor);
		DWORD GetPushedTextColor() const;

		UINT GetButtonState() {return m_uButtonState;};
	protected:
		UINT m_uButtonState;
		CDuiString m_sCursor;			//�����״ arrow/hand ����������Ҫ���

		DWORD m_dwHotBkColor;
		DWORD m_dwHotTextColor;
		DWORD m_dwPushedTextColor;
	};
}

#endif // ApngButtonUI_h__
