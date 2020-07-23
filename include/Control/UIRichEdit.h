#ifndef __UIRICHEDIT_H__
#define __UIRICHEDIT_H__

#pragma once

namespace DuiLib {

class CTxtWinHost;

class DUILIB_API CRichEditUI : public CContainerUI, public IMessageFilterUI
{
public:
    CRichEditUI();
    ~CRichEditUI();

    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    bool IsWantTab();
    void SetWantTab(bool bWantTab = true);
    bool IsWantReturn();
    void SetWantReturn(bool bWantReturn = true);
    bool IsWantCtrlReturn();
    void SetWantCtrlReturn(bool bWantCtrlReturn = true);
    bool IsTransparent();
    void SetTransparent(bool bTransparent = true);
    bool IsRich();
    void SetRich(bool bRich = true);
    bool IsReadOnly();
    void SetReadOnly(bool bReadOnly = true);
    bool IsWordWrap();
    void SetWordWrap(bool bWordWrap = true);
    int GetFont();
    void SetFont(int index);
    void SetFont(LPCTSTR pStrFontName, int nSize, bool bBold, bool bUnderline, bool bItalic, bool bStrikeOut);
    LONG GetWinStyle();
    void SetWinStyle(LONG lStyle);
    DWORD GetTextColor();
    void SetTextColor(DWORD dwTextColor);
    int GetLimitText();
    void SetLimitText(int iChars);
    long GetTextLength(DWORD dwFlags = GTL_DEFAULT) const;
    CDuiString GetText() const;
    void SetText(const CDuiString& strText);
    bool IsModify() const;
    void SetModify(bool bModified = true) const;
    void GetSel(CHARRANGE &cr) const;
    void GetSel(long& nStartChar, long& nEndChar) const;
    int SetSel(CHARRANGE &cr);
    int SetSel(long nStartChar, long nEndChar);
    void ReplaceSel(const CDuiString& strNewText, bool bCanUndo);
    void ReplaceSelW(LPCWSTR lpszNewText, bool bCanUndo = false);
    CDuiString GetSelText() const;
    int SetSelAll();
    int SetSelNone();
    WORD GetSelectionType() const;
    bool GetZoom(int& nNum, int& nDen) const;
    bool SetZoom(int nNum, int nDen);
    bool SetZoomOff();
    bool GetAutoURLDetect() const;
    bool SetAutoURLDetect(bool bAutoDetect = true);
    DWORD GetEventMask() const;
    DWORD SetEventMask(DWORD dwEventMask);
    CDuiString GetTextRange(long nStartChar, long nEndChar) const;
    void HideSelection(bool bHide = true, bool bChangeStyle = false);
    void ScrollCaret();
    int InsertText(long nInsertAfterChar, LPCTSTR lpstrText, bool bCanUndo = false);
    int AppendText(LPCTSTR lpstrText, bool bCanUndo = false);
    DWORD GetDefaultCharFormat(CHARFORMAT2 &cf) const;
    bool SetDefaultCharFormat(CHARFORMAT2 &cf);
    DWORD GetSelectionCharFormat(CHARFORMAT2 &cf) const;
    bool SetSelectionCharFormat(CHARFORMAT2 &cf);
    bool SetWordCharFormat(CHARFORMAT2 &cf);
    DWORD GetParaFormat(PARAFORMAT2 &pf) const;
    bool SetParaFormat(PARAFORMAT2 &pf);
    bool Redo();
    bool Undo();
    void Clear();
    void Copy();
    void Cut();
    void Paste();
    int GetLineCount() const;
    CDuiString GetLine(int nIndex, int nMaxLength) const;
    int LineIndex(int nLine = -1) const;
    int LineLength(int nLine = -1) const;
    bool LineScroll(int nLines, int nChars = 0);
	CDuiPoint GetCharPos(long lChar) const;
    long LineFromChar(long nIndex) const;
    CDuiPoint PosFromChar(UINT nChar) const;
    int CharFromPos(CDuiPoint pt) const;
    void EmptyUndoBuffer();
    UINT SetUndoLimit(UINT nLimit);
    long StreamIn(int nFormat, EDITSTREAM &es);
    long StreamOut(int nFormat, EDITSTREAM &es);

	RECT GetTextPadding() const;
	void SetTextPadding(RECT rc);
	DWORD GetTipValueTextColor();
	void SetTipValueTextColor(DWORD dwTextColor);
	CDuiString GetTipValueText() const;
	void SetTipValueText(const CDuiString& strText);

	void SetMultLine(bool bMultLine = true);
	bool GetMultLine();

	void SetVscrollbar(bool bVscrollbar);
	bool GetVscrollbar();
	void SetHscrollbar(bool bHscrollbar);
	bool GetHsrcollbar();
	void SetAutoVscroll(bool bAutoVscroll);
	bool GetAutoVscroll();
	void SetAutoHscroll(bool bAutoHscroll);
	bool GetAutoHscroll();

    void DoInit();
	bool SetDropAcceptFile(bool bAccept);
	// ע�⣺TxSendMessage��SendMessage��������ģ�TxSendMessageû��multibyte��unicode�Զ�ת���Ĺ��ܣ�
	// ��richedit2.0�ڲ�����unicodeʵ�ֵģ���multibyte�����У������Լ�����unicode��multibyte��ת��
    virtual HRESULT TxSendMessage(UINT msg, WPARAM wparam, LPARAM lparam, LRESULT *plresult) const; 
    IDropTarget* GetTxDropTarget();
    virtual bool OnTxViewChanged();
    virtual void OnTxNotify(DWORD iNotify, void *pv);

    void SetScrollPos(SIZE szPos,bool bTriggerEvent=true);
    void LineUp();
    void LineDown();
    void PageUp();
    void PageDown();
    void HomeUp();
    void EndDown();
    void LineLeft();
    void LineRight();
    void PageLeft();
    void PageRight();
    void HomeLeft();
    void EndRight();

	int EstimmateRichEditHeight(const int& nWidth,LPCTSTR lpszText);
    SIZE EstimateSize(SIZE szAvailable);
	void SetPos(RECT rc, bool bNeedInvalidate = true);
	void Move(SIZE szOffset, bool bNeedInvalidate = true);
    void DoEvent(TEventUI& event);
    bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
	void PaintStatusImage(HDC hDC);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void SetCaret(bool bCaret);
	bool GetCaret();

	void SetPasswordChar(TCHAR chPasswordChar);
	void SetPassword(bool bPassword = true);
	bool GetPassword();

	CDuiString GetFocusedImage();
	void SetFocusedImage(const CDuiString& strImage);
	CDuiString GetDisabledImage();
	void SetDisabledImage(const CDuiString& strImage);

    LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

protected:
	enum { 
		DEFAULT_TIMERID = 20,
	};

    CTxtWinHost* m_pTwh;
    bool m_bVScrollBarFixing;
    bool m_bWantTab;
    bool m_bWantReturn;
    bool m_bWantCtrlReturn;
    bool m_bTransparent;
    bool m_bRich;
    bool m_bReadOnly;
    bool m_bWordWrap;
    DWORD m_dwTextColor;
    int m_iFont;
    int m_iLimitText;
    LONG m_lTwhStyle;
	bool m_bCaret;
	bool m_bDrawCaret;
	bool m_bInited;
	RECT	m_rcTextPadding;
	CDuiString m_strTipValue;
	DWORD m_dwTipValueColor;

	TDrawInfo m_diFocused;
	TDrawInfo m_diDisabled;
};

} // namespace DuiLib

#endif // __UIRICHEDIT_H__
