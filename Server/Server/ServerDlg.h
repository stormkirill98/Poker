
// ServerDlg.h : header file
//

#pragma once
#define DATA_BUFSIZE 8192 	// Размер буфера по умолчанию

typedef struct _SOCKET_INFORMATION {
	BOOL RecvPosted;
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	SOCKET Socket;
	DWORD BytesSEND;
	DWORD BytesRECV;
	_SOCKET_INFORMATION *Next;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

// CServerDlg dialog
class CServerDlg : public CDialogEx
{
	
// Construction
public:
	CServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;


	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBox;

	SOCKET* m_Accept;
	LPSOCKET_INFORMATION* m_SocketInfo;
	DWORD* m_RecvBytes;
	DWORD* m_SendBytes;
	DWORD* m_Flags;

	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnClickedSend();
};
