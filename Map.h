#ifndef ROBOMAP_H
#define ROBOMAP_H

#define RGB_BLACK (0x00)
#define RGB_WHITE (0xFF)

#define RGB_TRANSPARENT (0x00)
#define RGB_OPAQUE 	(0xFF)

#define NORMALIZE_ANGLE(angle)		\
{					\
if (angle < 0)				\
	angle += (2 * M_PI);		\
else if (angle >= (2 * M_PI))		\
	while (angle > (2 * M_PI))	\
		angle -= (2 * M_PI);	\
}					\

#include <vector>
#include <float.h>
#include <cmath>
#include "typedefs.h"
#include "SDL2Wrapper.h"

using namespace std;

struct SPNGCell
{
	byte bR;
	byte bG;
	byte bB;
	byte bAlpha;
};

struct SMapCell
{
	dword dwWeight;
	bool  fIsPassable;
};

struct SPosition
{
	dword dwX;
	dword dwY;
	SPosition(): dwX(0), dwY(0) { }
	SPosition(dword dX, dword dY): dwX(dX), dwY(dY) { }
	
	static float CalcDistance(SPosition& from, SPosition& to)
	{
		float dY = (signed)(to.dwY - from.dwY);
		float dX = (signed)(to.dwX - from.dwX);

		return sqrt(dX * dX + dY * dY);
	}

	static float CalcGradient(SPosition& from, SPosition& to)
	{
		float dY = (signed)(to.dwY - from.dwY);
		float dX = (signed)(to.dwX - from.dwX);
	
		if (to.dwX == from.dwX)
			return FLT_MAX;
		else
			return abs(dY/dX);
	}

	static float CalcLeftAngleCost(float from, float to)
	{
		float fLeftCost = (2 * M_PI) - (from - to);
		// return ((2 * M_PI) - CalcRightAngleCost(from, to));

		NORMALIZE_ANGLE(fLeftCost);

		return fLeftCost;
	}

	static float CalcRightAngleCost(float from, float to)
	{
		float fRightCost = (from - to);

		NORMALIZE_ANGLE(fRightCost);

		return fRightCost;
	}

	static float CalcGradientAngleOffset(SPosition& from, SPosition& to, float angle)
	{
		//float fGradient = SPosition::CalcGradient(from, to);
		float dY = (signed)(to.dwY - from.dwY);
		float dX = (signed)(to.dwX - from.dwX);

		float fNecessaryAngle = atan2(dY, dX);

		// tan counts from -90 degrees by our map bearing with the wrong sign
		float fMapBearing = ((fNecessaryAngle < 0) ? (2 * M_PI - fNecessaryAngle) : (fNecessaryAngle)) - (M_PI / 2);
		NORMALIZE_ANGLE(fMapBearing);

		printf("Bearing %f needs to be fixed to %f\n", angle, fMapBearing);

	//	printf("Gradient from (%u, %u) to (%u, %u) is %f rads with wanted %f, off by %f!\n", 
	//		from.dwX, from.dwY, to.dwX, to.dwY, fNecessaryAngle, angle, angle - fNecessaryAngle);

		float fLeftCost = CalcLeftAngleCost(angle, fMapBearing);
		float fRightCost = CalcRightAngleCost(angle, fMapBearing);

		printf("Right cost: %f, left cost: %f\n", fRightCost, fLeftCost);

		// If turning right costs less, return it as a negative number
		if (fRightCost < fLeftCost)
			return (-1 * fRightCost);
		// Otherwise it's to the left which is just a positive number
		else
			return (fLeftCost);
	}
};

class CMap
{
	dword m_dwMapWidth;
	dword m_dwMapHeight;
	double m_dwPixelResolution;
	double m_dwGridResolution;

	SPNGCell *m_pRawMap;
	SMapCell *m_pWeightedMap;

	bool m_fIsMapLoaded;

	void ConvertMapToBlackAndWhite(vector<SPNGCell>& vMap);
	void GetNextPixelCoord(dword& dX, dword& dY);
	SPNGCell* GetPixelByCoord(dword dX, dword dY);
	dword PixelCoordToCellCoord(double fX, double fY);
	void BlowPixel(dword dwX, dword dwY, word wPixelRadius);
	dword GetCellStartingPixelByCellOffset(dword dwCellOffset);
	bool IsPassableCell(dword dwCellOffset);
	void PrepareMapGrid();
	void ColorCell(dword dwCellOffset, byte R, byte G, byte B);
	void CoatObstacle(dword dwCellOffset, dword dwRadius, dword dwWeight);
	void CoatObstacleWeights();
	const double PixelPerCell();
	const dword CellsInWidth();
	const dword CellsInHeight();
	dword GetCellOffsetByCoord(dword dX, dword dY);

	// Debug
	void DumpMap(SPNGCell* pMap, dword dwWidth, dword dwHeight, const char* szFilename);
	void DumpMap(vector<SPNGCell>& map, dword dwWidth, dword dwHeight, const char* szFilename);
        
	// SDL2 Implementation
	void DrawPixel(SDL2Wrapper* sdl, dword dwX, dword dwY, SPNGCell pixel);
	void FillMapCellByCellOffset(SDL2Wrapper* sdl, dword dwCellOffset, byte R, byte G, byte B);

	public:
		
	CMap() : m_pRawMap(NULL), m_pWeightedMap(NULL), m_fIsMapLoaded(false),
		 m_dwMapWidth(0), m_dwMapHeight(0), m_dwPixelResolution(0),
		 m_dwGridResolution(0) {};
	~CMap();

	bool LoadMap(const char* szMapPath);
	bool BlowMapObstacles(word wPixelRadius);
	void SetResolutions(double dwPixelResolution, double dwGridResolution);
	SMapCell* GetCellByPixelCoord(double fX, double fY);
	SMapCell* GetMapCell(dword dX, dword dY);
	void ColorCellByCoord(dword dX, dword dY, byte R, byte G, byte B);
	void DumpMap(const char* szFilename);
	SPosition PixelCoordToCellPosition(double fX, double fY);
	dword GetMapWidth();
	dword GetMapHeight();
	double GetPixelResolution();
	double GetGridResolution();
        
	// SDL2 Implementation
	void DrawMapState(SDL2Wrapper* sdl, bool flushImmediately);
	void FillMapCellByCoord(SDL2Wrapper* sdl, dword dX, dword dY, byte R, byte G, byte B);
};

#endif

