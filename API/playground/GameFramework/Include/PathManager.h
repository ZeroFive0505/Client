#pragma once

#include "GameInfo.h"

// ����� ������ ��� ����ü
struct PathInfo
{
	// TCHAR : �����ڵ� ���ڿ��� �����ϱ� ���� Ÿ���̴�.
	// ��, ������Ʈ�� Multibyte ���ڿ��� ����Ѵٸ� char Ÿ������ ���ǵȴ�.
	// �����ڵ��� ��� wchar_t �� ���ǵȴ�.
	TCHAR	Path[MAX_PATH];
	char	PathMultibyte[MAX_PATH];

	PathInfo() :
		Path{},
		PathMultibyte{}
	{
	}
};

/// <summary>
/// ��� ������ Ŭ�����̴�. ������ Ŭ�����̹Ƿ� ���� �Ŵ��� �ʱ�ȭ�ÿ� ���� �ʱ�ȭ�ǰ� ���ӸŴ��� �Ҹ�ÿ���
/// ���� �Ҹ��Ѵ�.
/// </summary>
class CPathManager
{
private:
	CPathManager();
	~CPathManager();

private:
	// Ű�� ����� �̸� ��) Player, Enemy.. 
	std::unordered_map<std::string, PathInfo*>	m_mapPath;

public:
	bool Init();
	bool AddPath(const std::string& Name, const TCHAR* Path,
		const std::string& BaseName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& Name);

private:
	static CPathManager* m_Inst;

public:
	static CPathManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CPathManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

