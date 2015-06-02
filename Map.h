#ifndef ROBOMAP_H
#define ROBOMAP_H

#define RGB_BLACK (0x00)
#define RGB_WHITE (0xFF)

#define RGB_TRANSPARENT (0x00)
#define RGB_OPAQUE 	(0xFF)

#include <vector>
#include "typedefs.h"

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

	// Debug
	void DumpMap(SPNGCell* pMap, dword dwWidth, dword dwHeight, char* szFilename);
	void DumpMap(vector<SPNGCell>& map, dword dwWidth, dword dwHeight, char* szFilename);

	public:
		
	CMap() : m_pRawMap(NULL), m_pWeightedMap(NULL), m_fIsMapLoaded(false),
		 m_dwMapWidth(0), m_dwMapHeight(0), m_dwPixelResolution(0),
		 m_dwGridResolution(0) {};
	~CMap();

	bool LoadMap(char* szMapPath);
	bool BlowMapObstacles(word wPixelRadius);
	void SetResolutions(dword dwPixelResolution, dword dwGridResolution);
	SMapCell* GetCellByPixelCoord(double fX, double fY);
};

#endif

