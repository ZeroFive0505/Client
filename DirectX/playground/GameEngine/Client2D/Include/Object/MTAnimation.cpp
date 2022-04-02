#include "MTAnimation.h"

CMTAnimation::CMTAnimation()
{
	SetTypeID<CMTAnimation>();
}

CMTAnimation::CMTAnimation(const CMTAnimation& anim) :
	CAnimationSequence2DInstance(anim)
{
}

CMTAnimation::~CMTAnimation()
{
}

bool CMTAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("MT\\sequences\\alert.sqc"), ANIMATION_PATH, "Alert", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\block.sqc"), ANIMATION_PATH, "Block", true);
	AddAnimation(TEXT("MT\\sequences\\block_success.sqc"), ANIMATION_PATH, "Block_success", false, 0.1f);
	AddAnimation(TEXT("MT\\sequences\\block_end.sqc"), ANIMATION_PATH, "Block_end", false, 0.1f);
	AddAnimation(TEXT("MT\\sequences\\blownback.sqc"), ANIMATION_PATH, "Blownback", true, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\blownback_midair.sqc"), ANIMATION_PATH, "Blownback_midair", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\crosspunch.sqc"), ANIMATION_PATH, "Crosspunch", false, 0.8f);
	AddAnimation(TEXT("MT\\sequences\\dodgekick.sqc"), ANIMATION_PATH, "Dodgekick", false);
	AddAnimation(TEXT("MT\\sequences\\snapkick.sqc"), ANIMATION_PATH, "Snapkick", false);
	AddAnimation(TEXT("MT\\sequences\\stunned.sqc"), ANIMATION_PATH, "Stunned", true, 0.8f);
	AddAnimation(TEXT("MT\\sequences\\down.sqc"), ANIMATION_PATH, "Down", true);
	AddAnimation(TEXT("MT\\sequences\\gethits.sqc"), ANIMATION_PATH, "Hit1", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\gethits2.sqc"), ANIMATION_PATH, "Hit2", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\gethits3.sqc"), ANIMATION_PATH, "Hit3", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\getup.sqc"), ANIMATION_PATH, "Getup", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\groundhit.sqc"), ANIMATION_PATH, "Groundhit", false, 0.2f);
	AddAnimation(TEXT("MT\\sequences\\grabbed.sqc"), ANIMATION_PATH, "Grabbed", true);
	AddAnimation(TEXT("MT\\sequences\\heldhit.sqc"), ANIMATION_PATH, "Heldhit", false, 0.2f);
	AddAnimation(TEXT("MT\\sequences\\heldrelease.sqc"), ANIMATION_PATH, "Heldrelease", false, 0.7f);
	AddAnimation(TEXT("MT\\sequences\\idle.sqc"), ANIMATION_PATH, "Idle", true, 0.7f);
	AddAnimation(TEXT("MT\\sequences\\jump.sqc"), ANIMATION_PATH, "Jump", false, 1.0f, 1.0f, false, true);
	AddAnimation(TEXT("MT\\sequences\\jumpback_kick.sqc"), ANIMATION_PATH, "Jumpbackkick", false);
	AddAnimation(TEXT("MT\\sequences\\jumpkick.sqc"), ANIMATION_PATH, "Jumpkick", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\knockdown.sqc"), ANIMATION_PATH, "Knockdown", true, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\knockdown_midair.sqc"), ANIMATION_PATH, "Knockdown_midair", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\knockdown_fall.sqc"), ANIMATION_PATH, "Knockdown_fall", true, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\knockdown_ground.sqc"), ANIMATION_PATH, "Knockdown_ground", false, 0.5f);
	AddAnimation(TEXT("MT\\sequences\\walk.sqc"), ANIMATION_PATH, "Walk", true, 0.7f);
	AddAnimation(TEXT("MT\\sequences\\run.sqc"), ANIMATION_PATH, "Run", true, 0.7f);
	AddAnimation(TEXT("MT\\sequences\\wallhit.sqc"), ANIMATION_PATH, "Wallhit", false, 0.25f);

	return true;
}

CMTAnimation* CMTAnimation::Clone()
{
	return new CMTAnimation(*this);
}
