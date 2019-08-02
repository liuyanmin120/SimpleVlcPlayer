
// SimpleVlcPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimpleVlcPlayer.h"
#include "SimpleVlcPlayerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HWND gPlayHwnd = NULL;
//ö��VLC����
BOOL CALLBACK EnumerateVLC(HWND hWndvlc, LPARAM lParam)
{
	TCHAR szWndTitle[1024];
	int nLen = GetWindowText(hWndvlc, szWndTitle, 1024);
	if (0 != nLen)
	{
		//���������Ϣ
		EnableWindow(hWndvlc, FALSE);
	}
	KillTimer(NULL, 1);
	return TRUE;
}

//��ʱ���ص�
void CALLBACK TimeProc(
	HWND hwnd,
	UINT message,
	UINT idTimer,
	DWORD dwTime)
{
	EnumChildWindows(gPlayHwnd, EnumerateVLC, NULL);
}
// CAboutDlg dialog used for App About
CStringA UnicodeToUTF8(const CStringW& strWide);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSimpleVlcPlayerDlg dialog



CSimpleVlcPlayerDlg::CSimpleVlcPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SIMPLEVLCPLAYER_DIALOG, pParent),
	m_strFilePath(L""),
	m_bIsFullScreen(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimpleVlcPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLAYWND, m_playWnd);
}

BEGIN_MESSAGE_MAP(CSimpleVlcPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSimpleVlcPlayerDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CSimpleVlcPlayerDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSimpleVlcPlayerDlg::OnBnClickedButtonStop)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CSimpleVlcPlayerDlg message handlers

BOOL CSimpleVlcPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(0, 0, 0, 80);
	m_playWnd.MoveWindow(rc);

	gPlayHwnd = GetDlgItem(IDC_PLAYWND)->GetSafeHwnd();
	m_myPlayer.SetHWND(gPlayHwnd);   // ���ò������Ĵ��ھ��

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimpleVlcPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimpleVlcPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimpleVlcPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSimpleVlcPlayerDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	//���ù�����   
	TCHAR szFilter[] = _T("��Ƶ�ļ�|*.mp4;*.rmvb;*.mkv|�����ļ�(*.*)|*.*||");
	// ������ļ��Ի���   
	CFileDialog fileDlg(TRUE, _T("ѡ����Ƶ�ļ�"), NULL, 0, szFilter, this);

	// ��ʾ���ļ��Ի���   
	if (IDOK == fileDlg.DoModal())
	{
		// ���������ļ��Ի����ϵġ��򿪡���ť����ѡ����ļ�·����ʾ���༭����   
		m_strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_PATH, m_strFilePath);
	}
	
}


void CSimpleVlcPlayerDlg::OnBnClickedButtonPlay()
{
	// TODO: Add your control notification handler code here
	CString strBtnName = L"";
	GetDlgItemText(IDC_BUTTON_PLAY, strBtnName);
	GetDlgItemText(IDC_EDIT_PATH, m_strFilePath);


	if (L"����" == strBtnName)
	{
		if (!m_myPlayer.IsOpen())
		{
			if (m_strFilePath != L"")
			{
				m_myPlayer.Play((LPCSTR)UnicodeToUTF8(m_strFilePath));
				::SetTimer(NULL, 1, 1000, (TIMERPROC)TimeProc);
				SetDlgItemText(IDC_BUTTON_PLAY, L"��ͣ");
			}
		}
		if (!m_myPlayer.IsPlaying() && m_myPlayer.IsOpen())
		{
			m_myPlayer.Play();
			SetDlgItemText(IDC_BUTTON_PLAY, L"��ͣ");
		}
	}
	else
	{
		if (m_myPlayer.IsPlaying())
		{
			m_myPlayer.Pause();
			SetDlgItemText(IDC_BUTTON_PLAY, L"����");
		}
	}
}


void CSimpleVlcPlayerDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	if (m_myPlayer.IsOpen())
	{
		m_myPlayer.Stop();
		SetDlgItemText(IDC_BUTTON_PLAY, L"����");
	}
}


void CSimpleVlcPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_myPlayer.IsOpen())
	{
		SetFullScreen(!m_bIsFullScreen);
	}
	
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CSimpleVlcPlayerDlg::SetFullScreen(BOOL full)
{
	m_bIsFullScreen = full;

	if (full)
	{
		HideControl(TRUE);
		//���ر�����
		SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) - WS_CAPTION);
		ShowWindow(SW_MAXIMIZE);
		CRect rc;
		GetClientRect(&rc);
		m_playWnd.MoveWindow(rc);
	}
	else
	{
		HideControl(FALSE);
		SetWindowLong(this->GetSafeHwnd(), GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) + WS_CAPTION);
		ShowWindow(SW_NORMAL);
		CRect rc;
		GetClientRect(&rc);
		rc.DeflateRect(0, 0, 0, 80);
		m_playWnd.MoveWindow(rc);
	}
}

void CSimpleVlcPlayerDlg::HideControl(BOOL isHide)
{
	((CButton *)GetDlgItem(IDC_BUTTON_OPEN))->ShowWindow(!isHide);
	((CButton *)GetDlgItem(IDC_BUTTON_PLAY))->ShowWindow(!isHide);
	((CButton *)GetDlgItem(IDC_BUTTON_STOP))->ShowWindow(!isHide);
	((CEdit *)GetDlgItem(IDC_EDIT_PATH))->ShowWindow(!isHide);
}

CStringA UnicodeToUTF8(const CStringW& strWide)
{
	CStringA strUTF8;
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, strWide, -1, NULL, 0, NULL, NULL);

	if (nLen > 1)
	{
		::WideCharToMultiByte(CP_UTF8, 0, strWide, -1, strUTF8.GetBuffer(nLen - 1), nLen, NULL, NULL);
		strUTF8.ReleaseBuffer();
	}

	return strUTF8;
}