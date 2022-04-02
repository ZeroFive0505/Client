#pragma once

#include "IMGUIWindow.h"

/// <summary>
/// 오브젝트 상세 정보를 보여주는 창
/// </summary>
class CDetailWindow :
	public CIMGUIWindow
{
public:
	CDetailWindow();
	virtual ~CDetailWindow();

private:
	class CIMGUITextInput* m_PosX;
	class CIMGUITextInput* m_PosY;
	class CIMGUITextInput* m_PosZ;

	class CIMGUITextInput* m_RotX;
	class CIMGUITextInput* m_RotY;
	class CIMGUITextInput* m_RotZ;

	class CIMGUITextInput* m_ScaleX;
	class CIMGUITextInput* m_ScaleY;
	class CIMGUITextInput* m_ScaleZ;

public:
	inline class CIMGUITextInput* GetScaleXInput() const
	{
		return m_ScaleX;
	}

	inline class CIMGUITextInput* GetScaleYInput() const
	{
		return m_ScaleY;
	}

	inline class CIMGUITextInput* GetScaleZInput() const
	{
		return m_ScaleZ;
	}

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

private:
	void PositionXCallback();
	void PositionYCallback();
	void PositionZCallbacK();

	void RotationXCallback();
	void RotationYCallback();
	void RotationZCallback();;

	void ScaleXCallback();
	void ScaleYCallback();
	void ScaleZCallback();
};

