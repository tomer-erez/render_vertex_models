#include "stdafx.h"
#include "CGWork.h"
#include "CTessellationDialog.h"
#include "iritSkel.h"

extern IPFreeformConvStateStruct CGSkelFFCState;

// Implement runtime dynamic class support
IMPLEMENT_DYNAMIC(CTessellationDialog, CDialog)

// Implementation
CTessellationDialog::CTessellationDialog(CWnd* pParent /*=nullptr*/)
    : CDialog(IDD_TESSELATION_DIALOG, pParent), m_tessellationLevel(1) {}

CTessellationDialog::~CTessellationDialog() {}

void CTessellationDialog::DoDataExchange(CDataExchange* pDX) {
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_TESSELATION_EDIT, m_tessellationLevel);
    DDV_MinMaxInt(pDX, m_tessellationLevel, 1, 100); // Ensure level is between 1 and 100
    CGSkelFFCState.FineNess = m_tessellationLevel;
}

BEGIN_MESSAGE_MAP(CTessellationDialog, CDialog)
END_MESSAGE_MAP()
