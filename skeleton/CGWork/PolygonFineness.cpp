// PolygonFineness.cpp : implementation file
//
#include "StdAfx.h"
#include "CGWork.h"
#include "PolygonFineness.h"
#include "afxdialogex.h"
#include "iritSkel.h"

extern IPFreeformConvStateStruct CGSkelFFCState;

// PolygonFineness dialog

IMPLEMENT_DYNAMIC(PolygonFineness, CDialog)

PolygonFineness::PolygonFineness(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_FINENESS_CONTROL, pParent)
{

}

PolygonFineness::~PolygonFineness()
{
}

void PolygonFineness::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_FINENESS, sliderForPolyFineness);

	CGSkelFFCState.FineNess = sliderForPolyFineness.GetPos();;
	int pause = 0;
}

void PolygonFineness::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	if (pScrollBar->GetSafeHwnd() == sliderForPolyFineness.GetSafeHwnd()) {
		// Update slider value
		int sliderValue = sliderForPolyFineness.GetPos();
		pApp->p_slider_polyFineness_value = sliderValue;
		CGSkelFFCState.FineNess = sliderValue;

		TRACE("Slider moved. New value: %d\n", sliderValue);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
BOOL PolygonFineness::OnInitDialog() {
	CDialog::OnInitDialog();

	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	// Initialize slider range and position
	sliderForPolyFineness.SetRange(2, 60);

	// Use the current fineness value to set the slider position
	int initialValue = static_cast<int>(CGSkelFFCState.FineNess);
	sliderForPolyFineness.SetPos(initialValue);

	TRACE("Slider initialized to: %d\n", initialValue);

	return TRUE; // Return TRUE unless you set focus to a control
}



BEGIN_MESSAGE_MAP(PolygonFineness, CDialog)
END_MESSAGE_MAP()


// PolygonFineness message handlers
