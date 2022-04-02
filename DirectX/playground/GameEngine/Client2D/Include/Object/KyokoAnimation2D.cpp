#include "KyokoAnimation2D.h"

CKyokoAnimation2D::CKyokoAnimation2D()
{
	SetTypeID<CKyokoAnimation2D>();
}

CKyokoAnimation2D::CKyokoAnimation2D(const CKyokoAnimation2D& anim) :
	CAnimationSequence2DInstance(anim)
{
}

CKyokoAnimation2D::~CKyokoAnimation2D()
{
}

bool CKyokoAnimation2D::Init()
{
	if (!CAnimationSequence2DInstance::Init())
		return false;

	AddAnimation(TEXT("Kyoko\\sequences\\idle.sqc"), ANIMATION_PATH, "Idle");
	AddAnimation(TEXT("Kyoko\\sequences\\walk.sqc"), ANIMATION_PATH, "Walk");
	AddAnimation(TEXT("Kyoko\\sequences\\run.sqc"), ANIMATION_PATH, "Run");
	AddAnimation(TEXT("Kyoko\\sequences\\jump.sqc"), ANIMATION_PATH, "Jump", false, 1.0f, 1.0f, false, true);
	AddAnimation(TEXT("Kyoko\\sequences\\dodge.sqc"), ANIMATION_PATH, "Dodge", false, 0.5f, 2.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\battlestart.sqc"), ANIMATION_PATH, "BattleStart", true, 0.5f);
	AddAnimation(TEXT("Kyoko\\sequences\\battlestart_ground.sqc"), ANIMATION_PATH, "BattleStartGround", false, 1.0f);

	AddAnimation(TEXT("Kyoko\\sequences\\qc1chop.sqc"), ANIMATION_PATH, "QC1_Chop", false, 0.5f);
	AddAnimation(TEXT("Kyoko\\sequences\\qc2snapkick.sqc"), ANIMATION_PATH, "QC2_Snapkick", false, 0.5f);
	AddAnimation(TEXT("Kyoko\\sequences\\qc3cresentkick.sqc"), ANIMATION_PATH, "QC3_Cresentkick", false, 0.5f);
	AddAnimation(TEXT("Kyoko\\sequences\\qc4hookkick.sqc"), ANIMATION_PATH, "QC4_Hookkick", false, 0.5f);
	AddAnimation(TEXT("Kyoko\\sequences\\qc5backkick.sqc"), ANIMATION_PATH, "QC5_Backkick", false, 0.5f);


	AddAnimation(TEXT("Kyoko\\sequences\\backelbow.sqc"), ANIMATION_PATH, "Backelbow", false, 0.5f);
	AddAnimation(TEXT("Kyoko\\sequences\\dive.sqc"), ANIMATION_PATH, "Dive", false, 0.8f);
	AddAnimation(TEXT("Kyoko\\sequences\\dropkick.sqc"), ANIMATION_PATH, "Dropkick", false, 0.8f);

	AddAnimation(TEXT("Kyoko\\sequences\\axekick.sqc"), ANIMATION_PATH, "Axekick", false, 0.8f);
	AddAnimation(TEXT("Kyoko\\sequences\\spinkick.sqc"), ANIMATION_PATH, "Spinkick", false, 0.7f);
	AddAnimation(TEXT("Kyoko\\sequences\\volleyset.sqc"), ANIMATION_PATH, "Volleyset", false, 0.7f);

	AddAnimation(TEXT("Kyoko\\sequences\\buttbump.sqc"), ANIMATION_PATH, "Buttbump", false, 0.5f);
	AddAnimation(TEXT("Kyoko\\sequences\\airstep.sqc"), ANIMATION_PATH, "Airstep", false, 0.5f);
	AddAnimation(TEXT("kyoko\\sequences\\volleyspike.sqc"), ANIMATION_PATH, "Volleyspike", false, 0.7f);

	AddAnimation(TEXT("Kyoko\\sequences\\dragon_feet.sqc"), ANIMATION_PATH, "Dragonfeet", false);

	AddAnimation(TEXT("Kyoko\\sequences\\acrocircus.sqc"), ANIMATION_PATH, "Acrocircus", false, 0.35f);
	AddAnimation(TEXT("Kyoko\\sequences\\acrocircus_loop.sqc"), ANIMATION_PATH, "Acrocircus_loop", true, 0.5f);
	AddAnimation(TEXT("Kyoko\\sequences\\acrocircus_land.sqc"), ANIMATION_PATH, "Acrocircus_land", false, 1.0f);

	AddAnimation(TEXT("Kyoko\\sequences\\battlestart.sqc"), ANIMATION_PATH, "Battle_start", false);

	AddAnimation(TEXT("Kyoko\\sequences\\climbing.sqc"), ANIMATION_PATH, "Climbing", true);
	AddAnimation(TEXT("Kyoko\\sequences\\climbing_top.sqc"), ANIMATION_PATH, "Climbingtop", false);

	AddAnimation(TEXT("Kyoko\\sequences\\defeated.sqc"), ANIMATION_PATH, "Defeated", false);
	AddAnimation(TEXT("Kyoko\\sequences\\defeatedhit.sqc"), ANIMATION_PATH, "Defeatedhit", false);

	AddAnimation(TEXT("Kyoko\\sequences\\donkeykick.sqc"), ANIMATION_PATH, "Donkeykick", false);

	AddAnimation(TEXT("Kyoko\\sequences\\ground_hit.sqc"), ANIMATION_PATH, "Groundhit", false);

	AddAnimation(TEXT("Kyoko\\sequences\\hurricane_kick.sqc"), ANIMATION_PATH, "Hurricanekick", false, 1.1f);

	AddAnimation(TEXT("Kyoko\\sequences\\newcheerdrill.sqc"), ANIMATION_PATH, "Newcheerdrill", false, 1.0f);

	AddAnimation(TEXT("Kyoko\\sequences\\cheerdrill.sqc"), ANIMATION_PATH, "Cheerdrill", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\parry.sqc"), ANIMATION_PATH, "Parry", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\stomp.sqc"), ANIMATION_PATH, "Stomp", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\stunned.sqc"), ANIMATION_PATH, "Stunned", true);

	AddAnimation(TEXT("Kyoko\\sequences\\dab.sqc"), ANIMATION_PATH, "Dab", false);

	AddAnimation(TEXT("Kyoko\\sequences\\block.sqc"), ANIMATION_PATH, "Block", true);

	AddAnimation(TEXT("Kyoko\\sequences\\donkeykick.sqc"), ANIMATION_PATH, "Donkeykick", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\block_end.sqc"), ANIMATION_PATH, "Blockend", false, 0.1f);

	AddAnimation(TEXT("Kyoko\\sequences\\block_success.sqc"), ANIMATION_PATH, "Block_success", false, 0.15f);

	AddAnimation(TEXT("Kyoko\\sequences\\grabbing.sqc"), ANIMATION_PATH, "Grabbing", true, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\grabbed.sqc"), ANIMATION_PATH, "Grabbed", true);

	AddAnimation(TEXT("Kyoko\\sequences\\grab_release.sqc"), ANIMATION_PATH, "Grabrelease", false, 0.7f);

	AddAnimation(TEXT("Kyoko\\sequences\\grabhit.sqc"), ANIMATION_PATH, "Grabhit", false);

	AddAnimation(TEXT("Kyoko\\sequences\\gutpunch.sqc"), ANIMATION_PATH, "Gutpunch", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\kicktoss.sqc"), ANIMATION_PATH, "Kicktoss", false);

	AddAnimation(TEXT("Kyoko\\sequences\\pogokick.sqc"), ANIMATION_PATH, "Pogokick", false, 1.0f);

	AddAnimation(TEXT("Kyoko\\sequences\\frankenstiner.sqc"), ANIMATION_PATH, "Franken", false, 0.8f);

	AddAnimation(TEXT("Kyoko\\sequences\\gethits.sqc"), ANIMATION_PATH, "Hit1", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\gethits2.sqc"), ANIMATION_PATH, "Hit2", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\gethits3.sqc"), ANIMATION_PATH, "Hit3", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\blowback.sqc"), ANIMATION_PATH, "Blownback", true, 0.7f);

	AddAnimation(TEXT("Kyoko\\sequences\\blowback_midair.sqc"), ANIMATION_PATH, "Blownback_midair", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\knockdown.sqc"), ANIMATION_PATH, "Knockdown", true, 0.7f);

	AddAnimation(TEXT("Kyoko\\sequences\\knockdown_midair.sqc"), ANIMATION_PATH, "Knockdown_midair", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\knockdown_fall.sqc"), ANIMATION_PATH, "Knockdown_fall", true, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\knockdown_ground.sqc"), ANIMATION_PATH, "Knockdown_ground", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\down.sqc"), ANIMATION_PATH, "Down", true);

	AddAnimation(TEXT("Kyoko\\sequences\\getup.sqc"), ANIMATION_PATH, "Getup", false, 1.0f);

	AddAnimation(TEXT("Kyoko\\sequences\\wallhit.sqc"), ANIMATION_PATH, "Wallhit", false, 0.5f);

	AddAnimation(TEXT("Kyoko\\sequences\\walljump.sqc"), ANIMATION_PATH, "Walljump", false, 0.5f);

	CAnimationSequence2DInstance::Play();

	return true;
}

CKyokoAnimation2D* CKyokoAnimation2D::Clone()
{
	return new CKyokoAnimation2D(*this);
}
