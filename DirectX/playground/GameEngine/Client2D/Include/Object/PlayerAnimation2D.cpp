#include "PlayerAnimation2D.h"

CPlayerAnimation2D::CPlayerAnimation2D()
{
	SetTypeID<CPlayerAnimation2D>();
}

CPlayerAnimation2D::CPlayerAnimation2D(const CPlayerAnimation2D& anim) :
	CAnimationSequence2DInstance(anim)
{
}

CPlayerAnimation2D::~CPlayerAnimation2D()
{
}

bool CPlayerAnimation2D::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	// 메인씬에서 추가된 시퀀스들을 찾아오고 Idle, Walk로 등록시킨다.
	// AddAnimation("PlayerIdle", "Idle");
	// AddAnimation("PlayerWalk", "Walk");

	AddAnimation(TEXT("Kyoko\\sequences\\idle.sqc"), ANIMATION_PATH, "Idle");
	AddAnimation(TEXT("Kyoko\\sequences\\walk.sqc"), ANIMATION_PATH, "Walk");
	AddAnimation(TEXT("Kyoko\\sequences\\run.sqc"), ANIMATION_PATH, "Run");
	AddAnimation(TEXT("Kyoko\\sequences\\jump.sqc"), ANIMATION_PATH, "Jump", false, 1.0f, 1.0f, false, true);
	AddAnimation(TEXT("Kyoko\\sequences\\dodge.sqc"), ANIMATION_PATH, "Dodge", false, 0.5f, 2.5f);
	
	// AddAnimation(TEXT("SchoolBoy\\sequences\\idle.sqc"), ANIMATION_PATH, "Idle");
	// AddAnimation(TEXT("SchoolBoy\\sequences\\walk.sqc"), ANIMATION_PATH, "Walk");
	

	CAnimationSequence2DInstance::Play();

	return true;
}

CPlayerAnimation2D* CPlayerAnimation2D::Clone()
{
	return new CPlayerAnimation2D(*this);
}
