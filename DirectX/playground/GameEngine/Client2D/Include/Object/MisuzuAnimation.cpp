#include "MisuzuAnimation.h"

CMisuzuAnimation::CMisuzuAnimation()
{
	SetTypeID<CMisuzuAnimation>();
}

CMisuzuAnimation::CMisuzuAnimation(const CMisuzuAnimation& anim) :
	CAnimationSequence2DInstance(anim)
{
}

CMisuzuAnimation::~CMisuzuAnimation()
{
}

bool CMisuzuAnimation::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("Misuzu\\sequences\\idle.sqc"), ANIMATION_PATH, "Idle", true, 0.8f);
	AddAnimation(TEXT("Misuzu\\sequences\\block.sqc"), ANIMATION_PATH, "Block", true);
	AddAnimation(TEXT("Misuzu\\sequences\\block_end.sqc"), ANIMATION_PATH, "Block_end", false, 0.1f);
	AddAnimation(TEXT("Misuzu\\sequences\\block_success.sqc"), ANIMATION_PATH, "Block_success", false, 0.1f);
	AddAnimation(TEXT("Misuzu\\sequences\\blownback.sqc"), ANIMATION_PATH, "Blownback", true, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\blownback_midair.sqc"), ANIMATION_PATH, "Blownback_midair", false, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\knockdown.sqc"), ANIMATION_PATH, "Knockdown", true, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\knockdown_midair.sqc"), ANIMATION_PATH, "Knockdown_midair", false, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\knockdown_fall.sqc"), ANIMATION_PATH, "Knockdown_fall", true, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\knockdown_ground.sqc"), ANIMATION_PATH, "Knockdown_ground", false, 0.7f);
	AddAnimation(TEXT("Misuzu\\sequences\\getup.sqc"), ANIMATION_PATH, "Getup", false);
	AddAnimation(TEXT("Misuzu\\sequences\\groundhit.sqc"), ANIMATION_PATH, "Groundhit", false, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\down.sqc"), ANIMATION_PATH, "Down", true);
	AddAnimation(TEXT("Misuzu\\sequences\\gethit.sqc"), ANIMATION_PATH, "Hit1", false, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\gethit2.sqc"), ANIMATION_PATH, "Hit2", false, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\gethit3.sqc"), ANIMATION_PATH, "Hit3", false, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\dizzy.sqc"), ANIMATION_PATH, "Stunned", true, 0.7f);
	AddAnimation(TEXT("Misuzu\\sequences\\defeat.sqc"), ANIMATION_PATH, "Defeat", false, 0.7f);
	AddAnimation(TEXT("Misuzu\\sequences\\defeat_loop.sqc"), ANIMATION_PATH, "Defeat_loop", true, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\elbow.sqc"), ANIMATION_PATH, "Elbow", false, 0.55f);
	AddAnimation(TEXT("Misuzu\\sequences\\getup_roar.sqc"), ANIMATION_PATH, "Getup_roar", false, 1.0f);
	AddAnimation(TEXT("Misuzu\\sequences\\meteor_fall.sqc"), ANIMATION_PATH, "Meteor_fall", true, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\meteor_jump.sqc"), ANIMATION_PATH, "Meteor_jump", true, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\meteor_jumpup.sqc"), ANIMATION_PATH, "Meteor_jumpup", false, 1.0f);
	AddAnimation(TEXT("Misuzu\\sequences\\meteor_landhit.sqc"), ANIMATION_PATH, "Meteor_landhit", false, 1.0f);
	AddAnimation(TEXT("Misuzu\\sequences\\meteor_landhit.sqc"), ANIMATION_PATH, "Meteor_again", false, 1.0f);
	AddAnimation(TEXT("Misuzu\\sequences\\meteor_landmiss.sqc"), ANIMATION_PATH, "Meteor_landmiss", false, 1.0f);
	AddAnimation(TEXT("Misuzu\\sequences\\roar.sqc"), ANIMATION_PATH, "Roar", false, 1.2f);
	AddAnimation(TEXT("Misuzu\\sequences\\rubbutt.sqc"), ANIMATION_PATH, "Rubbutt", true, 0.8f);
	AddAnimation(TEXT("Misuzu\\sequences\\rubbutt_end.sqc"), ANIMATION_PATH, "Rubbutt_end", false, 0.2f);
	AddAnimation(TEXT("Misuzu\\sequences\\slap.sqc"), ANIMATION_PATH, "Slap", false, 0.55f);
	AddAnimation(TEXT("Misuzu\\sequences\\tackle_loop.sqc"), ANIMATION_PATH, "Tackle_loop", true, 0.5f);
	AddAnimation(TEXT("Misuzu\\sequences\\tackle_start.sqc"), ANIMATION_PATH, "Tackle_start", false, 0.7f);
	AddAnimation(TEXT("Misuzu\\sequences\\tackle_turn.sqc"), ANIMATION_PATH, "Tackle_turn", false, 0.25f);
	AddAnimation(TEXT("Misuzu\\sequences\\taunt.sqc"), ANIMATION_PATH, "Taunt", false);
	AddAnimation(TEXT("Misuzu\\sequences\\walk.sqc"), ANIMATION_PATH, "Walk", true, 0.7f);
	AddAnimation(TEXT("Misuzu\\sequences\\WUpunch.sqc"), ANIMATION_PATH, "WUPunch", false, 1.5f);

	CAnimationSequence2DInstance::Play();

	return true;
}

CMisuzuAnimation* CMisuzuAnimation::Clone()
{
	return nullptr;
}
