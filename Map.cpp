#include "Map.h"
#include <math.h>
#include "Libraries/lodepng.h"
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


void CMap::SetResolutions(double dwPixelResolution, double dwGridResolution)
{
	if (0 == dwPixelResolution || 0 == dwGridResolution)
		return;

	m_dwPixelResolution = dwPixelResolution;
	m_dwGridResolution = dwGridResolution;
}

dword CMap::PixelCoordToCellCoord(double fX, double fY)
{
	// This only works assuming that MapWidth / PixelsPerCell is a whole number!
	// i.e. that dwCellsInWidth indeed gets filled with a legal value
        SPosition position = PixelCoordToCellPosition(fX, fY);
	return ((position.dwY * CellsInWidth()) + position.dwX);
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
	dword dwCellX = dwCellOffset % CellsInWidth();
	dword dwCellY = dwCellOffset / CellsInWidth();

	return ((dwCellY * m_dwMapWidth * PixelPerCell()) + (dwCellX * PixelPerCell()));
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

	dword dwX = dwPixelOffset % m_dwMapWidth;
	dword dwY = (dwPixelOffset / m_dwMapWidth);
	dword dwCurrX = dwX, dwCurrY = dwY;

	// Check all pixels in this cell
	while (dwCurrX != (dwX) || dwCurrY != (dwY + PixelPerCell()))
	{
		SPNGCell* pCurrPixel = GetPixelByCoord(dwCurrX, dwCurrY);
		
		if ((RGB_WHITE != pCurrPixel->bR) ||
		    (RGB_WHITE != pCurrPixel->bG) ||
		    (RGB_WHITE != pCurrPixel->bB))
		{
			return false;
		}

		dwCurrX++;

		if (dwCurrX == m_dwMapWidth || dwCurrX == dwX + PixelPerCell())
		{
			dwCurrX = dwX;
			dwCurrY++;
		}
	}

	return true;
}

void CMap::PrepareMapGrid()
{
	if (NULL != m_pWeightedMap)
		delete[] m_pWeightedMap;

	m_pWeightedMap = new SMapCell[CellsInHeight() * CellsInWidth()];

	// For each cell determine if passable or not!
	for (dword dwCurrCell = 0; dwCurrCell < CellsInHeight() * CellsInWidth(); dwCurrCell++)
	{
		m_pWeightedMap[dwCurrCell].fIsPassable = IsPassableCell(dwCurrCell);
		m_pWeightedMap[dwCurrCell].dwWeight = 
			(m_pWeightedMap[dwCurrCell].fIsPassable) ? DEFAULT_WEIGHT : OBSTACLE_WEIGHT;
	}
}

void CMap::ColorCell(dword dwCellOffset, byte R, byte G, byte B)
{
	dword dwPixelOffset = GetCellStartingPixelByCellOffset(dwCellOffset);

	dword dwX = dwPixelOffset % m_dwMapWidth;
	dword dwY = (dwPixelOffset / m_dwMapWidth);
	dword dwCurrX = dwX, dwCurrY = dwY;

//	dword dwCellX = dwCellOffset % CellsInWidth();
//	dword dwCellY = dwCellOffset / CellsInWidth();

	// Color all pixels
	while (dwCurrX != dwX || ((dwCurrY != (dwY + PixelPerCell())) && (dwCurrY < m_dwMapHeight)))
	{
		SPNGCell* pCurrPixel = GetPixelByCoord(dwCurrX, dwCurrY);

		pCurrPixel->bR = R;
		pCurrPixel->bG = G;
		pCurrPixel->bB = B;

		dwCurrX++;

		if (dwCurrX == m_dwMapWidth || dwCurrX == dwX + PixelPerCell())
		{
			dwCurrX = dwX;
			dwCurrY++;
		}
	}
}

void CMap::CoatObstacle(dword dwCellOffset, dword dwRadius, dword dwWeight)
{
	dword dwX = dwCellOffset % CellsInWidth();
	dword dwY = dwCellOffset / CellsInWidth();

	dword dwLeftLimit, dwRightLimit, dwDownLimit, dwUpLimit;

	if (CellsInHeight() <= dwY + dwRadius)
		dwDownLimit = CellsInHeight() - dwY;
	else
		dwDownLimit = dwRadius;

	if (dwRadius > dwY)
		dwUpLimit = dwY;
	else
		dwUpLimit = dwRadius;

	if (CellsInWidth() <= dwX + dwRadius)
		dwRightLimit = CellsInWidth() - dwX;
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
		if (m_pWeightedMap[(dwCurrY * CellsInWidth()) + dwCurrX].fIsPassable &&
		    m_pWeightedMap[(dwCurrY * CellsInWidth()) + dwCurrX].dwWeight < dwWeight)
		{
			// DEBUG!
			if (dwWeight == LOW_PRIOR_WEIGHT)
			{
				ColorCell((dwCurrY * CellsInWidth()) + dwCurrX, 142, 142, 142); // Debug, color grey
			}
			else if (dwWeight == MED_PRIOR_WEIGHT)
			{
				ColorCell((dwCurrY * CellsInWidth()) + dwCurrX, 198, 198, 198); // Debug, color grey
			}

			m_pWeightedMap[(dwCurrY * CellsInWidth()) + dwCurrX].dwWeight = dwWeight;
		}

		dwCurrX++;

		if (dwCurrX == CellsInWidth() || dwCurrX == dwX + dwRightLimit + 1)
		{
			dwCurrX = dwX - dwLeftLimit;
			dwCurrY++;
		}
	}
}

void CMap::CoatObstacleWeights()
{
	printf("Map has %u x %u cells (cell size %u x %u).\n", CellsInWidth(), CellsInHeight(), (dword)PixelPerCell(), (dword)PixelPerCell());

	// For each cell determine if passable or not!
	for (dword dwCurrCell = 0; dwCurrCell < CellsInHeight() * CellsInWidth(); dwCurrCell++)
	{
//		dword dwPixelOffset = GetCellStartingPixelByCellOffset(dwCurrCell);
		
		if (!m_pWeightedMap[dwCurrCell].fIsPassable)
		{
			CoatObstacle(dwCurrCell, WEIGHT_RADIUS_MED, MED_PRIOR_WEIGHT);
			CoatObstacle(dwCurrCell, WEIGHT_RADIUS_LOW, LOW_PRIOR_WEIGHT);
		}
	}
}

bool CMap::LoadMap(const char* szMapPath)
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

void CMap::DumpMap(SPNGCell* pMap, dword dwWidth, dword dwHeight, const char* szFilename)
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

void CMap::DumpMap(vector<SPNGCell>& map, dword dwWidth, dword dwHeight, const char* szFilename)
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

const double CMap::PixelPerCell() {
	const double fPixelsPerCell = ((double)m_dwGridResolution / (double)m_dwPixelResolution);
	return fPixelsPerCell;
}

const dword CMap::CellsInWidth() {
	const dword dwCellsInWidth = ((double)m_dwMapWidth) / PixelPerCell();
	return dwCellsInWidth;
}

const dword CMap::CellsInHeight() {
	const dword dwCellsInHeight = ((double)m_dwMapHeight) / PixelPerCell();
	return dwCellsInHeight;
}

dword CMap::GetCellOffsetByCoord(dword dX, dword dY) {
	return (dY * CellsInWidth()) + dX;
}

SMapCell* CMap::GetMapCell(dword dX, dword dY)
{
	if (!m_fIsMapLoaded)
		return NULL;

	if (dX >= m_dwMapWidth || dY >= m_dwMapHeight)
		return NULL;

	return &(m_pWeightedMap[GetCellOffsetByCoord(dX, dY)]);
}

void CMap::ColorCellByCoord(dword dX, dword dY, byte R, byte G, byte B) {
	ColorCell(GetCellOffsetByCoord(dX, dY), R, G, B);
}

void CMap::DumpMap(const char* szFilename) {
	DumpMap(m_pRawMap, m_dwMapWidth, m_dwMapHeight, szFilename);
}

SPosition CMap::PixelCoordToCellPosition(double fX, double fY) {
	dword dwCellX = (dword)floor(fX / PixelPerCell());
        dword dwCellY = (dword)floor(fY / PixelPerCell());
	return SPosition(dwCellX, dwCellY);
}

dword CMap::GetMapWidth() {
    return m_dwMapWidth;
}

dword CMap::GetMapHeight() {
    return m_dwMapHeight;
}

void CMap::DrawMapState(SDL2Wrapper* sdl, bool flushImmediately) {
    
    for (dword dwY = 0; dwY < m_dwMapHeight; dwY++) {
        for (dword dwX = 0; dwX < m_dwMapWidth; dwX++) {
            DrawPixel(sdl, dwX, dwY, m_pRawMap[dwY * m_dwMapWidth + dwX]);
        }
    }
    
    if (flushImmediately) {
        sdl->FlushChanges();
    }
}

void CMap::DrawPixel(SDL2Wrapper* sdl, dword dwX, dword dwY, SPNGCell pixel) {
    sdl->DrawPoint(dwX, dwY, pixel.bR, pixel.bG, pixel.bB, pixel.bAlpha);
}

void CMap::FillMapCellByCoord(SDL2Wrapper* sdl, dword dX, dword dY, byte R, byte G, byte B) {
    FillMapCellByCellOffset(sdl, GetCellOffsetByCoord(dX, dY), R, G, B);
}

void CMap::FillMapCellByCellOffset(SDL2Wrapper* sdl, dword dwCellOffset, byte R, byte G, byte B)
{
    dword dwPixelOffset = GetCellStartingPixelByCellOffset(dwCellOffset);

    dword dwX = dwPixelOffset % m_dwMapWidth;
    dword dwY = (dwPixelOffset / m_dwMapWidth);
    
    sdl->FillRectangle(dwX, dwY, PixelPerCell(), R, G, B, 255, false);
}

double CMap::GetPixelResolution() {
	return m_dwPixelResolution;
}

double CMap::GetGridResolution() {
	return m_dwGridResolution;
}
