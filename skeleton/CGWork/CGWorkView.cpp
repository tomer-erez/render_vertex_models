// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "stdafx.h"
#include "CGWork.h"
#include "ScanConvertZBuffer.h"
#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <algorithm> // Required for std::min
#undef min           // Prevent conflicts with Windows macros

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"
#include "PerspectiveDlg.h"
#include "MouseSensitivityDlg.h"
#include "PolygonFineness.h"
#include <thread>
#include <cmath>
#include "AntiAliasing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"
extern IPFreeformConvStateStruct CGSkelFFCState; // access to the polygon tesselation finess
#include "LineDrawer.h"


// For Status Bar access
#include "MainFrm.h"

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))

/*lets use the global scene container*/
#include "Scene.h"
#include "Vertex.h"


#define MOUSE_FACTOR 5

extern Scene scene;



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView

IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_BACKGROUNDIMAGE_LOAD, OnFileLoadBackGroundImage)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	ON_COMMAND(ID_SHADING_PHONG, OnLightShadingPhong)
	ON_UPDATE_COMMAND_UI(ID_SHADING_PHONG, OnUpdateLightShadingPhong)

	ON_COMMAND(ID_OPTIONS_PERSPECTIVECONTROL, OnPerspectiveParameters)
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, &CCGWorkView::OnOptionsMousesensitivity)

	ON_COMMAND(ID_VIEW_BOUNDINGBOX, OnBoundingBox)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BOUNDINGBOX, OnUpdateBoundingBox)

	ON_COMMAND(ID_VIEW_VERTEXNORMAL, OnVertexNormal)
	ON_UPDATE_COMMAND_UI(ID_VIEW_VERTEXNORMAL, OnUpdateVertexNormal)

	ON_COMMAND(ID_VIEW_POLYGONNORMALS, OnPolyNormal)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POLYGONNORMALS, OnUpdatePolyNormal)

	ON_COMMAND(ID_OPTIONS_POLYGONFINENESS, &CCGWorkView::OnOptionsPolygonFineness)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_POLYGONFINENESS, &CCGWorkView::OnUpdateOptionsPolygonFineness)

	//vertexNormal and poly normal from file and not from file:

	//poly from file
// Polygon normals not from file
ON_COMMAND(ID_VIEW_POLYGONNORMALSNOTFROM, OnPolyNormalsNotFrom)
ON_UPDATE_COMMAND_UI(ID_VIEW_POLYGONNORMALSNOTFROM, OnUpdatePolyNormalsNotFrom)
// Polygon normals from file
ON_COMMAND(ID_VIEW_POLYGONNORMALSFROM, OnPolyNormalsFrom)
ON_UPDATE_COMMAND_UI(ID_VIEW_POLYGONNORMALSFROM, OnUpdatePolyNormalsFrom)
// Vertex normals from file
ON_COMMAND(ID_VIEW_VERTEXNORMALSFROM, OnVertexNormalsFrom)
ON_UPDATE_COMMAND_UI(ID_VIEW_VERTEXNORMALSFROM, OnUpdateVertexNormalsFrom)
// Vertex normals not from file
ON_COMMAND(ID_VIEW_VERTEXNORMALSNOTFROM, OnVertexNormalsNotFrom)
ON_UPDATE_COMMAND_UI(ID_VIEW_VERTEXNORMALSNOTFROM, OnUpdateVertexNormalsNotFrom)

ON_COMMAND(ID_VIEW_VIEW, &CCGWorkView::OnViewView)
ON_COMMAND(ID_VIEW_OBJECT, &CCGWorkView::OnViewObject)

ON_UPDATE_COMMAND_UI(ID_VIEW_VIEW, &CCGWorkView::OnUpdateViewView)
ON_UPDATE_COMMAND_UI(ID_VIEW_OBJECT, &CCGWorkView::OnUpdateViewObject)

ON_COMMAND(ID_VIEW_DRAW_SILS, &CCGWorkView::OnDrawSilhouettes)
ON_UPDATE_COMMAND_UI(ID_VIEW_DRAW_SILS, &CCGWorkView::OnUpdateDrawSilhouettes)

ON_COMMAND(ID_DRAWS_FLIP_NORMALS, &CCGWorkView::OnFlipNormals)
ON_UPDATE_COMMAND_UI(ID_DRAWS_FLIP_NORMALS, &CCGWorkView::OnUpdateFlipNormals)

ON_COMMAND(ID_RENDER_TOFILE, &CCGWorkView::OnRenderToFile)
ON_UPDATE_COMMAND_UI(ID_RENDER_TOFILE, &CCGWorkView::OnUpdateRenderToFile)

ON_COMMAND(ID_RENDER_TOSCREEN, &CCGWorkView::OnRenderToScreen)
ON_UPDATE_COMMAND_UI(ID_RENDER_TOSCREEN, &CCGWorkView::OnUpdateRenderToScreen)

ON_COMMAND(ID_VIEW_BACKFACECULLING, &CCGWorkView::OnBackFaceCulling)
ON_UPDATE_COMMAND_UI(ID_VIEW_BACKFACECULLING, &CCGWorkView::OnUpdateBackFaceCulling)

ON_COMMAND(ID_VIEW_SOLIDRENDERING, &CCGWorkView::OnSolidRendering)
ON_UPDATE_COMMAND_UI(ID_VIEW_SOLIDRENDERING, &CCGWorkView::OnUpdateSolidRendering)

ON_COMMAND(ID_BACKGROUNDIMAGE_STRETCH, &CCGWorkView::OnBackGroundImageStretch)
ON_UPDATE_COMMAND_UI(ID_BACKGROUNDIMAGE_STRETCH, &CCGWorkView::OnUpdateBackGroundImageStretch)

ON_COMMAND(ID_BACKGROUNDIMAGE_ON, &CCGWorkView::OnBackGroundImageOn)
ON_UPDATE_COMMAND_UI(ID_BACKGROUNDIMAGE_ON, &CCGWorkView::OnUpdateBackGroundImageOn)

// 5x5 Anti-aliasing
ON_COMMAND(ID_ANTIALIASING_5X5, &CCGWorkView::OnAntiAliasing5x5)
ON_UPDATE_COMMAND_UI(ID_ANTIALIASING_5X5, &CCGWorkView::OnUpdateAntiAliasing5x5)

// None Anti-aliasing
ON_COMMAND(ID_ANTIALIASING_NONE, &CCGWorkView::OnAntiAliasingNone)
ON_UPDATE_COMMAND_UI(ID_ANTIALIASING_NONE, &CCGWorkView::OnUpdateAntiAliasingNone)

// Box Anti-aliasing
ON_COMMAND(ID_ANTIALIASING_BOX, &CCGWorkView::OnAntiAliasingBox)
ON_UPDATE_COMMAND_UI(ID_ANTIALIASING_BOX, &CCGWorkView::OnUpdateAntiAliasingBox)

// Triangle Anti-aliasing
ON_COMMAND(ID_ANTIALIASING_TRIANGLE, &CCGWorkView::OnAntiAliasingTriangle)
ON_UPDATE_COMMAND_UI(ID_ANTIALIASING_TRIANGLE, &CCGWorkView::OnUpdateAntiAliasingTriangle)

// Sinc Anti-aliasing
ON_COMMAND(ID_ANTIALIASING_SINC, &CCGWorkView::OnAntiAliasingSinc)
ON_UPDATE_COMMAND_UI(ID_ANTIALIASING_SINC, &CCGWorkView::OnUpdateAntiAliasingSinc)

// Gaussian Anti-aliasing
ON_COMMAND(ID_ANTIALIASING_GAUSSIAN, &CCGWorkView::OnAntiAliasingGaussian)
ON_UPDATE_COMMAND_UI(ID_ANTIALIASING_GAUSSIAN, &CCGWorkView::OnUpdateAntiAliasingGaussian)

// fog effects
ON_COMMAND(ID_FOGEFFECTS_ON, &CCGWorkView::OnFogEffectsOn)
ON_UPDATE_COMMAND_UI(ID_FOGEFFECTS_ON, &CCGWorkView::OnUpdateFogEffectsOn)

ON_COMMAND(ID_3DVOLUMETRICTEXTURE_NONE, &CCGWorkView::On3dVolumetricNone)
ON_UPDATE_COMMAND_UI(ID_3DVOLUMETRICTEXTURE_NONE, &CCGWorkView::OnUpdate3dVolumetricNone)
ON_COMMAND(ID_3DVOLUMETRICTEXTURE_WOOD, &CCGWorkView::On3dVolumetricWood)
ON_UPDATE_COMMAND_UI(ID_3DVOLUMETRICTEXTURE_WOOD, &CCGWorkView::OnUpdate3dVolumetricWood)
ON_COMMAND(ID_3DVOLUMETRICTEXTURE_MARBLE, &CCGWorkView::On3dVolumetricMarle)
ON_UPDATE_COMMAND_UI(ID_3DVOLUMETRICTEXTURE_MARBLE, &CCGWorkView::OnUpdate3dVolumetricMarble)




ON_COMMAND(ID_LIGHT_MATERIAL, OnMaterialDlg)

ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()

//}}AFX_MSG_MAP
ON_WM_TIMER()
END_MESSAGE_MAP()


// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
	GLUquadric* quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluCylinder(quad, radius, 0.0, height, 20, 20);
	gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView()
{
	m_draw_poly_normals = false;
	m_draw_vertex_normals = false;
	m_draw_bounding_box = false;
	m_uniform_color = false;
	m_draw_poly_normals_from = false; //poly normals form file
	m_draw_poly_normals_not_from = false; // poly normals not from file
	m_draw_vertex_normals_from = false; //vertex normals from file
	m_draw_vertex_normals_not_from = false; // vertex normal nor from file
	m_is_object_space_transform = 1;//default to transform relative to object center and not to 0,0
	m_flip_normals = 0;
	m_back_ground_image_on = false;
	m_back_ground_image_stretch = true;
	m_draw_silhouettes = 0;
	m_render_to_screen = true;
	// Set default values
	m_nAxis = ID_AXIS_X;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	m_back_ground_image_was_set = false;

	m_nLightShading = ID_LIGHT_SHADING_FLAT;
	m_solid_rendering = true;
	m_lMaterialAmbient = 0.6;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].enabled = true;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;
	m_isDragging = false;
	prev_start = CPoint(0, 0);
	m_do_back_face_culling = false;

	m_anti_aliasing_5x5 = false;
	m_anti_aliasing_None = true;
	m_anti_aliasing_Box = false;
	m_anti_aliasing_Triangle = false;
	m_anti_aliasing_Sinc = false;
	m_anti_aliasing_Gaussian = false;
	m_fog_effects_on = false;
	m_3DVOLUMETRICTEXTURE_MARBLE = false;
	m_3DVOLUMETRICTEXTURE_WOOD = false;
	m_3DVOLUMETRICTEXTURE_NONE = true;


	/*
	ID_3DVOLUMETRICTEXTURE_MARBLE
	ID_3DVOLUMETRICTEXTURE_WOOD
	ID_3DVOLUMETRICTEXTURE_NONE
	*/
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}



int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	return 0;
}


// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	SetTimer(1, 1, NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers

void CCGWorkView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy) {
		return; // Prevent resizing to zero or negative dimensions
	}

	// Save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// Compute the aspect ratio
	m_AspectRatio = (GLdouble)m_WindowWidth / (GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap); // Delete the old bitmap
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom); // Create a new bitmap
	m_pDbDC->SelectObject(m_pDbBitMap); // Attach the new bitmap to the device context

	// Apply the transformation to center the object to the screen
	Matrix4 centerMatrix = getMatrixToCenterObject();
	scene.applyTransform(centerMatrix);

	// Trigger a redraw
	Invalidate();
}



BOOL CCGWorkView::SetupViewingFrustum(void)
{
	return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}



BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC)
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.
	return true;
}

// Helper function to draw a single line
void CCGWorkView::DrawLineHelper(Point* oBuffer, size_t width, size_t height, const Vector4& start, const Vector4& end, COLORREF color) {
	LineDrawer::DrawLine(
		oBuffer,
		width,
		height,
		start,
		end,
		color
	);
}



void CCGWorkView::DrawPolygonEdgesAndVertexNormals(Point* oBuffer, size_t width, size_t height, Poly* poly, Vector4 cameraPosition, COLORREF edgeColor, COLORREF normalColor) {
	const std::vector<Vertex>& vertices = poly->getVertices();
	const size_t vertexCount = vertices.size();

	for (size_t i = 0; i < vertexCount; ++i) {
		const Vertex& start = vertices[i];
		const Vertex& end = vertices[(i + 1 < vertexCount) ? i + 1 : 0];

		// Draw polygon edge
		if (!m_do_back_face_culling || (!start.getNormalCalculated().isBackFacing(cameraPosition) && !end.getNormalCalculated().isBackFacing(cameraPosition))) {
			DrawLineHelper(oBuffer, width, height, start, end, edgeColor);
		}

		// Draw vertex normals from file
		if (m_draw_vertex_normals_from) {
			const Normal& normalFromFile = start.getNormalFromFile();
			DrawLineHelper(oBuffer, width, height, normalFromFile.start, normalFromFile.end, normalColor);
		}

		// Draw vertex normals calculated
		if (m_draw_vertex_normals_not_from) {
			const Normal& normalCalculated = start.getNormalCalculated();
			DrawLineHelper(oBuffer, width, height, normalCalculated.start, normalCalculated.end, normalColor);
		}
	}
}


void CCGWorkView::DrawPolygonNormal(Point* oBuffer, size_t width, size_t height, Poly* poly, COLORREF color) {
	// Draw polygon normals from file
	if (m_draw_poly_normals_from) {
		const Normal& normalFromFile = poly->getPolyNormalFromFile();
		DrawLineHelper(oBuffer, width, height, normalFromFile.start, normalFromFile.end, color);
	}

	// Draw calculated polygon normals
	if (m_draw_poly_normals_not_from) {
		const Normal& normalCalculated = poly->getPolyNormalCalculated();
		DrawLineHelper(oBuffer, width, height, normalCalculated.start, normalCalculated.end, color);
	}
}

void CCGWorkView::DrawBoundingBox(Point* oBuffer, size_t width, size_t height, const BoundingBox& bbox, COLORREF color) {
	const std::vector<Vector4>& corners = bbox.getCorners();

	std::vector<std::pair<int, int>> edges = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0}, // Top face
		{4, 5}, {5, 7}, {7, 6}, {6, 4}, // Bottom face
		{0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
	};

	for (const auto& edge : edges) {
		DrawLineHelper(oBuffer, width, height, corners[edge.first], corners[edge.second], color);
	}
}


COLORREF apply_fog(COLORREF objectColor, COLORREF fogColor, float pixel_z, float fogStart, float fogEnd) {
	// Clamp z to be within the fog range
	if (pixel_z < fogStart) {
		return objectColor;  // No fog if closer than fogStart
	}
	else if (pixel_z > fogEnd) {
		return fogColor;  // Full fog if farther than fogEnd
	}

	// Calculate fog factor (linear interpolation)
	float fogFactor = (pixel_z - fogStart) / (fogEnd - fogStart);

	// Reduce the fog effect for more moderate blending
	fogFactor = pow(fogFactor, 1.35f);  // Apply a power function to make the transition smoother

	// Blend object color with fog color
	BYTE r = (BYTE)((1 - fogFactor) * GetRValue(objectColor) + fogFactor * GetRValue(fogColor));
	BYTE g = (BYTE)((1 - fogFactor) * GetGValue(objectColor) + fogFactor * GetGValue(fogColor));
	BYTE b = (BYTE)((1 - fogFactor) * GetBValue(objectColor) + fogFactor * GetBValue(fogColor));

	return RGB(r, g, b);
}

Vector4 ComputeBarycentricCoords(const Vector4& P, const Vector4& A, const Vector4& B, const Vector4& C) {
	// Compute vectors using Vector4
	Vector4 v0 = B - A;
	Vector4 v1 = C - A;
	Vector4 v2 = P - A;

	// Compute dot products
	double d00 = v0.dot(v0);
	double d01 = v0.dot(v1);
	double d11 = v1.dot(v1);
	double d20 = v2.dot(v0);
	double d21 = v2.dot(v1);

	// Compute denominator
	double denom = d00 * d11 - d01 * d01;

	// Compute barycentric coordinates (u, v, w)
	double u = (d11 * d20 - d01 * d21) / denom;
	double v = (d00 * d21 - d01 * d20) / denom;
	double w = 1.0 - u - v;

	// Return as Vector4 (u, v, w are stored in x, y, z components)
	return Vector4(u, v, w, 0.0f); // The w component is unused
}

Vector4 GetInterpolatedNormal(const Vector4& pixelPos, const Poly* p) {
	const std::vector<Vertex>& vertices = p->getVertices();

	// Ensure we have a valid triangle
	if (vertices.size() < 3) return Vector4(0, 0, 0, 0);

	// Get vertex positions (we use Vector4 for positions)
	Vector4 v0 = vertices[0];
	Vector4 v1 = vertices[1];
	Vector4 v2 = vertices[2];

	// Get vertex normals (also using Vector4 for normals)
	Vector4 n0 = vertices[0].getNormalCalculated().end;
	Vector4 n1 = vertices[1].getNormalCalculated().end;
	Vector4 n2 = vertices[2].getNormalCalculated().end;

	// Compute Barycentric coordinates using Vector4
	Vector4 bary = ComputeBarycentricCoords(pixelPos, v0, v1, v2);

	// Interpolate normal using barycentric weights (same logic as before)
	Vector4 interpolatedNormal = (n0 * bary.x) + (n1 * bary.y) + (n2 * bary.z);

	// Normalize the result to ensure correct lighting
	return interpolatedNormal.normalize();
}


COLORREF ApplyPhongShading(const Poly* p, const Vector4& pixelPosition, Vector4 cameraPosition) {
	CCGWorkApp* cApp = (CCGWorkApp*)AfxGetApp();

	// Interpolated normal at this pixel (obtained from rasterization step)
	Vector4 interpolatedNormal = GetInterpolatedNormal(pixelPosition, p);
	interpolatedNormal = interpolatedNormal.normalize();

	// Compute view direction
	Vector4 viewDir = (cameraPosition - pixelPosition).normalize();

	// Base material color
	COLORREF baseColor = p->getColor();
	int baseR = GetRValue(baseColor);
	int baseG = GetGValue(baseColor);
	int baseB = GetBValue(baseColor);

	// Initialize final color with ambient lighting
	int pixelR = static_cast<int>(cApp->m_ambientLight.colorR * cApp->m_lMaterialAmbient * baseR / 255);
	int pixelG = static_cast<int>(cApp->m_ambientLight.colorG * cApp->m_lMaterialAmbient * baseG / 255);
	int pixelB = static_cast<int>(cApp->m_ambientLight.colorB * cApp->m_lMaterialAmbient * baseB / 255);

	// Loop through all enabled lights
	for (int i = LIGHT_ID_1; i < MAX_LIGHT; i++) {
		if (cApp->m_lights[i].enabled) {
			Vector4 dir(cApp->m_lights[i].dirX, cApp->m_lights[i].dirY, cApp->m_lights[i].dirZ);
			Vector4 pos(cApp->m_lights[i].posX, cApp->m_lights[i].posY, cApp->m_lights[i].posZ);

			// Determine light direction
			Vector4 lightDir = (cApp->m_lights[i].type == LIGHT_TYPE_DIRECTIONAL)
				? (dir.flip()).normalize()
				: (pos - pixelPosition).normalize();

			// Compute diffuse lighting
			double cosTheta = max(0.0, interpolatedNormal.dot(lightDir));
			pixelR += static_cast<int>(cApp->m_lights[i].colorR * cApp->m_lMaterialDiffuse * cosTheta);
			pixelG += static_cast<int>(cApp->m_lights[i].colorG * cApp->m_lMaterialDiffuse * cosTheta);
			pixelB += static_cast<int>(cApp->m_lights[i].colorB * cApp->m_lMaterialDiffuse * cosTheta);

			// Compute specular lighting
			Vector4 halfwayDir = (lightDir + viewDir).normalize();
			double specAngle = max(0.0, interpolatedNormal.dot(halfwayDir));
			double specular = cApp->m_lMaterialSpecular * pow(specAngle, cApp->m_lMaterialShininess);

			pixelR += static_cast<int>(cApp->m_lights[i].colorR * specular);
			pixelG += static_cast<int>(cApp->m_lights[i].colorG * specular);
			pixelB += static_cast<int>(cApp->m_lights[i].colorB * specular);
		}
	}

	// Clamp color values to avoid overflow
	pixelR = std::min(255, max(0, pixelR));
	pixelG = std::min(255, max(0, pixelG));
	pixelB = std::min(255, max(0, pixelB));

	return RGB(pixelR, pixelG, pixelB);
}



COLORREF ApplySpotlight(const Poly* p); // declaration only

COLORREF shade_polygon(const Poly* p, COLORREF baseColor, Vector4 cameraPosition, int Shading,int i,int j) {
	CCGWorkApp* cApp = (CCGWorkApp*)AfxGetApp();  // Access the application instance

	int baseB = GetBValue(baseColor);
	int baseG = GetGValue(baseColor);
	int baseR = GetRValue(baseColor);

	int r, g, b;  // Final RGB values
	bool affected = false;
	for (int j = LIGHT_ID_1; j < MAX_LIGHT; j++) {
		if (cApp->m_lights[j].enabled) {
			affected = true;
			if (cApp->m_lights->type == LIGHT_TYPE_SPOTLIGHT)
				return ApplySpotlight(p);
		}
	}
	if (!affected) {
		return baseColor;
	}
	if (Shading==ID_LIGHT_SHADING_FLAT) {
		// **Flat Shading** - Calculate color once for the entire polygon
		r = static_cast<int>(cApp->m_ambientLight.colorR * cApp->m_lMaterialAmbient * baseR / 255);
		g = static_cast<int>(cApp->m_ambientLight.colorG * cApp->m_lMaterialAmbient * baseG / 255);
		b = static_cast<int>(cApp->m_ambientLight.colorB * cApp->m_lMaterialAmbient * baseB / 255);

		Normal n = p->getNormal();
		Vector4 normal = n.getVector().normalize();
		Vector4 pointOnSurface = n.getVector();  // Assume normal origin is the point being shaded
		Vector4 viewDir = (cameraPosition - pointOnSurface).normalize();  // View direction

		// Iterate over lights
		for (int i = LIGHT_ID_1; i < MAX_LIGHT; i++) {
			if (cApp->m_lights[i].enabled) {
				Vector4 dir(cApp->m_lights[i].dirX, cApp->m_lights[i].dirY, cApp->m_lights[i].dirZ);
				Vector4 pos(cApp->m_lights[i].posX, cApp->m_lights[i].posY, cApp->m_lights[i].posZ);
				Vector4 lightDir;

				// Directional or point light handling
				if (cApp->m_lights[i].type == LIGHT_TYPE_DIRECTIONAL) {
					lightDir = (dir.flip()).normalize();  // Directional light
				}
				else {
					lightDir = (pos - pointOnSurface).normalize();  // Point light
				}

				double cosTheta = max(0.0, normal.dot(lightDir));  // Clamp cosine for diffuse

				// Diffuse lighting
				r += static_cast<int>(cApp->m_lights[i].colorR * cApp->m_lMaterialDiffuse * cosTheta);
				g += static_cast<int>(cApp->m_lights[i].colorG * cApp->m_lMaterialDiffuse * cosTheta);
				b += static_cast<int>(cApp->m_lights[i].colorB * cApp->m_lMaterialDiffuse * cosTheta);

				// Specular lighting (Blinn-Phong)
				Vector4 halfwayDir = (lightDir + viewDir).normalize();
				double specAngle = max(0.0, normal.dot(halfwayDir));
				double specular = cApp->m_lMaterialSpecular * pow(specAngle, cApp->m_lMaterialShininess);

				r += static_cast<int>(cApp->m_lights[i].colorR * specular);
				g += static_cast<int>(cApp->m_lights[i].colorG * specular);
				b += static_cast<int>(cApp->m_lights[i].colorB * specular);
			}
		}

	}
	else if (Shading == ID_LIGHT_SHADING_GOURAUD) {


		// **Gouraud Shading** - Calculate color for each vertex and interpolate
		r = g = b = 0;

		const std::vector<Vertex>& vertices = p->getVertices();
		int numVertices = vertices.size();

		for (const Vertex& vertex : vertices) {
			Vector4 normal = vertex.getNormalCalculated().end.normalize();
			Vector4 viewDir = (cameraPosition - vertex).normalize();

			// Ambient Component
			int vertexR = static_cast<int>(cApp->m_ambientLight.colorR * cApp->m_lMaterialAmbient * baseR / 255);
			int vertexG = static_cast<int>(cApp->m_ambientLight.colorG * cApp->m_lMaterialAmbient * baseG / 255);
			int vertexB = static_cast<int>(cApp->m_ambientLight.colorB * cApp->m_lMaterialAmbient * baseB / 255);

			for (int i = LIGHT_ID_1; i < MAX_LIGHT; i++) {
				if (cApp->m_lights[i].enabled) {
					Vector4 dir(cApp->m_lights[i].dirX, cApp->m_lights[i].dirY, cApp->m_lights[i].dirZ);
					Vector4 pos(cApp->m_lights[i].posX, cApp->m_lights[i].posY, cApp->m_lights[i].posZ);

					// Light direction
					Vector4 lightDir = (cApp->m_lights[i].type == LIGHT_TYPE_DIRECTIONAL)
						? (dir.flip()).normalize()
						: (pos - vertex).normalize();

					// Diffuse Component
					double cosTheta = max(0.0, normal.dot(lightDir));
					vertexR += static_cast<int>(cApp->m_lights[i].colorR * cApp->m_lMaterialDiffuse * cosTheta);
					vertexG += static_cast<int>(cApp->m_lights[i].colorG * cApp->m_lMaterialDiffuse * cosTheta);
					vertexB += static_cast<int>(cApp->m_lights[i].colorB * cApp->m_lMaterialDiffuse * cosTheta);

					// Specular Component
					Vector4 halfwayDir = (lightDir + viewDir).normalize();
					double specAngle = max(0.0, normal.dot(halfwayDir));
					double specular = cApp->m_lMaterialSpecular * pow(specAngle, cApp->m_lMaterialShininess);

					vertexR += static_cast<int>(cApp->m_lights[i].colorR * specular);
					vertexG += static_cast<int>(cApp->m_lights[i].colorG * specular);
					vertexB += static_cast<int>(cApp->m_lights[i].colorB * specular);
				}
			}

			// Clamp values to 0-255
			vertexR = std::min(255, max(0, vertexR));
			vertexG = std::min(255, max(0, vertexG));
			vertexB = std::min(255, max(0, vertexB));

			// Sum the vertex colors
			r += vertexR;
			g += vertexG;
			b += vertexB;
		}

		// Final color averaging
		r = std::min(255, max(0, r / numVertices));
		g = std::min(255, max(0, g / numVertices));
		b = std::min(255, max(0, b / numVertices));
	}

	else { //Phong Shading

	Vector4 pixelPosition(i, j, 0, 1);

	return ApplyPhongShading(p, pixelPosition, cameraPosition);
	}


	// Clamp final color values
	r = std::min(255, max(0, r));
	g = std::min(255, max(0, g));
	b = std::min(255, max(0, b));

	return RGB(r, g, b);
}

void saveCombinedBufferToPNG(
	Point* bgBuffer, Point* edgesBuffer, Point* normalsBuffer,
	Point* polygonsBuffer, Point* boundingBoxBuffer, size_t width,
	size_t height, const std::string& filename,
	Vector4 cameraPosition, COLORREF bg_color, int isFlatShading, bool use_fog,
	COLORREF fog_color, bool m_anti_aliasing_None, int kernelSize, const std::string& filterName) {

	PngWrapper png(filename.c_str(), width, height);

	if (!png.InitWritePng()) {
		std::cerr << "Failed to initialize PNG writing!" << std::endl;
		return;
	}

	// Temporary buffer for storing the combined image
	std::vector<Point> combinedBuffer(width * height);

	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			size_t index = y * width + x;

			COLORREF color = bg_color; // Default color

			// Render in the order: bgBuffer -> edgesBuffer -> normalsBuffer -> polygonsBuffer -> boundingBoxBuffer
			if (bgBuffer && bgBuffer[index].z < FLT_MAX) {
				color = bgBuffer[index].getColor();
			}

			if (edgesBuffer && edgesBuffer[index].getColor() != RGB(0, 0, 0)) {
				color = edgesBuffer[index].getColor();
			}

			if (normalsBuffer && normalsBuffer[index].getColor() != RGB(0, 0, 0)) {
				color = normalsBuffer[index].getColor();
			}

			if (polygonsBuffer && polygonsBuffer[index].z < FLT_MAX) {
				const Poly* p = polygonsBuffer[index].getPolygon();  // Pointer to Poly
				COLORREF baseColor = polygonsBuffer[index].getColor();
				color = shade_polygon(p, baseColor, cameraPosition, isFlatShading,x,y);
				if (use_fog) {
					float obj_z = polygonsBuffer[index].z;
					color = apply_fog(color, fog_color, obj_z, scene.minz, scene.maxz);  // Example fog range and color
				}
			}

			if (boundingBoxBuffer && boundingBoxBuffer[index].getColor() != RGB(0, 0, 0)) {
				color = boundingBoxBuffer[index].getColor();
			}

			// Store the combined color in the temporary buffer
			combinedBuffer[index].setColor(color);
		}
	}

	// Apply anti-aliasing if enabled
	if (!m_anti_aliasing_None) {
		applyAntiAliasingByName(combinedBuffer.data(), width, height, kernelSize, filterName);
	}

	// Write the (possibly anti-aliased) combined buffer to the PNG
	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			size_t index = y * width + x;
			COLORREF color = combinedBuffer[index].getColor();
			// Convert COLORREF to RGBA for PngWrapper
			unsigned int r = GetBValue(color);
			unsigned int g = GetGValue(color);
			unsigned int b = GetRValue(color);
			unsigned int pixelValue = (r << 24) | (g << 16) | (b << 8);
			// Set the pixel value in the PngWrapper
			png.SetValue(static_cast<unsigned int>(x), static_cast<unsigned int>(y), pixelValue);
		}
	}

	if (!png.WritePng()) {
		std::cerr << "Failed to write PNG file!" << std::endl;
	}
	else {
		std::cout << "Image saved to " << filename << std::endl;
	}
}


void StretchBackgroundToBuffer(Point* bgBuffer, int* bgImageData, int bgWidth, int bgHeight, size_t width, size_t height) {
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			int srcX = static_cast<int>((static_cast<float>(x) / width) * bgWidth);
			int srcY = static_cast<int>((static_cast<float>(y) / height) * bgHeight);
			// Get the color from the background image
			int color = bgImageData[srcY * bgWidth + srcX];
			// Keep it in RGB format
			bgBuffer[y * width + x] = Point(static_cast<float>(x), static_cast<float>(y), 1.0f, 1.0f, color);
		}
	}
}


void RepeatBackgroundToBuffer(Point* bgBuffer, int* bgImageData, int bgWidth, int bgHeight, size_t width, size_t height) {
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			int srcX = x % bgWidth;
			int srcY = y % bgHeight;

			// Get the color from the background image
			int color = bgImageData[srcY * bgWidth + srcX];

			// Keep it in RGB format
			bgBuffer[y * width + x] = Point(static_cast<float>(x), static_cast<float>(y), 1.0f, 1.0f, color);
		}
	}
}

int ind = 0;

void CCGWorkView::renderToBitmap(Point* bgBuffer, Point* edgesBuffer,
	Point* normalsBuffer, Point* polygonsBuffer,
	Point* boundingBoxBuffer,
	int width, int height, CDC* pDC, COLORREF bg_color,
	Vector4 cameraPosition,
	COLORREF fog_color, bool use_fog,
	int kernelSize, const std::string& filterName) {
	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // Negative for top-down DIB
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;   // 32-bit color depth
	bmi.bmiHeader.biCompression = BI_RGB;

	void* pBits = nullptr;
	HBITMAP hDIB = CreateDIBSection(pDC->GetSafeHdc(), &bmi, DIB_RGB_COLORS, &pBits, nullptr, 0);
	if (!hDIB) {
		std::cerr << "Failed to create DIBSection!" << std::endl;
		return;
	}

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC.GetSafeHdc(), hDIB);

	COLORREF* pixels = static_cast<COLORREF*>(pBits);

	// Temporary buffer to store the rendered image for anti-aliasing
	std::vector<Point> renderedBuffer(width * height);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			size_t index = y * width + x;
			COLORREF color = bg_color; // Default background color

			// Render buffers in the correct order of priority
			if (bgBuffer && bgBuffer[index].z < FLT_MAX) {
				color = bgBuffer[index].getColor();
			}

			if (edgesBuffer && edgesBuffer[index].getColor() != RGB(0, 0, 0)) {
				color = edgesBuffer[index].getColor();
			}
			if (polygonsBuffer && polygonsBuffer[index].z < FLT_MAX) { // Polygons take priority

				const Poly* p = polygonsBuffer[index].getPolygon();  // Pointer to Poly
				if (p == NULL) continue;
				color = polygonsBuffer[index].getColor();
				color = shade_polygon(p, color, cameraPosition, m_nLightShading ,x,y);
				// Apply fog based on the distance z
				if (use_fog) {
					float obj_z = polygonsBuffer[index].z;
					color = apply_fog(color, fog_color, obj_z, scene.minz, scene.maxz);  // Example fog range and color
				}
			}
			if (normalsBuffer && normalsBuffer[index].getColor() != RGB(0, 0, 0)) {
				color = normalsBuffer[index].getColor();
			}

			if (boundingBoxBuffer && boundingBoxBuffer[index].getColor() != RGB(0, 0, 0)) {
				color = boundingBoxBuffer[index].getColor();
			}

			// Assign the color to the pixels and store it in the rendered buffer
			pixels[index] = color;

			// Populate the rendered buffer for anti-aliasing
			renderedBuffer[index].setColor(color);
			renderedBuffer[index].z = 0; // Set depth to a neutral value since it's not used for display
		}
	}

	// Apply anti-aliasing on the rendered buffer
	if (!m_anti_aliasing_None) {
		applyAntiAliasingByName(renderedBuffer.data(), width, height, kernelSize, filterName);

		// Copy anti-aliased data back to the pixels buffer
		for (int i = 0; i < width * height; ++i) {
			pixels[i] = renderedBuffer[i].getColor();
		}
	}

	// Render the bitmap to the device context
	pDC->BitBlt(0, 0, width, height, &memDC, 0, 0, SRCCOPY);

	SelectObject(memDC.GetSafeHdc(), hOldBitmap);
	DeleteObject(hDIB);
	memDC.DeleteDC();
}


void threeDmarble() {

}
void threeDwood() {

}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////
void CCGWorkView::OnDraw(CDC* pDC) {
	// Get the mouse position
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);  // Convert to client coordinates
	CString str;
	// Log the mouse position to the status bar
	str.Format(_T("Mouse Position: X = %d, Y = %d"), point.x, point.y);
	//STATUS_BAR_TEXT(str);  // Assuming STATUS_BAR_TEXT sets the status bar text

	//str.Format(_T("onDraw on the %d time   \n"),ind++ );
	//STATUS_BAR_TEXT(str);


	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	CRect r;
	GetClientRect(&r);

	// Screen dimensions
	const int width = r.Width();
	const int height = r.Height();

	// Initialize buffers

	Point* bgBuffer = nullptr;
	Point* edgesBuffer = nullptr;
	Point* normalsBuffer = nullptr;
	Point* polygonsBuffer = nullptr;
	Point* boundingBoxBuffer = nullptr;
	Vector4 cameraPosition(width / 2.0, height / 2.0, -500.0); // Z position is set to -500 for perspective
	COLORREF white = RGB(255, 255, 255);
	COLORREF green = RGB(0, 255, 0);

	// Render background
	if (m_back_ground_image_on) {
		bgBuffer = initZBuffer(width, height);
		int* bgImageData = nullptr;
		int bgWidth = 0, bgHeight = 0;

		if (scene.getBackgroundImage(bgImageData, bgWidth, bgHeight) && m_back_ground_image_was_set) {
			if (m_back_ground_image_stretch) {
				StretchBackgroundToBuffer(bgBuffer, bgImageData, bgWidth, bgHeight, width, height);
			}
			else {
				RepeatBackgroundToBuffer(bgBuffer, bgImageData, bgWidth, bgHeight, width, height);
			}
		}
	}

	// Render edges
	if (m_draw_poly_normals_from || m_draw_vertex_normals_from || m_draw_vertex_normals_not_from) {
		edgesBuffer = initZBuffer(width, height);
	}

	// Render normals
	if (m_draw_poly_normals_from || m_draw_vertex_normals_from) {
		normalsBuffer = initZBuffer(width, height);
	}

	// Render polygons
	if (m_solid_rendering) {
		polygonsBuffer = initZBuffer(width, height);
	}

	// Render bounding box
	if (m_draw_bounding_box) {
		boundingBoxBuffer = initZBuffer(width, height);
	}

	

	// Process polygons
	for (Poly* poly : *scene.getPolygons()) {
		if (m_solid_rendering && polygonsBuffer) {
			renderPolygon(polygonsBuffer, width, height, *poly, cameraPosition, m_do_back_face_culling, m_3DVOLUMETRICTEXTURE_MARBLE, m_3DVOLUMETRICTEXTURE_WOOD);
		}
		if (edgesBuffer) {
			DrawPolygonEdgesAndVertexNormals(edgesBuffer, width, height, poly, cameraPosition, white, pApp->vertex_normals_color);
		}
		if (normalsBuffer) {
			DrawPolygonNormal(normalsBuffer, width, height, poly, pApp->poly_normals_color);
		}
	}


	std::string filterName = "None";
	int kernelSize = m_anti_aliasing_5x5 ? 5 : 3;
	// Apply anti-aliasing after rendering
	if (polygonsBuffer && !m_anti_aliasing_None) {

		if (m_anti_aliasing_Box) {
			filterName = "Box";
		}
		else if (m_anti_aliasing_Triangle) {
			filterName = "Triangle";
		}
		else if (m_anti_aliasing_Gaussian) {
			filterName = "Gaussian";
		}
		else if (m_anti_aliasing_Sinc) {
			filterName = "Sinc";
		}
	}


	//////////////// perspective code goes here
///*
	if (m_nView == ID_VIEW_PERSPECTIVE) {
		PerspectiveView(polygonsBuffer, width, height);
		//PerspectiveView(edgesBuffer, width, height);
	}
	//*/
	// Draw bounding box if enabled
	if (scene.hasBoundingBox && m_draw_bounding_box && boundingBoxBuffer) {
		DrawBoundingBox(boundingBoxBuffer, width, height, scene.getBoundingBox(), green);
	}

	//if apply anti aliasing, do it once, and to file!
	if (!m_anti_aliasing_None) {
		saveCombinedBufferToPNG(bgBuffer, edgesBuffer, normalsBuffer, polygonsBuffer, boundingBoxBuffer,
			width, height, "..\\..\\output_image.png", cameraPosition,
			pApp->Background_color, m_nLightShading ,
			m_fog_effects_on, pApp->fog_color, m_anti_aliasing_None, kernelSize, filterName);
		m_render_to_screen = true;
		m_anti_aliasing_None = true;
		m_anti_aliasing_Box = false;
		m_anti_aliasing_Triangle = false;
		m_anti_aliasing_Gaussian = false;
		m_anti_aliasing_Sinc = false;
	}
	else {
		// Render to screen or save to file
		if (m_render_to_screen) {
			renderToBitmap(bgBuffer, edgesBuffer, normalsBuffer, polygonsBuffer, boundingBoxBuffer,
				width, height, pDC, pApp->Background_color, cameraPosition, pApp->fog_color,
				m_fog_effects_on, kernelSize, filterName);
		}
		else {
			m_render_to_screen = true;
			saveCombinedBufferToPNG(bgBuffer, edgesBuffer, normalsBuffer, polygonsBuffer, boundingBoxBuffer,
				width, height, "..\\..\\combined_output.png", cameraPosition,
				pApp->Background_color, m_nLightShading == ID_LIGHT_SHADING_FLAT,
				m_fog_effects_on, pApp->fog_color, m_anti_aliasing_None, kernelSize, filterName);
		}
	}


	// Cleanup buffers
	if (bgBuffer) freeZBuffer(bgBuffer);
	if (edgesBuffer) freeZBuffer(edgesBuffer);
	if (normalsBuffer) freeZBuffer(normalsBuffer);
	if (polygonsBuffer) freeZBuffer(polygonsBuffer);
	if (boundingBoxBuffer) freeZBuffer(boundingBoxBuffer);
}






/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...
	return;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy()
{
	CView::OnDestroy();

	// delete the DC
	if (m_pDC) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}
}





Matrix4 CCGWorkView::getMatrixToCenterObject() {
	// Calculate bounding box and determine initial transformation
	scene.calculateBoundingBox();

	const BoundingBox& bbox = scene.getBoundingBox();
	std::vector<Vector4> corners = bbox.getCorners(); // Get all corners of the bounding box

	// Calculate min and max using appropriate corners
	const Vector4& min = corners[5]; // BLB: Bottom-Left-Backward corner
	const Vector4& max = corners[2]; // TRF: Top-Right-Forward corner

	// Compute the center of the bounding box
	const Vector4 center = Vector4(
		(min.x + max.x) / 2.0,
		(min.y + max.y) / 2.0,
		(min.z + max.z) / 2.0,
		1.0 // Maintain consistent w for homogeneous coordinates
	);

	// Get the client rectangle
	CRect r;
	GetClientRect(&r);

	// Screen dimensions
	const double screenWidth = static_cast<double>(r.Width());
	const double screenHeight = static_cast<double>(r.Height());

	// Margin factor for initial load (centered on the window)
	const double marginFactor = 0.47;

	// Scene dimensions
	const double sceneWidth = max.x - min.x;
	const double sceneHeight = max.y - min.y;


	const double targetWidth = screenWidth * (1.0 - marginFactor);
	const double targetHeight = screenHeight * (1.0 - marginFactor);

	const double scaleX = sceneWidth > 1e-6 ? targetWidth / sceneWidth : 1.0;
	const double scaleY = sceneHeight > 1e-6 ? targetHeight / sceneHeight : 1.0;

	const double sceneScale = (scaleX < scaleY) ? scaleX : scaleY;

	Matrix4 t; // Starts as the identity matrix

	const Matrix4 translateToOrigin = Matrix4::translate(-center.x, -center.y, -center.z);
	const Matrix4 rotate = Matrix4::rotateX(180);
	const Matrix4 scaling = Matrix4::scale(sceneScale, sceneScale, sceneScale);
	const Matrix4 translateToScreen = Matrix4::translate(screenWidth / 2.0, screenHeight / 2.0, 0.0);

	// Translate to origin (center the scene)
	t = t * translateToScreen;

	t = t * rotate;
	// Scale the scene to fit within the target screen area
	t = t * scaling;
	// Translate to the screen center
	t = t * translateToOrigin;
	return t;
}

void CCGWorkView::OnFileLoadBackGroundImage() {
	// Open a file dialog to select the PNG file
	m_back_ground_image_was_set = false;
	TCHAR szFilters[] = _T("Background Image files (*.PNG)|*.PNG|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("PNG"), _T("*.PNG"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	if (dlg.DoModal() != IDOK) {
		return; // User canceled or no file selected
	}

	CString filePath = dlg.GetPathName(); // Get the selected file's path
#ifdef UNICODE
	size_t convertedChars = 0;
	char charPath[MAX_PATH];
	wcstombs_s(&convertedChars, charPath, MAX_PATH, filePath, _TRUNCATE);
#else
	const char* charPath = filePath.GetString();
#endif

	// Load the PNG image
	PngWrapper pngImage(charPath);
	if (!pngImage.ReadPng()) {
		AfxMessageBox(_T("Failed to load PNG image."));
		return;
	}

	int numChannels = pngImage.GetNumChannels();
	// Get image dimensions
	int width = pngImage.GetWidth();
	int height = pngImage.GetHeight();

	// Validate dimensions
	if (width <= 0 || height <= 0) {
		AfxMessageBox(_T("Invalid image dimensions."));
		return;
	}

	// Create an array to store the pixel data
	std::vector<int> imageData(width * height);

	// Fill the array with pixel data, flipping vertically
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// Get the pixel value
			int pixel = pngImage.GetValue(x, y);

			if (numChannels == 4) {
				// Extract RGBA components, ignoring alpha
				unsigned int r = (pixel >> 24) & 0xFF; // Extract Red
				unsigned int g = (pixel >> 16) & 0xFF; // Extract Green
				unsigned int b = (pixel >> 8) & 0xFF;  // Extract Blue

				// Repack into RGB format (ignoring alpha)
				imageData[y * width + x] = (r << 16) | (g << 8) | b; // RGB format
			}
			else if (numChannels == 3) {
				// If no alpha channel, process as RGB
				unsigned int r = (pixel >> 16) & 0xFF;
				unsigned int g = (pixel >> 8) & 0xFF;
				unsigned int b = pixel & 0xFF;

				// Repack into RGB format
				imageData[y * width + x] = (r << 16) | (g << 8) | b;
			}
		}
	}

	// Set the image as the background using scene.setBackgroundImage
	if (!scene.setBackgroundImage(imageData.data(), width, height)) {
		AfxMessageBox(_T("Failed to set background image."));
		return;
	}

	AfxMessageBox(_T("Background image loaded successfully!"));
	m_back_ground_image_was_set = true;
	}


#include "CTessellationDialog.h"
#include "perlin.h"
void CCGWorkView::OnFileLoad() {
	initializePermutation();

	TCHAR szFilters[] = _T("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) { // Show the file load dialog
		m_strItdFileName = dlg.GetPathName(); // Full path and filename
		scene.clear(); // Clear the existing scene data

		// Load and process the IRIT file with the selected tessellation level
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);

		// Calculate bounding box and determine initial transformation
		Matrix4 t = getMatrixToCenterObject();
		scene.calculateVertexNormals();
		scene.applyTransform(t);
		double scene_min_z = scene.minz;
		Matrix4 zBack = Matrix4::translate(0, 0, -scene.minz);
		scene.applyTransform(zBack);
		scene.calculateVertexNormals();

		scene.updateIsFirstDraw(false);

		Invalidate(); // Trigger WM_PAINT for redraw
	}

}






// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic()
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective()
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	//PerspectiveTransformPoly();
	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}




// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate()
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate()
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale()
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX()
{
	m_nAxis = ID_AXIS_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_X);
}


void CCGWorkView::OnAxisY()
{
	m_nAxis = ID_AXIS_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Y);
}


void CCGWorkView::OnAxisZ()
{
	m_nAxis = ID_AXIS_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAxis == ID_AXIS_Z);
}





// OPTIONS HANDLERS ///////////////////////////////////////////




// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat()
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}


void CCGWorkView::OnLightShadingGouraud()
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}


void CCGWorkView::OnLightShadingPhong()
{
	m_nLightShading = ID_SHADING_PHONG;
}

void CCGWorkView::OnUpdateLightShadingPhong(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_SHADING_PHONG);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void lightsDlgThread()
{

	CLightDialog dlg;
	CCGWorkView* cApp = (CCGWorkView*)AfxGetApp();
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
	{
		dlg.SetDialogData((LightID)id, pApp->m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT, pApp->m_ambientLight);

	if (dlg.DoModal() == IDOK)
	{
		for (int id = LIGHT_ID_1; id < MAX_LIGHT; id++)
		{
			pApp->m_lights[id] = dlg.GetDialogData((LightID)id);
		}
		pApp->m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
	}
	cApp->Invalidate();
}

void CCGWorkView::OnLightConstants()
{
	std::thread dialogThread(lightsDlgThread);
	dialogThread.detach();
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
	if (nIDEvent == 1)
		Invalidate();
}

/////////////////////////////////////////////////////////////////////////////////////////
void PerspectiveParametersThread()
{
	// Create and open the dialog
	PerspectiveDlg paramDlg;
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	// Initialize default values
	paramDlg.d = pApp->d;                   // Camera distance
	paramDlg.fovy = pApp->fovy;             // Vertical field of view
	paramDlg.aspectRatio = pApp->aspectRatio; // Aspect ratio
	paramDlg.nearPlane = pApp->nearPlane;   // Near clipping plane
	paramDlg.farPlane = pApp->farPlane;     // Far clipping plane

	if (paramDlg.DoModal() == IDOK)
	{
		// Retrieve updated values after the dialog is closed
		pApp->d = paramDlg.d;
		pApp->fovy = paramDlg.fovy;
		pApp->aspectRatio = paramDlg.aspectRatio;
		pApp->nearPlane = paramDlg.nearPlane;
		pApp->farPlane = paramDlg.farPlane;

		// Apply the parameters if needed (e.g., to update a perspective matrix)
	}
}

void CCGWorkView::OnPerspectiveParameters()
{
	std::thread dialogThread(PerspectiveParametersThread);
	dialogThread.detach();
}

void MouseDlgThread() {
	// TODO: Add your command handler code here
	CDialog* dlg = new MouseSensitivityDlg();
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	if (dlg->DoModal() == IDOK)
	{
		//	t_slider_value = dlg.TranslationSensitivity;

			// Handle OK click if needed
	}

}

void CCGWorkView::OnOptionsMousesensitivity()
{
	std::thread dialogThread(MouseDlgThread);
	dialogThread.detach();


}


void CCGWorkView::OnBoundingBox()
{
	//scene.hasBoundingBox = !scene.hasBoundingBox;
	m_draw_bounding_box = !m_draw_bounding_box;
	Invalidate();

}
void CCGWorkView::OnUpdateBoundingBox(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_draw_bounding_box);
}


void CCGWorkView::OnVertexNormal()
{
	//scene.hasBoundingBox = !scene.hasBoundingBox;
	m_draw_vertex_normals = !m_draw_vertex_normals;
	Invalidate();

}
void CCGWorkView::OnUpdateVertexNormal(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_draw_vertex_normals);
}

void CCGWorkView::OnPolyNormal()
{
	//scene.hasBoundingBox = !scene.hasBoundingBox;
	m_draw_poly_normals = !m_draw_poly_normals;
	Invalidate();

}

void CCGWorkView::OnUpdatePolyNormal(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_draw_poly_normals);
}

void CCGWorkView::OnPolyNormalsNotFrom()
{
	m_draw_poly_normals_not_from = !m_draw_poly_normals_not_from;
	Invalidate();
}

void CCGWorkView::OnUpdatePolyNormalsNotFrom(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_draw_poly_normals_not_from);
}

void CCGWorkView::OnPolyNormalsFrom()
{
	m_draw_poly_normals_from = !m_draw_poly_normals_from;
	Invalidate();
}

void CCGWorkView::OnUpdatePolyNormalsFrom(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_draw_poly_normals_from);
}

void CCGWorkView::OnVertexNormalsFrom()
{
	m_draw_vertex_normals_from = !m_draw_vertex_normals_from;
	Invalidate();
}

void CCGWorkView::OnUpdateVertexNormalsFrom(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_draw_vertex_normals_from);
}

void CCGWorkView::OnVertexNormalsNotFrom()
{
	m_draw_vertex_normals_not_from = !m_draw_vertex_normals_not_from;
	Invalidate();
}

void CCGWorkView::OnUpdateVertexNormalsNotFrom(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_draw_vertex_normals_not_from);
}

/////////////////////////////// MOUSE CONTROL //////////////////////////////////////////////

void CCGWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// Begin dragging
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	m_isDragging = true;
	SetCapture();  // Capture mouse events globally for this window

	//pApp->start = point;
	// Log or process the starting point
	TRACE(_T("Mouse Down at: X=%d, Y=%d\n"), point.x, point.y);

	prev_start = point;

	CView::OnLButtonDown(nFlags, point);
}

void CCGWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_isDragging)
	{
		// Handle dragging logic (e.g., drawing, updating UI, etc.)
		//TRACE(_T("Mouse Move at: X=%d, Y=%d\n"), point.x, point.y);


		// If needed, convert to screen coordinates
		CPoint screenPoint = point;
		ClientToScreen(&screenPoint);
		//TRACE(_T("Mouse Move (Screen): X=%d, Y=%d\n"), screenPoint.x, screenPoint.y);


		//CString str;
		//str.Format(_T("Mouse Move (Screen): X=%d, Y=%d\n"), screenPoint.x, screenPoint.y);
		//STATUS_BAR_TEXT(str);

		int diffrence = prev_start.x - point.x;
		prev_start = point;
		//CString str;
		//str.Format(_T("raw diff= %d calculated diff %d\n"), diffrence, diffrence / MOUSE_FACTOR);
		//STATUS_BAR_TEXT(str);
		diffrence = diffrence / MOUSE_FACTOR;

		MapMouseMovement(diffrence);


	}

	//CString str;
	//str.Format(_T("mouse position = ( %d , %d )\n"), point.x, point.y);
	//STATUS_BAR_TEXT(str);

	CView::OnMouseMove(nFlags, point);
}

void CCGWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_isDragging)
	{
		CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
		// End dragging
		m_isDragging = false;
		ReleaseCapture();  // Release mouse capture

		//pApp->end = point;
		//ApplyXRotation();
		//Invalidate();

		// Log or process the final point
		TRACE(_T("Mouse Up at: X=%d, Y=%d\n"), point.x, point.y);
		CString str;
		str.Format(_T("mouse bn up"));
		//STATUS_BAR_TEXT(str);
	}

	CView::OnLButtonUp(nFlags, point);
}


double validateFactor(double factor) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	if (factor == 0) {
		return 1;
	}
	else if (factor >= 0) {
		return 1.2 * pApp->s_slider_value;
	}
	else {
		return 0.81 / pApp->s_slider_value;;
	}
	/*
	const double threshold = 0.01;
	if (std::abs(factor) < threshold) {
		// Return the threshold value with the same sign as the original factor
		return (factor < 0) ? -threshold : threshold;
	}
	return factor; // Return the original factor if it's above the threshold
	*/
}

////transformation matrices
Matrix4 CreateCenteredRotationMatrix(const Matrix4& rotationMatrix) {
	Vector4 center = scene.getObjectCenter();

	// Create translation matrices
	Matrix4 translateToOrigin = Matrix4::translate(-center.x, -center.y, -center.z);
	Matrix4 translateBack = Matrix4::translate(center.x, center.y, center.z);

	// Combine transformations: Translate to origin -> Rotate -> Translate back
	return translateBack * rotationMatrix * translateToOrigin;
}



Matrix4 CreateCenteredScalingMatrix(double scaleX, double scaleY, double scaleZ) {
	Vector4 center = scene.getObjectCenter();
	// Create translation matrices
	Matrix4 translateToOrigin = Matrix4::translate(-center.x, -center.y, -center.z);

	// Scaling matrix
	Matrix4 scaling = Matrix4::scale(scaleX, scaleY, scaleZ);

	// Translation back to the center
	Matrix4 translateBack = Matrix4::translate(center.x, center.y, center.z);

	// Combine transformations: Translate to origin -> Scale -> Translate back
	return translateBack * scaling * translateToOrigin;
}


// Apply rotation around the X-axis
void CCGWorkView::ApplyXRotation(int d) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	// Create the rotation matrix


	Matrix4 r = Matrix4::rotateX(d);

	if (m_is_object_space_transform) {
		r = CreateCenteredRotationMatrix(r);
	}
	// Create the centered rotation matrix

	// Apply the transformation
	ApplyTransformation(r);

	// Update the status bar
	CString str;
	str.Format(_T("X-Rotation: deg = %d, sens = %d"), d, pApp->r_slider_value);
	STATUS_BAR_TEXT(str);
}

// Apply rotation around the Y-axis
void CCGWorkView::ApplyYRotation(int d) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	// Create the rotation matrix
	Matrix4 r = Matrix4::rotateY(d);

	if (m_is_object_space_transform) {
		r = CreateCenteredRotationMatrix(r);
	}
	// Create the centered rotation matrix

	// Apply the transformation
	ApplyTransformation(r);

	// Update the status bar
	CString str;
	str.Format(_T("Y-Rotation: deg = %d, sens = %d"), d, pApp->r_slider_value);
	STATUS_BAR_TEXT(str);
}

// Apply rotation around the Z-axis
void CCGWorkView::ApplyZRotation(int d) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	// Create the rotation matrix
	Matrix4 r = Matrix4::rotateZ(d);

	// Create the centered rotation matrix
	if (m_is_object_space_transform) {
		r = CreateCenteredRotationMatrix(r);
	}
	// Apply the transformation
	ApplyTransformation(r);

	// Update the status bar
	CString str;
	str.Format(_T("Z-Rotation: deg = %d, sens = %d"), d, pApp->r_slider_value);
	STATUS_BAR_TEXT(str);
}


void CCGWorkView::ApplyXTranslation(int d) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	Matrix4 transformation = Matrix4::translate(d, 0, 0);

	ApplyTransformation(transformation);

	// Update status bar
	CString str;
	str.Format(_T("Translated along X: %d"), d);
	STATUS_BAR_TEXT(str);
}

void CCGWorkView::ApplyYTranslation(int d) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	Matrix4 transformation = Matrix4::translate(0, d, 0);

	ApplyTransformation(transformation);

	// Update status bar
	CString str;
	str.Format(_T("Translated along Y: %d"), d);
	STATUS_BAR_TEXT(str);
}

void CCGWorkView::ApplyZTranslation(int d) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	Matrix4 r = Matrix4::translate(0, 0, d);
	double f = validateFactor(d);
	Matrix4 q = CreateCenteredScalingMatrix(f, f, f);
	Matrix4 res = r * q;
	ApplyTransformation(q);

	// Update status bar
	CString str;
	str.Format(_T("Translated along Z: %d"), d);
	STATUS_BAR_TEXT(str);
}




void CCGWorkView::ApplyXScale(double factor) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	const double  f = validateFactor(factor);
	Matrix4 scalingMatrix = CreateCenteredScalingMatrix(f, 1.0, 1.0);
	ApplyTransformation(scalingMatrix);

	// Update status bar
	CString str;
	str.Format(_T("Scaled along X by factor: %.3f"), f);
	STATUS_BAR_TEXT(str);
}

void CCGWorkView::ApplyYScale(double factor) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	const double f = validateFactor(factor);

	Matrix4 scalingMatrix = CreateCenteredScalingMatrix(1.0, f, 1.0);
	ApplyTransformation(scalingMatrix);

	// Update status bar
	CString str;
	str.Format(_T("Scaled along Y by factor: %.3f"), f);
	STATUS_BAR_TEXT(str);
}

void CCGWorkView::ApplyZScale(double factor) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	const double f = validateFactor(factor);

	Matrix4 scalingMatrix = CreateCenteredScalingMatrix(1.0, 1.0, f); // Use factor, not 1.0 / factor
	ApplyTransformation(scalingMatrix);

	// Update status bar
	CString str;
	str.Format(_T("Scaled along Z by factor: %.3f"), f);
	STATUS_BAR_TEXT(str);
}

void CCGWorkView::ApplyTransformation(Matrix4& t)
{
	scene.applyTransform(t); // Directly apply the transformation to the scene
	Invalidate(); // Trigger a redraw
}

void CCGWorkView::MapMouseMovement(int deg) {
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();

	// Scale deg based on the selected action
	if (m_nAction == ID_ACTION_ROTATE) {
		deg *= pApp->r_slider_value;
	}
	else if (m_nAction == ID_ACTION_TRANSLATE) {
		deg *= pApp->t_slider_value;
	}
	else if (m_nAction == ID_ACTION_SCALE) {
		deg *= pApp->s_slider_value;
	}
	else {
		return; // Invalid action, exit early
	}

	// Perform the transformation based on the selected axis
	switch (m_nAxis) {
	case ID_AXIS_X:
		if (m_nAction == ID_ACTION_ROTATE) ApplyXRotation(deg);
		else if (m_nAction == ID_ACTION_TRANSLATE) ApplyXTranslation(deg);
		else if (m_nAction == ID_ACTION_SCALE) ApplyXScale(deg);
		break;
	case ID_AXIS_Y:
		if (m_nAction == ID_ACTION_ROTATE) ApplyYRotation(deg);
		else if (m_nAction == ID_ACTION_TRANSLATE) ApplyYTranslation(deg);
		else if (m_nAction == ID_ACTION_SCALE) ApplyYScale(deg);
		break;
	case ID_AXIS_Z:
		if (m_nAction == ID_ACTION_ROTATE) ApplyZRotation(deg);
		else if (m_nAction == ID_ACTION_TRANSLATE) ApplyZTranslation(deg);
		else if (m_nAction == ID_ACTION_SCALE) ApplyZScale(deg);
		break;
	default:
		// Invalid axis, do nothing
		break;
	}

}
void CheckMenuItem(UINT menuID, bool checked) {
	CMenu* pMenu = AfxGetMainWnd()->GetMenu(); // Get the main menu
	if (pMenu) {
		pMenu->CheckMenuItem(menuID, checked ? MF_CHECKED | MF_BYCOMMAND : MF_UNCHECKED | MF_BYCOMMAND);
	}
}


void CCGWorkView::OnUpdateViewView(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_is_object_space_transform == 0);
}

void CCGWorkView::OnUpdateViewObject(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_is_object_space_transform == 1);
}

void CCGWorkView::OnViewView() {
	m_is_object_space_transform = 0; // Set flag to View
	CheckMenuItem(ID_VIEW_VIEW, true);
	CheckMenuItem(ID_VIEW_OBJECT, false);
}

void CCGWorkView::OnViewObject() {
	m_is_object_space_transform = 1; // Set flag to World
	CheckMenuItem(ID_VIEW_VIEW, false);
	CheckMenuItem(ID_VIEW_OBJECT, true);
}


void CCGWorkView::OnDrawSilhouettes() {
	m_draw_silhouettes = 1 - m_draw_silhouettes; // Set flag to View
	CheckMenuItem(ID_VIEW_DRAW_SILS, true);
}
void CCGWorkView::OnUpdateDrawSilhouettes(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_draw_silhouettes == 1);
}

void CCGWorkView::OnFlipNormals() {
	m_flip_normals = 1 - m_flip_normals;
	scene.flipNormals();

}
void CCGWorkView::OnUpdateFlipNormals(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_flip_normals == 1);
}

void CCGWorkView::OnRenderToFile() {
	m_render_to_screen = !m_render_to_screen;
}

void CCGWorkView::OnUpdateRenderToFile(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_render_to_screen == false);
}

void CCGWorkView::OnRenderToScreen() {
	m_render_to_screen = !m_render_to_screen;
}

void CCGWorkView::OnUpdateRenderToScreen(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_render_to_screen == true);
}

void CCGWorkView::OnBackFaceCulling() {
	m_do_back_face_culling = !m_do_back_face_culling;
}

void CCGWorkView::OnUpdateBackFaceCulling(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_do_back_face_culling == true);
}

void CCGWorkView::OnSolidRendering() {
	m_solid_rendering = !m_solid_rendering;
}

void CCGWorkView::OnUpdateSolidRendering(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_solid_rendering == true);
}

void CCGWorkView::OnBackGroundImageStretch() {
	m_back_ground_image_stretch = !m_back_ground_image_stretch;
}

void CCGWorkView::OnUpdateBackGroundImageStretch(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_back_ground_image_stretch == true);
}

void CCGWorkView::OnBackGroundImageOn() {
	m_back_ground_image_on = !m_back_ground_image_on;
}

void CCGWorkView::OnUpdateBackGroundImageOn(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_back_ground_image_on == true);
}


////////////polygon finess:


////////////polygon finess:
void CCGWorkView::OnUpdateOptionsPolygonFineness(CCmdUI* pCmdUI)
{
	// Enable the menu item
	pCmdUI->Enable(TRUE);
}


void PolygonFinenessThread()
{
	// Create and display the dialog
	PolygonFineness Dlg;
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();




	if (Dlg.DoModal() == IDOK)
	{
		// Retrieve updated values after the dialog is closed

		// Use the parameters (e.g., store or apply them)

	}
}


void CCGWorkView::OnOptionsPolygonFineness()
{
	std::thread dialogThread(PolygonFinenessThread);
	dialogThread.detach();
}
void MaterialDlgThread()
{
	CMaterialDlg dlg;
	CCGWorkView* cApp = (CCGWorkView*)AfxGetApp();
	CCGWorkApp* pApp = (CCGWorkApp*)AfxGetApp();
	//dlg.m_ambient = pApp->m_lMaterialAmbient;
	//dlg.m_diffuse = pApp->m_lMaterialDiffuse;
	//dlg.m_specular = pApp->m_lMaterialSpecular;
	//dlg.m_shininess = pApp->m_lMaterialShininess;

	if (dlg.DoModal() == IDOK)
	{
		pApp->m_lMaterialAmbient = dlg.m_ambient;
		pApp->m_lMaterialDiffuse = dlg.m_diffuse;
		pApp->m_lMaterialSpecular = dlg.m_specular;
		pApp->m_lMaterialShininess = dlg.m_shininess;
	}
	cApp->Invalidate();

}

void CCGWorkView::OnMaterialDlg()
{
	std::thread dialogThread(MaterialDlgThread);
	dialogThread.detach();
}


/*
ANTI ALIASING menu stuff
*/

// 5x5 Anti-aliasing
void CCGWorkView::OnAntiAliasing5x5() {
	// Enable 5x5 and disable others
	m_anti_aliasing_5x5 = !m_anti_aliasing_5x5;
	// Update menu items
}

void CCGWorkView::OnUpdateAntiAliasing5x5(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_anti_aliasing_5x5 == true);
}

// None Anti-aliasing
void CCGWorkView::OnAntiAliasingNone() {
	m_anti_aliasing_None = true;
	m_anti_aliasing_Box = false;
	m_anti_aliasing_Triangle = false;
	m_anti_aliasing_Sinc = false;
	m_anti_aliasing_Gaussian = false;

	// Update menu items
	CheckMenuItem(ID_ANTIALIASING_NONE, true);
	CheckMenuItem(ID_ANTIALIASING_BOX, false);
	CheckMenuItem(ID_ANTIALIASING_TRIANGLE, false);
	CheckMenuItem(ID_ANTIALIASING_SINC, false);
	CheckMenuItem(ID_ANTIALIASING_GAUSSIAN, false);
}

void CCGWorkView::OnUpdateAntiAliasingNone(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_anti_aliasing_None == true);
}

// Box Anti-aliasing
void CCGWorkView::OnAntiAliasingBox() {
	m_anti_aliasing_None = false;
	m_anti_aliasing_Box = true;
	m_anti_aliasing_Triangle = false;
	m_anti_aliasing_Sinc = false;
	m_anti_aliasing_Gaussian = false;

	// Update menu items
	CheckMenuItem(ID_ANTIALIASING_NONE, false);
	CheckMenuItem(ID_ANTIALIASING_BOX, true);
	CheckMenuItem(ID_ANTIALIASING_TRIANGLE, false);
	CheckMenuItem(ID_ANTIALIASING_SINC, false);
	CheckMenuItem(ID_ANTIALIASING_GAUSSIAN, false);
}

void CCGWorkView::OnUpdateAntiAliasingBox(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_anti_aliasing_Box == true);
}

// Triangle Anti-aliasing
void CCGWorkView::OnAntiAliasingTriangle() {
	m_anti_aliasing_None = false;
	m_anti_aliasing_Box = false;
	m_anti_aliasing_Triangle = true;
	m_anti_aliasing_Sinc = false;
	m_anti_aliasing_Gaussian = false;

	// Update menu items
	CheckMenuItem(ID_ANTIALIASING_NONE, false);
	CheckMenuItem(ID_ANTIALIASING_BOX, false);
	CheckMenuItem(ID_ANTIALIASING_TRIANGLE, true);
	CheckMenuItem(ID_ANTIALIASING_SINC, false);
	CheckMenuItem(ID_ANTIALIASING_GAUSSIAN, false);
}

void CCGWorkView::OnUpdateAntiAliasingTriangle(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_anti_aliasing_Triangle == true);
}

// Sinc Anti-aliasing
void CCGWorkView::OnAntiAliasingSinc() {
	m_anti_aliasing_None = false;
	m_anti_aliasing_Box = false;
	m_anti_aliasing_Triangle = false;
	m_anti_aliasing_Sinc = true;
	m_anti_aliasing_Gaussian = false;

	// Update menu items
	CheckMenuItem(ID_ANTIALIASING_NONE, false);
	CheckMenuItem(ID_ANTIALIASING_BOX, false);
	CheckMenuItem(ID_ANTIALIASING_TRIANGLE, false);
	CheckMenuItem(ID_ANTIALIASING_SINC, true);
	CheckMenuItem(ID_ANTIALIASING_GAUSSIAN, false);
}

void CCGWorkView::OnUpdateAntiAliasingSinc(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_anti_aliasing_Sinc == true);
}

// Gaussian Anti-aliasing
void CCGWorkView::OnAntiAliasingGaussian() {
	m_anti_aliasing_None = false;
	m_anti_aliasing_Box = false;
	m_anti_aliasing_Triangle = false;
	m_anti_aliasing_Sinc = false;
	m_anti_aliasing_Gaussian = true;

	// Update menu items
	CheckMenuItem(ID_ANTIALIASING_NONE, false);
	CheckMenuItem(ID_ANTIALIASING_BOX, false);
	CheckMenuItem(ID_ANTIALIASING_TRIANGLE, false);
	CheckMenuItem(ID_ANTIALIASING_SINC, false);
	CheckMenuItem(ID_ANTIALIASING_GAUSSIAN, true);
}

void CCGWorkView::OnUpdateAntiAliasingGaussian(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_anti_aliasing_Gaussian == true);
}

void CCGWorkView::OnFogEffectsOn() {
	m_fog_effects_on = !m_fog_effects_on;
}
void CCGWorkView::OnUpdateFogEffectsOn(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_fog_effects_on == true);
}

void CCGWorkView::On3dVolumetricNone() {
	m_3DVOLUMETRICTEXTURE_MARBLE = false;
	m_3DVOLUMETRICTEXTURE_WOOD = false;
	m_3DVOLUMETRICTEXTURE_NONE = true;
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_MARBLE, false);
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_WOOD, false);
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_NONE, true);
}

void CCGWorkView::OnUpdate3dVolumetricNone(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_3DVOLUMETRICTEXTURE_NONE == true);

}


void CCGWorkView::On3dVolumetricWood() {
	m_3DVOLUMETRICTEXTURE_MARBLE = false;
	m_3DVOLUMETRICTEXTURE_WOOD = true;
	m_3DVOLUMETRICTEXTURE_NONE = false;
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_MARBLE, false);
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_WOOD, true);
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_NONE, false);
}
void CCGWorkView::OnUpdate3dVolumetricWood(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_3DVOLUMETRICTEXTURE_WOOD == true);

}

void CCGWorkView::On3dVolumetricMarle() {
	m_3DVOLUMETRICTEXTURE_MARBLE = true;
	m_3DVOLUMETRICTEXTURE_WOOD = false;
	m_3DVOLUMETRICTEXTURE_NONE = false;
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_MARBLE, true);
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_WOOD, false);
	CheckMenuItem(ID_3DVOLUMETRICTEXTURE_NONE, false);
}
void CCGWorkView::OnUpdate3dVolumetricMarble(CCmdUI* pCmdUI) {
	pCmdUI->SetCheck(m_3DVOLUMETRICTEXTURE_MARBLE == true);

}

Vector4 CalculatePolygonCenter(const Poly* p) {
	Vector4 center(0.0f, 0.0f, 0.0f, 1.0f);
	std::vector<Vertex>  vertices = p->getVertices();

	int vertexCount = vertices.size();

	// Sum up all vertex coordinates
	for (const auto& vertex : vertices) {
		center.x += vertex.x;
		center.y += vertex.y;
		center.z += vertex.z;
	}

	// Divide by the number of vertices to get the average
	if (vertexCount > 0) {
		center.x /= vertexCount;
		center.y /= vertexCount;
		center.z /= vertexCount;
	}

	return center;
}

// Function to convert degrees to radians
constexpr float DegreesToRadians(float degrees) {
	return degrees * (static_cast<float>(M_PI) / 180.0f);
}

// Function to scale a COLORREF by an intensity factor
COLORREF ScaleColor(COLORREF color, float factor) {
	// Extract the red, green, and blue components
	int red = GetRValue(color);
	int green = GetGValue(color);
	int blue = GetBValue(color);

	// Scale each component and clamp to 0-255
	red = static_cast<int>(red * factor);
	green = static_cast<int>(green * factor);
	blue = static_cast<int>(blue * factor);

	red = std::min(max(red, 0), 255);   // Clamp red to [0, 255]
	green = std::min(max(green, 0), 255); // Clamp green to [0, 255]
	blue = std::min(max(blue, 0), 255);  // Clamp blue to [0, 255]

	// Return the scaled color
	return RGB(red, green, blue);
}
/*
COLORREF ApplySpotlight(const Poly* p) {

	CCGWorkApp* cApp = (CCGWorkApp*)AfxGetApp();  // Access the application instance
	//COLORREF   outColor = RGB(0, 0, 0);
	COLORREF outColor = p->getColor();
	Vector4 polygonCenter = CalculatePolygonCenter(p);
	Vector4 spotlightPosition(cApp->m_lights->posX, cApp->m_lights->posY, cApp->m_lights->posZ, 1.0f);
	Vector4  lightToSurface = (polygonCenter - spotlightPosition).normalize();
	Vector4  lightToSurfaceNeg(-lightToSurface.x, -lightToSurface.y, -lightToSurface.z, 1.0f);
	Vector4 spotlightDirection(cApp->m_lights->dirX, cApp->m_lights->dirY, cApp->m_lights->dirZ, 1.0f);
	float cosTheta = (spotlightDirection.normalize()).dot((lightToSurfaceNeg.normalize()));
	if (cosTheta < cos(DegreesToRadians(cApp->m_lights->spotlightCone))) {
		// Outside the spotlight cone
		return outColor;

	}
	// Spotlight attenuation factor
	float spotlightFactor = pow(cosTheta, cApp->m_lights->spotlightFactor);

	// Calculate diffuse lighting
	Normal n = p->getNormal();
	Vector4 polyNormal = n.getVector();
	float diffuseIntensity = max(0.0f, polyNormal.dot(lightToSurfaceNeg)) * spotlightFactor;

	// Combine ambient and diffuse components
	COLORREF lightColor = RGB(cApp->m_lights->colorR, cApp->m_lights->colorG, cApp->m_lights->colorB);
	COLORREF diffuseColor = ScaleColor(lightColor, diffuseIntensity);
	outColor = diffuseColor;
	outColor += diffuseColor;
	return outColor;
}
*/

COLORREF BlendColors(COLORREF baseColor, COLORREF blendColor) {
	int baseR = GetRValue(baseColor);
	int baseG = GetGValue(baseColor);
	int baseB = GetBValue(baseColor);

	int blendR = GetRValue(blendColor);
	int blendG = GetGValue(blendColor);
	int blendB = GetBValue(blendColor);

	// Sum colors and clamp to 255
	int finalR = std::min(baseR + blendR, 255);
	int finalG = std::min(baseG + blendG, 255);
	int finalB = std::min(baseB + blendB, 255);

	return RGB(finalR, finalG, finalB);
}

COLORREF ApplySpotlight(const Poly* p) {
	CCGWorkApp* cApp = (CCGWorkApp*)AfxGetApp();

	COLORREF outColor = p->getColor();
	Vector4 polygonCenter = CalculatePolygonCenter(p);
	Vector4 spotlightPosition(cApp->m_lights->posX, cApp->m_lights->posY, cApp->m_lights->posZ, 1.0f);

	// Light direction to polygon
	Vector4 lightToSurface = (polygonCenter - spotlightPosition).normalize();
	Vector4 lightToSurfaceNeg(-lightToSurface.x, -lightToSurface.y, -lightToSurface.z, 1.0f);

	// Spotlight direction (ensure it's normalized)
	Vector4 spotlightDirection(cApp->m_lights->dirX, cApp->m_lights->dirY, cApp->m_lights->dirZ, 1.0f);
	spotlightDirection = spotlightDirection.normalize();

	float cosTheta = spotlightDirection.dot(lightToSurfaceNeg);

	// Spotlight cone cutoff
	float spotlightConeRad = DegreesToRadians(cApp->m_lights->spotlightCone);
	if (cosTheta < cos(spotlightConeRad)) {
		return outColor; // Outside spotlight cone
	}

	// Spotlight attenuation
	float spotlightFactor = pow(max(cosTheta, 0.0f), cApp->m_lights->spotlightFactor);

	// Diffuse Lighting Calculation
	Normal n = p->getNormal();
	Vector4 polyNormal = n.getVector().normalize(); // Ensure normal is normalized
	float diffuseIntensity = max(0.0f, polyNormal.dot(lightToSurfaceNeg)) * spotlightFactor;

	// Compute final color
	COLORREF lightColor = RGB(cApp->m_lights->colorR, cApp->m_lights->colorG, cApp->m_lights->colorB);
	COLORREF diffuseColor = ScaleColor(lightColor, diffuseIntensity);

	// Combine colors properly (clamp to avoid overflow)
	outColor = BlendColors(outColor, diffuseColor);

	return outColor;
}

void CCGWorkView::PerspectiveTransformPoly()
{
	CCGWorkApp* cApp = (CCGWorkApp*)AfxGetApp();
	Matrix4 Perspective_mat = Matrix4::PerspectiveMatrix(cApp->fovy, cApp->aspectRatio, cApp->nearPlane, cApp->farPlane, cApp->d);

	for (Poly* poly : *scene.getPolygons()) {
		std::vector<Vertex>& vertices = poly->getVertices();

		for (Vertex& v : vertices)
		{
			Vector4 point(v.x, v.y, v.z, v.w);
			Vector4 transformed = Perspective_mat.MultiplyMatrixVector(point);
			// Perform perspective divide
			if (transformed.w != 0.0f) {
				transformed.x /= transformed.w;
				transformed.y /= transformed.w;
				transformed.z /= transformed.w;
			}
			v.x = transformed.x;
			v.y = transformed.y;
			v.z = transformed.z;
			v.w = transformed.w;

		}

	}

}



void CCGWorkView::PerspectiveView(Point* polygonsBuffer, int width, int height)
{
	CCGWorkApp* cApp = (CCGWorkApp*)AfxGetApp();
	Matrix4 Perspective_mat = Matrix4::PerspectiveMatrix(cApp->fovy, cApp->aspectRatio, cApp->nearPlane, cApp->farPlane, cApp->d);
	CString str;
	str.Format(_T("mat: %.3f %.3f %.3f %.3f , %.3f %.3f %.3f %.3f , %.3f %.3f %.3f %.3f , %.3f %.3f %.3f %.3f"), Perspective_mat.m[0][0], Perspective_mat.m[0][1], Perspective_mat.m[0][2], Perspective_mat.m[0][3], Perspective_mat.m[1][0], Perspective_mat.m[1][1], Perspective_mat.m[1][2], Perspective_mat.m[1][3], Perspective_mat.m[2][0], Perspective_mat.m[2][1], Perspective_mat.m[2][2], Perspective_mat.m[2][3], Perspective_mat.m[3][0], Perspective_mat.m[3][1], Perspective_mat.m[3][2], Perspective_mat.m[3][3]);
	STATUS_BAR_TEXT(str);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			size_t index = y * width + x;
			if (polygonsBuffer[index].getPolygon() == NULL) continue;
			Vector4 point(polygonsBuffer[index].x, polygonsBuffer[index].y, polygonsBuffer[index].z);
			Vector4 transformed = Perspective_mat.MultiplyMatrixVector(point);
			// Perform perspective divide
			if (transformed.w != 0.0f) {
				transformed.x /= transformed.w;
				transformed.y /= transformed.w;
				transformed.z /= transformed.w;
			}


			float screenX = (transformed.x + 1) * (width / 2.0f);
			float screenY = (1 - transformed.y) * (height / 2.0f);

			// Update the buffer
			polygonsBuffer[index].x = screenX;
			polygonsBuffer[index].y = screenY;
			polygonsBuffer[index].z = transformed.z;
			polygonsBuffer[index].w = transformed.w;



		}
	}




	/* old code
	double alpha = (cApp->nearPlane + cApp->farPlane) / (cApp->farPlane - cApp->nearPlane);
	double beta = (2 * cApp->nearPlane * cApp->farPlane) / (cApp->nearPlane - cApp->farPlane);




	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			size_t index = y * width + x;
			polygonsBuffer[index].x = polygonsBuffer[index].x / polygonsBuffer[index].z;
			polygonsBuffer[index].y = polygonsBuffer[index].y / polygonsBuffer[index].z;
			polygonsBuffer[index].z = -(alpha + beta / polygonsBuffer[index].z);



		}

	}

	*/
}