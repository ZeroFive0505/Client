#pragma once

#include "ConstantBufferBase.h"

class CTileMapConstantBuffer :
	public CConstantBufferBase
{
public:
	CTileMapConstantBuffer();
	CTileMapConstantBuffer(const CTileMapConstantBuffer& buffer);
	virtual ~CTileMapConstantBuffer();

protected:
	sTileMapCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CTileMapConstantBuffer* Clone();

public:
	inline void SetImageSize(const Vector2& imageSize)
	{
		m_BufferData.imageSize = imageSize;
	}

	inline void SetTileSize(const Vector2& tileSize)
	{
		m_BufferData.size = tileSize;
	}

	inline void SetImageStart(const Vector2& start)
	{
		m_BufferData.start = start;
	}

	inline void SetImageEnd(const Vector2& end)
	{
		m_BufferData.end = end;
	}

	inline void SetWVP(const Matrix& matWVP)
	{
		m_BufferData.matWVP = matWVP;
	}

	inline void SetWire(bool wire)
	{
		m_BufferData.wire = wire ? 1 : 0;
	}
};

