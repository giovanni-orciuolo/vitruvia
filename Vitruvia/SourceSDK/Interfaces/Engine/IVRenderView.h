/*
	 _____                            ___________ _   __
	/  ___|                          /  ___|  _  \ | / /
	\ `--.  ___  _   _ _ __ ___ ___  \ `--.| | | | |/ /
	 `--. \/ _ \| | | | '__/ __/ _ \  `--. \ | | |    \
	/\__/ / (_) | |_| | | | (_|  __/ /\__/ / |/ /| |\  \
	\____/ \___/ \__,_|_|  \___\___| \____/|___/ \_| \_/

	Designed for Counter-Strike: Global Offensive
	Copyright: ValvE Corporation 2017 (C)
*/

#pragma once
#include "../../SourceCommon.h"
#include "../MaterialSystem/IMaterial.h"
#include "../Client/IClientEntity.h"
#include "../IAppSystem.h"

SOURCE_INIT

abstract_class IWorldRenderList : public IRefCounted
{
};

struct VisibleFogVolumeInfo_t
{
	int		m_nVisibleFogVolume;
	int		m_nVisibleFogVolumeLeaf;
	bool	m_bEyeInFogVolume;
	float	m_flDistanceToWater;
	float	m_flWaterHeight;
	IMaterial *m_pFogVolumeMaterial;
};

typedef unsigned short LeafIndex_t;

enum
{
	INVALID_LEAF_INDEX = (LeafIndex_t)~0
};

struct WorldListLeafData_t
{
	LeafIndex_t	leafIndex;	// 16 bits
	int16	waterData;
	uint16 	firstTranslucentSurface;	// engine-internal list index
	uint16	translucentSurfaceCount;	// count of translucent surfaces+disps
};

struct WorldListInfo_t
{
	int		m_ViewFogVolume;
	int		m_LeafCount;
	bool	m_bHasWater;
	WorldListLeafData_t	*m_pLeafDataList;
};

struct VisOverrideData_t
{
	Vector		m_vecVisOrigin;					// The point to to use as the viewpoint for area portal backface cull checks.
	float		m_fDistToAreaPortalTolerance;	// The distance from an area portal before using the full screen as the viewable portion.
};

struct BrushVertex_t
{
	Vector		m_Pos;
	Vector		m_Normal;
	Vector		m_TangentS;
	Vector		m_TangentT;
	Vector2	m_TexCoord;
	Vector2	m_LightmapCoord;

private:
	BrushVertex_t(const BrushVertex_t& src);
};

abstract_class IBrushSurface
{
public:
	// Computes texture coordinates + lightmap coordinates given a world position
	virtual void ComputeTextureCoordinate(Vector const& worldPos, Vector2& texCoord) = 0;
	virtual void ComputeLightmapCoordinate(Vector const& worldPos, Vector2& lightmapCoord) = 0;

	// Gets the vertex data for this surface
	virtual int  GetVertexCount() const = 0;
	virtual void GetVertexData(BrushVertex_t* pVerts) = 0;

	// Gets at the material properties for this surface
	virtual IMaterial* GetMaterial() = 0;
};

abstract_class IBrushRenderer
{
public:
	// Draws the surface; returns true if decals should be rendered on this surface
	virtual bool RenderBrushModelSurface(CBaseEntity* pBaseEntity, IBrushSurface* pBrushSurface) = 0;
};

struct colorVec
{
	unsigned r, g, b, a;
};

struct CViewSetup
{
	char _0x0000[16];
	__int32 x;
	__int32 x_old;
	__int32 y;
	__int32 y_old;
	__int32 width;
	__int32    width_old;
	__int32 height;
	__int32    height_old;
	char _0x0030[128];
	float fov;
	float fovViewmodel;
	Vector origin;
	Vector angles;
	float zNear;
	float zFar;
	float zNearViewmodel;
	float zFarViewmodel;
	float m_flAspectRatio;
	float m_flNearBlurDepth;
	float m_flNearFocusDepth;
	float m_flFarFocusDepth;
	float m_flFarBlurDepth;
	float m_flNearBlurRadius;
	float m_flFarBlurRadius;
	float m_nDoFQuality;
	__int32 m_nMotionBlurMode;
	char _0x0104[68];
	__int32 m_EdgeBlur;
};

struct WriteReplayScreenshotParams_t;

abstract_class IViewRender
{
public:
	// SETUP
	// Initialize view renderer
	virtual void		Init(void) = 0;

	// Clear any systems between levels
	virtual void		LevelInit(void) = 0;
	virtual void		LevelShutdown(void) = 0;

	// Shutdown
	virtual void		Shutdown(void) = 0;

	// RENDERING
	// Called right before simulation. It must setup the view model origins and angles here so 
	// the correct attachment points can be used during simulation.	
	virtual void		OnRenderStart() = 0;

	// Called to render the entire scene
	virtual	void		Render(void *rect) = 0;

	// Called to render just a particular setup ( for timerefresh and envmap creation )
	virtual void		RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw) = 0;

	// What are we currently rendering? Returns a combination of DF_ flags.
	virtual int GetDrawFlags() = 0;

	// MISC
	// Start and stop pitch drifting logic
	virtual void		StartPitchDrift(void) = 0;
	virtual void		StopPitchDrift(void) = 0;

	// This can only be called during rendering (while within RenderView).
	virtual void*		GetFrustum() = 0;

	virtual bool		ShouldDrawBrushModels(void) = 0;

	virtual const CViewSetup *GetPlayerViewSetup(void) const = 0;
	virtual const CViewSetup *GetViewSetup(void) const = 0;

	virtual void		DisableVis(void) = 0;

	virtual int			BuildWorldListsNumber() const = 0;

	virtual void		SetCheapWaterStartDistance(float flCheapWaterStartDistance) = 0;
	virtual void		SetCheapWaterEndDistance(float flCheapWaterEndDistance) = 0;

	virtual void		GetWaterLODParams(float &flCheapWaterStartDistance, float &flCheapWaterEndDistance) = 0;

	virtual void		DriftPitch(void) = 0;

	virtual void		SetScreenOverlayMaterial(IMaterial *pMaterial) = 0;
	virtual IMaterial	*GetScreenOverlayMaterial() = 0;

	virtual void		WriteSaveGameScreenshot(const char *pFilename) = 0;
	virtual void		WriteSaveGameScreenshotOfSize(const char *pFilename, int width, int height, bool bCreatePowerOf2Padded = false, bool bWriteVTF = false) = 0;

	virtual void		WriteReplayScreenshot(WriteReplayScreenshotParams_t &params) = 0;
	virtual void		UpdateReplayScreenshotCache() = 0;

	// Draws another rendering over the top of the screen
	virtual void		QueueOverlayRenderView(const CViewSetup &view, int nClearFlags, int whatToDraw) = 0;

	// Returns znear and zfar
	virtual float		GetZNear() = 0;
	virtual float		GetZFar() = 0;

	virtual void		GetScreenFadeDistances(float *min, float *max) = 0;

	virtual void		*GetCurrentlyDrawingEntity() = 0;
	virtual void		SetCurrentlyDrawingEntity(void *pEnt) = 0;

	virtual bool		UpdateShadowDepthTexture(ITexture *pRenderTarget, ITexture *pDepthTexture, const CViewSetup &shadowView) = 0;

	virtual void		FreezeFrame(float flFreezeTime) = 0;

	virtual void		*GetReplayScreenshotSystem() = 0;
};

abstract_class IVRenderView
{
public:

	// Draw normal brush model.
	// If pMaterialOverride is non-null, then all the faces of the bmodel will
	// set this material rather than their regular material.
	virtual void			DrawBrushModel(CBaseEntity *baseentity, model_t *model, const Vector& origin, const QAngle& angles, bool sort) = 0;

	// Draw brush model that has no origin/angles change ( uses identity transform )
	// FIXME, Material proxy IClientEntity *baseentity is unused right now, use DrawBrushModel for brushes with
	//  proxies for now.
	virtual void			DrawIdentityBrushModel(IWorldRenderList *pList, model_t *model) = 0;

	// Mark this dynamic light as having changed this frame ( so light maps affected will be recomputed )
	virtual void			TouchLight(struct dlight_t *light) = 0;
	// Draw 3D Overlays
	virtual void			Draw3DDebugOverlays(void) = 0;
	// Sets global blending fraction
	virtual void			SetBlend(float blend) = 0;
	virtual float			GetBlend(void) = 0;

	// Sets global color modulation
	virtual void			SetColorModulation(float const* blend) = 0;
	virtual void			GetColorModulation(float* blend) = 0;

	// Wrap entire scene drawing
	virtual void			SceneBegin(void) = 0;
	virtual void			SceneEnd(void) = 0;

	// Gets the fog volume for a particular point
	virtual void			GetVisibleFogVolume(const Vector& eyePoint, VisibleFogVolumeInfo_t *pInfo) = 0;

	// Wraps world drawing
	// If iForceViewLeaf is not -1, then it uses the specified leaf as your starting area for setting up area portal culling.
	// This is used by water since your reflected view origin is often in solid space, but we still want to treat it as though
	// the first portal we're looking out of is a water portal, so our view effectively originates under the water.
	virtual IWorldRenderList * CreateWorldList() = 0;

	virtual void			BuildWorldLists(IWorldRenderList *pList, WorldListInfo_t* pInfo, int iForceFViewLeaf, const VisOverrideData_t* pVisData = NULL, bool bShadowDepth = false, float *pReflectionWaterHeight = NULL) = 0;
	virtual void			DrawWorldLists(IWorldRenderList *pList, unsigned long flags, float waterZAdjust) = 0;
	virtual int				GetNumIndicesForWorldLists(IWorldRenderList *pList, unsigned long nFlags) = 0;

	// Optimization for top view
	virtual void			DrawTopView(bool enable) = 0;
	virtual void			TopViewBounds(Vector2 const& mins, Vector2 const& maxs) = 0;

	// Draw lights
	virtual void			DrawLights(void) = 0;
	// FIXME:  This function is a stub, doesn't do anything in the engine right now
	virtual void			DrawMaskEntities(void) = 0;

	// Draw surfaces with alpha, don't call in shadow depth pass
	virtual void			DrawTranslucentSurfaces(IWorldRenderList *pList, int *pSortList, int sortCount, unsigned long flags) = 0;

	// Draw Particles ( just draws the linefine for debugging map leaks )
	virtual void			DrawLineFile(void) = 0;
	// Draw lightmaps
	virtual void			DrawLightmaps(IWorldRenderList *pList, int pageId) = 0;
	// Wraps view render sequence, sets up a view
	virtual void			ViewSetupVis(bool novis, int numorigins, const Vector origin[]) = 0;

	// Return true if any of these leaves are visible in the current PVS.
	virtual bool			AreAnyLeavesVisible(int *leafList, int nLeaves) = 0;

	virtual	void			VguiPaint(void) = 0;
	// Sets up view fade parameters
	virtual void			ViewDrawFade(byte *color, IMaterial *pMaterial) = 0;
	// Sets up the projection matrix for the specified field of view
	virtual void			OLD_SetProjectionMatrix(float fov, float zNear, float zFar) = 0;
	// Determine lighting at specified position
	virtual colorVec		GetLightAtPoint(Vector& pos) = 0;
	// Whose eyes are we looking through?
	virtual int				GetViewEntity(void) = 0;
	virtual bool			IsViewEntity(int entindex) = 0;
	// Get engine field of view setting
	virtual float			GetFieldOfView(void) = 0;
	// 1 == ducking, 0 == not
	virtual unsigned char	**GetAreaBits(void) = 0;

	// Set up fog for a particular leaf
	virtual void			SetFogVolumeState(int nVisibleFogVolume, bool bUseHeightFog) = 0;

	// Installs a brush surface draw override method, null means use normal renderer
	virtual void			InstallBrushSurfaceRenderer(IBrushRenderer* pBrushRenderer) = 0;

	// Draw brush model shadow
	virtual void			DrawBrushModelShadow(IClientRenderable *pRenderable) = 0;

	// Does the leaf contain translucent surfaces?
	virtual	bool			LeafContainsTranslucentSurfaces(IWorldRenderList *pList, int sortIndex, unsigned long flags) = 0;

	virtual bool			DoesBoxIntersectWaterVolume(const Vector &mins, const Vector &maxs, int leafWaterDataID) = 0;

	virtual void			SetAreaState(unsigned char chAreaBits[32], unsigned char chAreaPortalBits[24]) = 0;

	// See i
	virtual void			VGui_Paint(int mode) = 0;
};

SOURCE_END