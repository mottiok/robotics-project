#ifndef ROBOMAP_H
#define ROBOMAP_H

#define RGB_BLACK (0x00)
#define RGB_WHITE (0xFF)

#define RGB_TRANSPARENT (0x00)
#define RGB_OPAQUE 	(0xFF)

#include <vector>
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
};

class CMap
{
	dword m_dwMapWidth;
	dword m_dwMapHeight;
	dword m_dwPixelResolution;
	dword m_dwGridResolution;

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
	void SetResolutions(dword dwPixelResolution, dword dwGridResolution);
	SMapCell* GetCellByPixelCoord(double fX, double fY);
	SMapCell* GetMapCell(dword dX, dword dY);
	void ColorCellByCoord(dword dX, dword dY, byte R, byte G, byte B);
	void DumpMap(const char* szFilename);
	SPosition PixelCoordToCellPosition(double fX, double fY);
        dword GetMapWidth();
        dword GetMapHeight();
        
        // SDL2 Implementation
        void DrawMapState(SDL2Wrapper* sdl, bool flushImmediately);
        void FillMapCellByCoord(SDL2Wrapper* sdl, dword dX, dword dY, byte R, byte G, byte B);
};

#endif

