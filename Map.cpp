#include "Map.h"
#include <math.h>
#include "lodepng.h"
#include <stdio.h>

#define RGBA_CELL_SIZE 4

#define HIGH_PRIOR_WEIGHT 1
#define MED_PRIOR_WEIGHT 3
#define LOW_PRIOR_WEIGHT 5
#define DEFAULT_WEIGHT HIGH_PRIOR_WEIGHT
#define OBSTACLE_WEIGHT 10

#define WEIGHT_RADIUS_LOW 1
#define WEIGHT_RADIUS_MED 2
#define WEIGHT_RADIUS_HIGH 3

CMap::~CMap()
{
	if (m_fIsMapLoaded)
	{
		if (NULL != m_pRawMap)
			delete[] m_pRawMap;

		if (NULL != m_pWeightedMap)
			delete [] m_pWeightedMap;
	}
}

void CMap::ConvertMapToBlackAndWhite(vector<SPNGCell>& vMap)
{
	for (dword i = 0; i < vMap.size(); i++)
	{
		if ((RGB_WHITE != vMap[i].bR) ||
		    (RGB_WHITE != vMap[i].bG) ||
		    (RGB_WHITE != vMap[i].bB))
		{
			vMap[i].bR = RGB_BLACK;
			vMap[i].bG = RGB_BLACK;
			vMap[i].bB = RGB_BLACK;
		}
	}
}

void CMap::GetNextPixelCoord(dword& dX, dword& dY)
{
	if (0 == m_dwMapWidth || 0 == m_dwMapHeight)
		return;

	dX = (dX + 1) % m_dwMapWidth;

	if (0 == dX)
	{
		dY = (dY + 1) % m_dwMapHeight;
	}
}

SPNGCell* CMap::GetPixelByCoord(dword dX, dword dY)
{
	if (dX >= m_dwMapWidth || dY >= m_dwMapHeight)
		return NULL;

	if (!m_fIsMapLoaded)
		return NULL;

	return (&m_pRawMap[(dY * m_dwMapWidth) + dX]);
}


void CMap::SetResolutions(dword dwPixelResolution, dword dwGridResolution)
{
	if (0 == dwPixelResolution || 0 == dwGridResolution)
		return;

	m_dwPixelResolution = dwPixelResolution;
	m_dwGridResolution = dwGridResolution;
}

dword CMap::PixelCoordToCellCoord(double fX, double fY)
{
	const double fPixelsPerCell = ((double)m_dwGridResolution / (double)m_dwPixelResolution);
	const dword dwCellsInWidth = ((double)m_dwMapWidth) / fPixelsPerCell;

	// This only works assuming that MapWidth / PixelsPerCell is a whole number!
	// i.e. that dwCellsInWidth indeed gets filled with a legal value 
	dword dwCellX = (dword)floor(fX / fPixelsPerCell);
	dword dwCellY = (dword)floor(fY / fPixelsPerCell);

	return ((dwCellY * dwCellsInWidth) + dwCellX);
}

SMapCell* CMap::GetCellByPixelCoord(double fX, double fY)
{
	if (!m_fIsMapLoaded)
		return NULL;

	if (fX >= m_dwMapWidth || fY >= m_dwMapHeight)
		return NULL;

	dword dwCellOffset = PixelCoordToCellCoord(fX, fY);

	return &(m_pWeightedMap[dwCellOffset]);
}

dword CMap::GetCellStartingPixelByCellOffset(dword dwCellOffset)
{
	const double fPixelsPerCell = ((double)m_dwGridResolution / (double)m_dwPixelResolution);
	const dword dwCellsInWidth = ((double)m_dwMapWidth) / fPixelsPerCell;

	dword dwCellX = dwCellOffset % dwCellsInWidth;
	dword dwCellY = dwCellOffset / dwCellsInWidth;

	return ((dwCellY * dwCellsInWidth * fPixelsPerCell) + (dwCellX * fPixelsPerCell));
}	

void CMap::BlowPixel(dword dwX, dword dwY, word wPixelRadius)
{
	// Lazily make a square with the dimension given in each direction!

	// Determine how far we can go in each direction...
	dword dwLeftLimit, dwUpLimit, dwRightLimit, dwDownLimit;

	if (m_dwMapHeight <= dwY + wPixelRadius)
		dwDownLimit = m_dwMapHeight - dwY;
	else
		dwDownLimit = wPixelRadius;

	if (wPixelRadius > dwY)
		dwUpLimit = dwY;
	else
		dwUpLimit = wPixelRadius;

	if (m_dwMapWidth <= dwX + wPixelRadius)
		dwRightLimit = m_dwMapWidth - dwX;
	else
		dwRightLimit = wPixelRadius;

	if (wPixelRadius > dwX)
		dwLeftLimit = dwX;
	else
		dwLeftLimit = wPixelRadius;

	// Loop!
	dword dwCurrX = dwX - dwLeftLimit;
	dword dwCurrY = dwY - dwUpLimit;

	while (dwCurrX != (dwX + dwRightLimit - 1) || dwCurrY != (dwY + dwDownLimit - 1))
	{
		SPNGCell* pCurrPixel = GetPixelByCoord(dwCurrX, dwCurrY);

		pCurrPixel->bR = RGB_BLACK;
		pCurrPixel->bG = RGB_BLACK;
		pCurrPixel->bB = RGB_BLACK;

		dwCurrX++;

		if (dwCurrX == m_dwMapWidth || dwCurrX == dwX + dwRightLimit)
		{
			dwCurrX = dwX - dwLeftLimit;
			dwCurrY++;
		}
	}
}

struct SPosition
{
	dword dwX;
	dword dwY;
};

bool CMap::BlowMapObstacles(word wPixelRadius)
{
	if (!m_fIsMapLoaded)
		return false;

	// Find all the pixels that need blowing...
	vector<SPosition> vObstaclePixels;

	vObstaclePixels.resize(m_dwMapWidth * m_dwMapHeight);

	for (dword dwX = 0, dwY = 0;
	     dwX != m_dwMapWidth - 1 || dwY != m_dwMapHeight - 1; 
	     GetNextPixelCoord(dwX, dwY))
	{
		SPNGCell* pCurrPixel = GetPixelByCoord(dwX, dwY);

		if ((RGB_WHITE != pCurrPixel->bR) ||
		    (RGB_WHITE != pCurrPixel->bG) ||
		    (RGB_WHITE != pCurrPixel->bB))
		{
			vObstaclePixels[(dwY * m_dwMapWidth) + dwX].dwX = dwX;
			vObstaclePixels[(dwY * m_dwMapWidth) + dwX].dwY = dwY;
		}
	}

	// Blow each pixel
	for (dword dwCurrPixel = 0; dwCurrPixel < vObstaclePixels.size(); dwCurrPixel++)
	{
		dword dwCurrX = vObstaclePixels[dwCurrPixel].dwX;
		dword dwCurrY = vObstaclePixels[dwCurrPixel].dwY;

		if (0 != dwCurrX && 0 != dwCurrY)
		{
			BlowPixel(dwCurrX, dwCurrY, wPixelRadius);
		}
	}

	return true;
}

bool CMap::IsPassableCell(dword dwCellOffset)
{
	dword dwPixelOffset = GetCellStartingPixelByCellOffset(dwCellOffset);
	const double fPixelsPerCell = ((double)m_dwGridResolution / (double)m_dwPixelResolution);
	const dword dwCellsInWidth = ((double)m_dwMapWidth) / fPixelsPerCell;

	dword dwX = dwPixelOffset % m_dwMapWidth;
	dword dwY = (dwPixelOffset / m_dwMapWidth) * fPixelsPerCell;
	dword dwCurrX = dwX, dwCurrY = dwY;

	// Check all pixels in this cell
	while (dwCurrX != (dwX) || dwCurrY != (dwY + fPixelsPerCell))
	{
		SPNGCell* pCurrPixel = GetPixelByCoord(dwCurrX, dwCurrY);
		
		if ((RGB_WHITE != pCurrPixel->bR) ||
		    (RGB_WHITE != pCurrPixel->bG) ||
		    (RGB_WHITE != pCurrPixel->bB))
		{
			return false;
		}

		dwCurrX++;

		if (dwCurrX == m_dwMapWidth || dwCurrX == dwX + fPixelsPerCell)
		{
			dwCurrX = dwX;
			dwCurrY++;
		}
	}

	return true;
}

void CMap::PrepareMapGrid()
{
	const double fPixelsPerCell = ((double)m_dwGridResolution / (double)m_dwPixelResolution);
	const dword dwCellsInWidth = ((double)m_dwMapWidth) / fPixelsPerCell;
	const dword dwCellsInHeight = ((double)m_dwMapHeight) / fPixelsPerCell;
	
	if (NULL != m_pWeightedMap)
		delete[] m_pWeightedMap;

	m_pWeightedMap = new SMapCell[dwCellsInHeight * dwCellsInWidth];

	// For each cell determine if passable or not!
	for (dword dwCurrCell = 0; dwCurrCell < dwCellsInHeight * dwCellsInWidth; dwCurrCell++)
	{
		m_pWeightedMap[dwCurrCell].fIsPassable = IsPassableCell(dwCurrCell);
		m_pWeightedMap[dwCurrCell].dwWeight = 
			(m_pWeightedMap[dwCurrCell].fIsPassable) ? DEFAULT_WEIGHT : OBSTACLE_WEIGHT;
	}
}

void CMap::ColorCell(dword dwCellOffset, byte R, byte G, byte B)
{
	dword dwPixelOffset = GetCellStartingPixelByCellOffset(dwCellOffset);
	const double fPixelsPerCell = ((double)m_dwGridResolution / (double)m_dwPixelResolution);

	dword dwX = dwPixelOffset % m_dwMapWidth;
	dword dwY = (dwPixelOffset / m_dwMapWidth) * fPixelsPerCell;
	dword dwCurrX = dwX, dwCurrY = dwY;
	
	const dword dwCellsInWidth = ((double)m_dwMapWidth) / fPixelsPerCell;
	const dword dwCellsInHeight = ((double)m_dwMapHeight) / fPixelsPerCell;

	dword dwCellX = dwCellOffset % dwCellsInWidth;
	dword dwCellY = dwCellOffset / dwCellsInWidth;

	// Color all pixels
	while (dwCurrX != dwX || ((dwCurrY != (dwY + fPixelsPerCell)) && (dwCurrY < m_dwMapHeight)))
	{
		SPNGCell* pCurrPixel = GetPixelByCoord(dwCurrX, dwCurrY);

		pCurrPixel->bR = R;
		pCurrPixel->bG = G;
		pCurrPixel->bB = B;

		dwCurrX++;

		if (dwCurrX == m_dwMapWidth || dwCurrX == dwX + fPixelsPerCell)
		{
			dwCurrX = dwX;
			dwCurrY++;
		}
	}
}

void CMap::CoatObstacle(dword dwCellOffset, dword dwRadius, dword dwWeight)
{
	const double fPixelsPerCell = ((double)m_dwGridResolution / (double)m_dwPixelResolution);
	const dword dwCellsInWidth = ((double)m_dwMapWidth) / fPixelsPerCell;
	const dword dwCellsInHeight = ((double)m_dwMapHeight) / fPixelsPerCell;

	dword dwX = dwCellOffset % dwCellsInWidth;
	dword dwY = dwCellOffset / dwCellsInWidth;

	dword dwLeftLimit, dwRightLimit, dwDownLimit, dwUpLimit;

	if (dwCellsInHeight <= dwY + dwRadius)
		dwDownLimit = dwCellsInHeight - dwY;
	else
		dwDownLimit = dwRadius;

	if (dwRadius > dwY)
		dwUpLimit = dwY;
	else
		dwUpLimit = dwRadius;

	if (dwCellsInWidth <= dwX + dwRadius)
		dwRightLimit = dwCellsInWidth - dwX;
	else
		dwRightLimit = dwRadius;

	if (dwRadius > dwX)
		dwLeftLimit = dwX;
	else
		dwLeftLimit = dwRadius;

//	printf("There is an obstacle at (%u, %u)! U: %u R: %u D: %u L: %u W: %u H: %u\n", dwX, dwY, dwUpLimit, dwRightLimit, dwDownLimit, dwLeftLimit, dwCellsInWidth, dwCellsInHeight);

	dword dwCurrX = dwX - dwLeftLimit;
	dword dwCurrY = dwY - dwUpLimit;

	while (dwCurrX != (dwX - dwLeftLimit) || dwCurrY != (dwY + dwDownLimit + 1))
	{
		if (m_pWeightedMap[(dwCurrY * dwCellsInWidth) + dwCurrX].fIsPassable &&
		    m_pWeightedMap[(dwCurrY * dwCellsInWidth) + dwCurrX].dwWeight < dwWeight)
		{
			// DEBUG!
			if (dwWeight == LOW_PRIOR_WEIGHT)
			{
				ColorCell((dwCurrY * dwCellsInWidth) + dwCurrX, 142, 142, 142); // Debug, color grey
			}
			else if (dwWeight == MED_PRIOR_WEIGHT)
			{
				ColorCell((dwCurrY * dwCellsInWidth) + dwCurrX, 198, 198, 198); // Debug, color grey			
			}

			m_pWeightedMap[(dwCurrY * dwCellsInWidth) + dwCurrX].dwWeight = dwWeight;
		}

		dwCurrX++;

		if (dwCurrX == dwCellsInWidth || dwCurrX == dwX + dwRightLimit + 1)
		{
			dwCurrX = dwX - dwLeftLimit;
			dwCurrY++;
		}
	}
}

void CMap::CoatObstacleWeights()
{
	const double fPixelsPerCell = ((double)m_dwGridResolution / (double)m_dwPixelResolution);
	const dword dwCellsInWidth = ((double)m_dwMapWidth) / fPixelsPerCell;
	const dword dwCellsInHeight = ((double)m_dwMapHeight) / fPixelsPerCell;

	printf("Map has %u x %u cells (cell size %u x %u).\n", dwCellsInWidth, dwCellsInHeight, (dword)fPixelsPerCell, (dword)fPixelsPerCell);

	// For each cell determine if passable or not!
	for (dword dwCurrCell = 0; dwCurrCell < dwCellsInHeight * dwCellsInWidth; dwCurrCell++)
	{
		dword dwPixelOffset = GetCellStartingPixelByCellOffset(dwCurrCell);
		
		if (!m_pWeightedMap[dwCurrCell].fIsPassable)
		{
			CoatObstacle(dwCurrCell, WEIGHT_RADIUS_MED, MED_PRIOR_WEIGHT);
			CoatObstacle(dwCurrCell, WEIGHT_RADIUS_LOW, LOW_PRIOR_WEIGHT);
		}
	}
}

bool CMap::LoadMap(char* szMapPath)
{
	vector<byte> vRawImg;

	// Decode PNG into RGBA vector
	lodepng::decode(vRawImg, m_dwMapWidth, m_dwMapHeight, szMapPath);

	// Create PNGCell vector
	vector<SPNGCell> vPNGImg;
	vPNGImg.resize(m_dwMapWidth * m_dwMapHeight);

	for (dword dwCurrPixel = 0; dwCurrPixel < vRawImg.size(); dwCurrPixel += RGBA_CELL_SIZE)
	{
		vPNGImg[dwCurrPixel / RGBA_CELL_SIZE].bR = vRawImg[dwCurrPixel + 0];
		vPNGImg[dwCurrPixel / RGBA_CELL_SIZE].bG = vRawImg[dwCurrPixel + 1];
		vPNGImg[dwCurrPixel / RGBA_CELL_SIZE].bB = vRawImg[dwCurrPixel + 2];
		vPNGImg[dwCurrPixel / RGBA_CELL_SIZE].bAlpha = vRawImg[dwCurrPixel + 3];
	}

	DumpMap(vPNGImg, m_dwMapWidth, m_dwMapHeight, "orig.png");

	// Convert to black and white
	ConvertMapToBlackAndWhite(vPNGImg);
	DumpMap(vPNGImg, m_dwMapWidth, m_dwMapHeight, "black_and_white.png");

	// Copy the PNG image into ourselves
	m_pRawMap = new SPNGCell[m_dwMapWidth * m_dwMapHeight];

	for (dword dwCurrPixel = 0; dwCurrPixel < vPNGImg.size(); dwCurrPixel++)
	{
		m_pRawMap[dwCurrPixel].bR = vPNGImg[dwCurrPixel].bR;
		m_pRawMap[dwCurrPixel].bG = vPNGImg[dwCurrPixel].bG;
		m_pRawMap[dwCurrPixel].bB = vPNGImg[dwCurrPixel].bB;
		m_pRawMap[dwCurrPixel].bAlpha = vPNGImg[dwCurrPixel].bAlpha;
	}

	m_fIsMapLoaded = true;

	// Bloat obstacles
	BlowMapObstacles(5);
	DumpMap(m_pRawMap, m_dwMapWidth, m_dwMapHeight, "bloated.png");

	PrepareMapGrid();

	// Create weight radius around obstacles for smooth path finding
	CoatObstacleWeights();

	DumpMap(m_pRawMap, m_dwMapWidth, m_dwMapHeight, "weighted.png");

	return true;
}

void CMap::DumpMap(SPNGCell* pMap, dword dwWidth, dword dwHeight, char* szFilename)
{
	vector<SPNGCell> map;

	map.resize(dwWidth * dwHeight);

	for (dword dwCurrPixel = 0; dwCurrPixel < dwWidth * dwHeight; dwCurrPixel++)
	{
		map[dwCurrPixel].bR = pMap[dwCurrPixel].bR;
		map[dwCurrPixel].bG = pMap[dwCurrPixel].bG;
		map[dwCurrPixel].bB = pMap[dwCurrPixel].bB;
		map[dwCurrPixel].bAlpha = pMap[dwCurrPixel].bAlpha;
	}

	DumpMap(map, dwWidth, dwHeight, szFilename);
}

void CMap::DumpMap(vector<SPNGCell>& map, dword dwWidth, dword dwHeight, char* szFilename)
{
	vector<byte> vRawImg;

	vRawImg.resize(map.size() * RGBA_CELL_SIZE);

	for (dword dwCurrPixel = 0; dwCurrPixel < vRawImg.size(); dwCurrPixel += RGBA_CELL_SIZE)
	{
		vRawImg[dwCurrPixel + 0] = map[dwCurrPixel / RGBA_CELL_SIZE].bR;
		vRawImg[dwCurrPixel + 1] = map[dwCurrPixel / RGBA_CELL_SIZE].bG;
		vRawImg[dwCurrPixel + 2] = map[dwCurrPixel / RGBA_CELL_SIZE].bB;
		vRawImg[dwCurrPixel + 3] = map[dwCurrPixel / RGBA_CELL_SIZE].bAlpha;
	}

	lodepng::encode(szFilename, vRawImg, dwWidth, dwHeight);
}
