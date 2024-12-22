// PerspectiveDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "CGWork.h"
#include "PerspectiveDlg.h"
#include "afxdialogex.h"


// PerspectiveDlg dialog

IMPLEMENT_DYNAMIC(PerspectiveDlg, CDialog)

PerspectiveDlg::PerspectiveDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PERSPECTIVE, pParent)
	, d(0)
	, fovy(0)
	, nearPlane(0),
	farPlane(0),
	aspectRatio(0)
{

}

PerspectiveDlg::~PerspectiveDlg()
{
}

void PerspectiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, d);
	DDX_Text(pDX, IDC_EDIT2, fovy);
	DDX_Text(pDX, IDC_EDIT3, nearPlane);
	DDX_Text(pDX, IDC_EDIT4, farPlane);
	DDX_Text(pDX, IDC_EDIT5, aspectRatio);

}


BEGIN_MESSAGE_MAP(PerspectiveDlg, CDialog)
	ON_BN_CLICKED(IDC_PERSPECTIVEDEFAULTS, &PerspectiveDlg::OnBnClickedDefaultsButton)
END_MESSAGE_MAP()


// PerspectiveDlg message handlers


void PerspectiveDlg::OnBnClickedDefaultsButton()
{
	// TODO: Add your control notification handler code here
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	pApp->d = 5;
	d = pApp->d; // Example default value

	pApp->fovy = 45;
	fovy = pApp->fovy; // Example default value

	pApp->nearPlane = 45;
	nearPlane = pApp->nearPlane; // Example default value

	pApp->farPlane = 45;
	farPlane = pApp->farPlane; // Example default value

	pApp->aspectRatio = 45;
	aspectRatio = pApp->aspectRatio; // Example default value


	UpdateData(FALSE);
	//EndDialog(IDD_PERSPECTIVE);


}
