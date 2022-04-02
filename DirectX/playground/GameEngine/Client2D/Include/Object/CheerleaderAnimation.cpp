#include "CheerleaderAnimation.h"

CCheerleaderAnimation::CCheerleaderAnimation()
{
    SetTypeID<CCheerleaderAnimation>();
}

CCheerleaderAnimation::CCheerleaderAnimation(const CCheerleaderAnimation& anim) :
    CAnimationSequence2DInstance(anim)
{
}

CCheerleaderAnimation::~CCheerleaderAnimation()
{
}

bool CCheerleaderAnimation::Init()
{
    if (!CAnimationSequence2DInstance::Init())
        return false;

    CAnimationSequence2DInstance::Stop();

    AddAnimation(TEXT("Cheerleader\\sequences\\alert.sqc"), ANIMATION_PATH, "Alert", false, 0.35f);
    AddAnimation(TEXT("Cheerleader\\sequences\\idle.sqc"), ANIMATION_PATH, "Idle", true);
    AddAnimation(TEXT("Cheerleader\\sequences\\begging.sqc"), ANIMATION_PATH, "Begging", true, 0.2f);
    AddAnimation(TEXT("Cheerleader\\sequences\\blownback.sqc"), ANIMATION_PATH, "Blownback", true, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\blownback_midair.sqc"), ANIMATION_PATH, "Blownback_midair", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\cartwheel.sqc"), ANIMATION_PATH, "Cartwheel", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\divekick_start.sqc"), ANIMATION_PATH, "Divekick_start", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\divekick_middle.sqc"), ANIMATION_PATH, "Divekick_middle", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\divekick_end.sqc"), ANIMATION_PATH, "Divekick_end", false);
    AddAnimation(TEXT("Cheerleader\\sequences\\doublepom.sqc"), ANIMATION_PATH, "Doublepom", false, 0.8f);
    AddAnimation(TEXT("Cheerleader\\sequences\\down.sqc"), ANIMATION_PATH, "Down", true);
    AddAnimation(TEXT("Cheerleader\\sequences\\gethits.sqc"), ANIMATION_PATH, "Hit1", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\gethits2.sqc"), ANIMATION_PATH, "Hit2", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\gethits3.sqc"), ANIMATION_PATH, "Hit3", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\getup.sqc"), ANIMATION_PATH, "Getup", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\backfilp.sqc"), ANIMATION_PATH, "Backflip", false);
    AddAnimation(TEXT("Cheerleader\\sequences\\grabbed.sqc"), ANIMATION_PATH, "Grabbed", true);
    AddAnimation(TEXT("Cheerleader\\sequences\\grab_release.sqc"), ANIMATION_PATH, "Heldrelease", false, 0.7f);
    AddAnimation(TEXT("Cheerleader\\sequences\\grabhit.sqc"), ANIMATION_PATH, "Heldhit", false, 0.2f);
    AddAnimation(TEXT("Cheerleader\\sequences\\jump.sqc"), ANIMATION_PATH, "Jump", false, 1.0f, 1.0f, false, true);
    AddAnimation(TEXT("Cheerleader\\sequences\\groundhit.sqc"), ANIMATION_PATH, "Groundhit", false, 0.2f);
    AddAnimation(TEXT("Cheerleader\\sequences\\knockdown.sqc"), ANIMATION_PATH, "Knockdown", true, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\blocks.sqc"), ANIMATION_PATH, "Block", true);
    AddAnimation(TEXT("Cheerleader\\sequences\\block_success.sqc"), ANIMATION_PATH, "Block_success", false, 0.1f);
    AddAnimation(TEXT("Cheerleader\\sequences\\block_end.sqc"), ANIMATION_PATH, "Block_end", false, 0.1f);
    AddAnimation(TEXT("Cheerleader\\sequences\\knockdown_midair.sqc"), ANIMATION_PATH, "Knockdown_midair", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\knockdown_fall.sqc"), ANIMATION_PATH, "Knockdown_fall", true, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\knockdown_ground.sqc"), ANIMATION_PATH, "Knockdown_ground", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\blownback_midair.sqc"), ANIMATION_PATH, "Blownback_midair", false, 0.5f);
    AddAnimation(TEXT("Cheerleader\\sequences\\stunned.sqc"), ANIMATION_PATH, "Stunned", true, 0.7f);
    AddAnimation(TEXT("Cheerleader\\sequences\\walk.sqc"), ANIMATION_PATH, "Walk", true, 0.7f);
    AddAnimation(TEXT("Cheerleader\\sequences\\run.sqc"), ANIMATION_PATH, "Run", true, 0.7f);
    AddAnimation(TEXT("Cheerleader\\sequences\\wallhit.sqc"), ANIMATION_PATH, "Wallhit", false, 0.25f);
    AddAnimation(TEXT("Cheerleader\\sequences\\jab.sqc"), ANIMATION_PATH, "Jab", false, 0.7f);

    return true;
}

CCheerleaderAnimation* CCheerleaderAnimation::Clone()
{
    return new CCheerleaderAnimation(*this);
}
