// CGWorkView.h : interface of the CCGWorkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
#define AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Poly.h"
#include "Scene.h"
#include "Matrix4.h"
#include "Point.h"
#include "gl\gl.h"    // Include the standard CGWork  headers
#include "gl\glu.h"   // Add the utility library


#include "Light.h"

class CCGWorkView : public CView
{
protected: // create from serialization only
	CCGWorkView();
	DECLARE_DYNCREATE(CCGWorkView)

	// Attributes
public:
	CCGWorkDoc* GetDocument();

	// Operations
public:

public:
	CSliderCtrl m_finenessSlider; // Slider control for tessellation fineness

	bool m_draw_poly_normals; //flag to choose whether to draw poly normals
	bool m_draw_vertex_normals;//flag to choose whether to draw vertex normals
	bool m_draw_bounding_box;
	bool m_uniform_color;
	bool m_draw_poly_normals_from ; //poly normals form file
	bool m_draw_poly_normals_not_from ; // poly normals not from file
	bool m_draw_vertex_normals_from ; //vertex normals from file
	bool m_draw_vertex_normals_not_from ; // vertex normal nor from file
	int m_draw_silhouettes;
	int m_flip_normals;
	bool m_render_to_screen;
	int m_nAxis;				// Axis of Action, X Y or Z
	int m_nAction;				// Rotate, Translate, Scale
	int m_nView;				// Orthographic, perspective
	bool m_bIsPerspective;			// is the view perspective
	bool m_do_back_face_culling;
	bool m_solid_rendering;
	CString m_strItdFileName;		// file name of IRIT data

	int m_nLightShading;			// shading: Flat, Gouraud.
	bool m_back_ground_image_was_set;
	int m_is_object_space_transform; // is object or view space transform
	bool m_back_ground_image_on;
	bool m_back_ground_image_stretch;
	double m_lMaterialAmbient;		// The Ambient in the scene
	double m_lMaterialDiffuse;		// The Diffuse in the scene
	double m_lMaterialSpecular;		// The Specular in the scene
	int m_nMaterialCosineFactor;		// The cosine factor for the specular

	bool m_anti_aliasing_5x5;
	bool m_anti_aliasing_None;
	bool m_anti_aliasing_Box;
	bool m_anti_aliasing_Triangle;
	bool m_anti_aliasing_Sinc;
	bool m_anti_aliasing_Gaussian;

	LightParams m_lights[MAX_LIGHT];	//configurable lights array
	LightParams m_ambientLight;		//ambient light (only RGB is used)
	CPoint prev_start;
	bool m_directional_light;
	bool m_positional_light;
	bool m_fog_effects_on;
	bool m_3DVOLUMETRICTEXTURE_MARBLE;
	bool m_3DVOLUMETRICTEXTURE_WOOD;
	bool m_3DVOLUMETRICTEXTURE_NONE;


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CCGWorkView)
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCGWorkView();

	bool m_isDragging;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitializeCGWork();
	BOOL SetupViewingFrustum(void);
	BOOL SetupViewingOrthoConstAspect(void);

	virtual void RenderScene();

	// Command Handlers
	void OnOptionsPolygonFineness();             // Opens the polygon fineness dialog
	void OnUpdateOptionsPolygonFineness(CCmdUI* pCmdUI); // Enables the menu item



	HGLRC    m_hRC;			// holds the Rendering Context
	CDC* m_pDC;			// holds the Device Context
	int m_WindowWidth;		// hold the windows width
	int m_WindowHeight;		// hold the windows height
	double m_AspectRatio;		// hold the fixed Aspect Ration

	HBITMAP m_pDbBitMap;
	CDC* m_pDbDC;

	// New helper function declarations
private:
	void InitializeFinenessSlider(); // Initializes the fineness slider
	void OnFinenessSliderChanged();  // Handles fineness slider changes
	void UpdateSceneForFineness();
	void DrawPolygonEdgesAndVertexNormals(Point* oBuffer, size_t width, size_t height, Poly* poly, Vector4 cameraPosition, COLORREF edgeColor, COLORREF normalColor);
	void DrawPolygonNormal(Point* oBuffer, size_t width, size_t height, Poly* poly, COLORREF color);
	void DrawBoundingBox(Point* oBuffer, size_t width, size_t height, const BoundingBox& bbox, COLORREF color);
	void DrawLineHelper(Point* oBuffer, size_t width, size_t height, const Vector4& start, const Vector4& end, COLORREF color);

	
	Matrix4 getMatrixToCenterObject();

	void ApplyXRotation(int d);
	void ApplyYRotation(int d);
	void ApplyZRotation(int d);

	void ApplyXTranslation(int d);
	void ApplyYTranslation(int d);
	void ApplyZTranslation(int d);

	void ApplyXScale(double d);
	void ApplyYScale(double d);
	void ApplyZScale(double d);


	void ApplyTransformation(Matrix4& t);
	void MapMouseMovement(int deg);
		
	void OnFlipNormals();
	void OnDrawSilhouettes(); 
	// Generated message map functions
	void DrawSilhouettes(CDC* pDC, double screenHeight, COLORREF silhouetteColor);

	void OnRenderToFile();
	void OnRenderToScreen();
	void OnBackFaceCulling();
	void OnSolidRendering();
	void OnBackGroundImageStretch();
	void OnBackGroundImageOn();
	void renderToBitmap(Point* bgBuffer, Point* edgesBuffer, Point* normalsBuffer, Point* polygonsBuffer, Point* boundingBoxBuffer, int width, 
		int height, CDC* pDC, COLORREF bg_color, Vector4 cameraPosition, COLORREF fog_color, bool use_fog, int kernelSize, const std::string& filterName);

	void PerspectiveView(Point* polygonsBuffer, int width, int height);
	void PerspectiveTransformPoly();

protected:
	//{{AFX_MSG(CCGWorkView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);



	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoad();
	afx_msg void OnFileLoadBackGroundImage();
	afx_msg void OnViewOrthographic();
	afx_msg void OnUpdateViewOrthographic(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnActionRotate();
	afx_msg void OnUpdateActionRotate(CCmdUI* pCmdUI);
	afx_msg void OnActionScale();
	afx_msg void OnUpdateActionScale(CCmdUI* pCmdUI);
	afx_msg void OnActionTranslate();
	afx_msg void OnUpdateActionTranslate(CCmdUI* pCmdUI);
	afx_msg void OnAxisX();
	afx_msg void OnUpdateAxisX(CCmdUI* pCmdUI);
	afx_msg void OnAxisY();
	afx_msg void OnUpdateAxisY(CCmdUI* pCmdUI);
	afx_msg void OnAxisZ();
	afx_msg void OnUpdateAxisZ(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingFlat();
	afx_msg void OnUpdateLightShadingFlat(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingGouraud();
	afx_msg void OnUpdateLightShadingGouraud(CCmdUI* pCmdUI);
	afx_msg void OnLightConstants();
	afx_msg void OnLightShadingPhong();
	afx_msg void OnUpdateLightShadingPhong(CCmdUI* pCmdUI);

	afx_msg void OnPerspectiveParameters();
	afx_msg void OnOptionsMousesensitivity();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnBoundingBox();
	afx_msg void OnUpdateBoundingBox(CCmdUI* pCmdUI);

	afx_msg void OnVertexNormal();
	afx_msg void OnUpdateVertexNormal(CCmdUI* pCmdUI);

	afx_msg void OnPolyNormal();
	afx_msg void OnUpdatePolyNormal(CCmdUI* pCmdUI);
	/*
	poly and vert from and not from
	*/

	afx_msg void OnPolyNormalsNotFrom();
	afx_msg void OnUpdatePolyNormalsNotFrom(CCmdUI* pCmdUI);


	afx_msg void OnPolyNormalsFrom();
	afx_msg void OnUpdatePolyNormalsFrom(CCmdUI* pCmdUI);


	afx_msg void OnVertexNormalsFrom();
	afx_msg void OnUpdateVertexNormalsFrom(CCmdUI* pCmdUI);

	afx_msg void OnVertexNormalsNotFrom();
	afx_msg void OnUpdateVertexNormalsNotFrom(CCmdUI* pCmdUI);

	afx_msg void OnPositionalLight();
	afx_msg void OnUpdatePositionalLight(CCmdUI* pCmdUI);

	afx_msg void OnViewObject();
	afx_msg void OnUpdateViewObject(CCmdUI* pCmdUI);

	afx_msg void OnViewView();
	afx_msg void OnUpdateViewView(CCmdUI* pCmdUI);

	afx_msg void OnUpdateDrawSilhouettes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFlipNormals(CCmdUI* pCmdUI);

	afx_msg void OnUpdateRenderToFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRenderToScreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBackFaceCulling(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSolidRendering(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBackGroundImageOn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBackGroundImageStretch(CCmdUI* pCmdUI);
	afx_msg void OnMaterialDlg();

	// 5x5 Anti-aliasing
	afx_msg void OnAntiAliasing5x5();
	afx_msg void OnUpdateAntiAliasing5x5(CCmdUI* pCmdUI);

	// None Anti-aliasing
	afx_msg void OnAntiAliasingNone();
	afx_msg void OnUpdateAntiAliasingNone(CCmdUI* pCmdUI);

	// Box Anti-aliasing
	afx_msg void OnAntiAliasingBox();
	afx_msg void OnUpdateAntiAliasingBox(CCmdUI* pCmdUI);

	// Triangle Anti-aliasing
	afx_msg void OnAntiAliasingTriangle();
	afx_msg void OnUpdateAntiAliasingTriangle(CCmdUI* pCmdUI);

	// Sinc Anti-aliasing
	afx_msg void OnAntiAliasingSinc();
	afx_msg void OnUpdateAntiAliasingSinc(CCmdUI* pCmdUI);

	// Gaussian Anti-aliasing
	afx_msg void OnAntiAliasingGaussian();
	afx_msg void OnUpdateAntiAliasingGaussian(CCmdUI* pCmdUI);

	// Gaussian Anti-aliasing
	afx_msg void OnFogEffectsOn();
	afx_msg void OnUpdateFogEffectsOn(CCmdUI* pCmdUI);

	// 3d volumetric none
	afx_msg void On3dVolumetricNone();
	afx_msg void OnUpdate3dVolumetricNone(CCmdUI* pCmdUI);

	// 3d volumetric wood
	afx_msg void On3dVolumetricWood();
	afx_msg void OnUpdate3dVolumetricWood(CCmdUI* pCmdUI);

	// 3d volumetric marble
	afx_msg void On3dVolumetricMarle();
	afx_msg void OnUpdate3dVolumetricMarble(CCmdUI* pCmdUI);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
{
	return (CCGWorkDoc*)m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)