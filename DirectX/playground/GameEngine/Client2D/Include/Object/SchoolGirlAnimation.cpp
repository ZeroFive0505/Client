#include "SchoolGirlAnimation.h"

CSchoolGirlAnimation::CSchoolGirlAnimation()
{
    SetTypeID<CSchoolGirlAnimation>();
}

CSchoolGirlAnimation::CSchoolGirlAnimation(const CSchoolGirlAnimation& anim) :
    CAnimationSequence2DInstance(anim)
{

}

CSchoolGirlAnimation::~CSchoolGirlAnimation()
{
}

bool CSchoolGirlAnimation::Init()
{
    if (!CAnimationSequence2DInstance::Init())
        return false;

    AddAnimation(TEXT("SchoolGirl\\sequences\\idle.sqc"), ANIMATION_PATH, "Idle", true);
    AddAnimation(TEXT("SchoolGirl\\sequences\\gethits.sqc"), ANIMATION_PATH, "Hit1", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\gethits2.sqc"), ANIMATION_PATH, "Hit2", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\gethits3.sqc"), ANIMATION_PATH, "Hit3", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\getup.sqc"), ANIMATION_PATH, "Getup", false);
    AddAnimation(TEXT("SchoolGirl\\sequences\\groundhit.sqc"), ANIMATION_PATH, "Groundhit", false, 0.2f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\held.sqc"), ANIMATION_PATH, "Grabbed", true);
    AddAnimation(TEXT("SchoolGirl\\sequences\\heldhit.sqc"), ANIMATION_PATH, "Heldhit", false, 0.2f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\heldrelease.sqc"), ANIMATION_PATH, "Heldrelease", false, 0.7f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\knockdown.sqc"), ANIMATION_PATH, "Knockdown", true, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\knockdown_midair.sqc"), ANIMATION_PATH, "Knockdown_midair", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\knockdown_fall.sqc"), ANIMATION_PATH, "Knockdown_fall", true, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\knockdown_ground.sqc"), ANIMATION_PATH, "Knockdown_ground", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\blownback.sqc"), ANIMATION_PATH, "Blownback", true, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\blownback_midair.sqc"), ANIMATION_PATH, "Blownback_midair", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\down.sqc"), ANIMATION_PATH, "Down", true);
    AddAnimation(TEXT("SchoolGirl\\sequences\\stunned.sqc"), ANIMATION_PATH, "Stunned", true, 0.7f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\wallhit.sqc"), ANIMATION_PATH, "Wallhit", false, 0.25f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\block.sqc"), ANIMATION_PATH, "Block", true);
    AddAnimation(TEXT("SchoolGirl\\sequences\\block_success.sqc"), ANIMATION_PATH, "Block_success", false, 0.1f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\block_end.sqc"), ANIMATION_PATH, "Block_end", false, 0.1f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\walk.sqc"), ANIMATION_PATH, "Walk", true, 0.7f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\run.sqc"), ANIMATION_PATH, "Run", true, 0.7f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\jump.sqc"), ANIMATION_PATH, "Jump", false, 1.0f, 1.0f, false, true);
    AddAnimation(TEXT("SchoolGirl\\sequences\\cresentkick.sqc"), ANIMATION_PATH, "Cresentkick", false);
    AddAnimation(TEXT("SchoolGirl\\sequences\\jab.sqc"), ANIMATION_PATH, "Jab", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\jumpknee.sqc"), ANIMATION_PATH, "Jumpknee", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\kick.sqc"), ANIMATION_PATH, "Kick", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\tornadokick_start.sqc"), ANIMATION_PATH, "Tornadokick_start", false, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\tornadokick_loop.sqc"), ANIMATION_PATH, "Tornadokick_loop", true, 0.5f);
    AddAnimation(TEXT("SchoolGirl\\sequences\\tornadokick_end.sqc"), ANIMATION_PATH, "Tornadokick_end", false, 0.3f);
    AddAnimation(TEXT("SChoolGirl\\sequences\\begging.sqc"), ANIMATION_PATH, "Begging", true, 0.2f);

    CAnimationSequence2DInstance::Play();

    return true;
}

CSchoolGirlAnimation* CSchoolGirlAnimation::Clone()
{
    return new CSchoolGirlAnimation(*this);
}
