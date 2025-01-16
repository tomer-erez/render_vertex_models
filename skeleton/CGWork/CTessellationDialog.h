#pragma once
#include "stdafx.h"
#include "CGWork.h"
#include "resource.h"

class CTessellationDialog : public CDialog {
    DECLARE_DYNAMIC(CTessellationDialog)

public:
    CTessellationDialog(CWnd* pParent = nullptr); // Constructor
    virtual ~CTessellationDialog();

    // Dialog Data
    enum { IDD = IDD_TESSELATION_DIALOG };
    int m_tessellationLevel; // User-selected tessellation level

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    DECLARE_MESSAGE_MAP()
};
