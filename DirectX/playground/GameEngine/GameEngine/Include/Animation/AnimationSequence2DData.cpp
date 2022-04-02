#include "AnimationSequence2DData.h"
#include "../Resource/Animation/AnimationSequence2D.h"

CAnimationSequence2DData::CAnimationSequence2DData() :
	m_Frame(0),
	m_Time(0.0f),
	m_FrameTime(0.0f),
	m_PlayTime(0.0f),
	m_PlayScale(1.0f),
	m_Loop(false),
	m_Reverse(false),
	m_IsMenual(false),
	m_Start(false)
{
}

CAnimationSequence2DData::~CAnimationSequence2DData()
{
	auto iter = m_vecNotify.begin();
	auto iterEnd = m_vecNotify.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE((*iter));
	}
}

void CAnimationSequence2DData::Save(FILE* pFile)
{
	int length = (int)m_Name.length();
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), length, pFile);

	fwrite(&m_Frame, sizeof(int), 1, pFile);
	fwrite(&m_Time, sizeof(float), 1, pFile);
	fwrite(&m_FrameTime, sizeof(float), 1, pFile);
	fwrite(&m_PlayTime, sizeof(float), 1, pFile);
	fwrite(&m_PlayScale, sizeof(float), 1, pFile);

	fwrite(&m_Loop, sizeof(bool), 1, pFile);
	fwrite(&m_Reverse, sizeof(bool), 1, pFile);

	std::string sequenceName = m_Sequence->GetName();

	length = (int)sequenceName.length();
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(sequenceName.c_str(), sizeof(char), length, pFile);
}

void CAnimationSequence2DData::Load(FILE* pFile)
{
	int length = 0;
	char name[256] = {};

	fread(&length, sizeof(int), 1, pFile);
	fread(name, sizeof(char), length, pFile);
	m_Name = name;

	fread(&m_Frame, sizeof(int), 1, pFile);
	fread(&m_Time, sizeof(float), 1, pFile);
	fread(&m_FrameTime, sizeof(float), 1, pFile);
	fread(&m_PlayTime, sizeof(float), 1, pFile);
	fread(&m_PlayScale, sizeof(float), 1, pFile);

	fread(&m_Loop, sizeof(bool), 1, pFile);
	fread(&m_Reverse, sizeof(bool), 1, pFile);

	length = 0;
	char sequenceName[256] = {};
	fread(&length, sizeof(int), 1, pFile);
	fread(sequenceName, sizeof(char), length, pFile);

	m_SequenceName = sequenceName;
}

void CAnimationSequence2DData::SetCurrentFrame(int frame)
{
	if (m_Sequence->GetFrameCount() == 0 || frame < 0 || frame >= m_Sequence->GetFrameCount())
		return;

	m_Frame = frame;
}
