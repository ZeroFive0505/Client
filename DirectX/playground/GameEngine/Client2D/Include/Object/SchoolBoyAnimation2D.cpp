#include "SchoolBoyAnimation2D.h"

CSchoolBoyAnimation2D::CSchoolBoyAnimation2D()
{
    SetTypeID<CSchoolBoyAnimation2D>();
}

CSchoolBoyAnimation2D::CSchoolBoyAnimation2D(const CSchoolBoyAnimation2D& anim) :
    CAnimationSequence2DInstance(anim)
{
}

CSchoolBoyAnimation2D::~CSchoolBoyAnimation2D()
{
}

bool CSchoolBoyAnimation2D::Init()
{
    if (!CAnimationSequence2DInstance::Init())
        return false;

    AddAnimation(TEXT("SchoolBoy\\sequences\\idle.sqc"), ANIMATION_PATH, "Idle", true);
    AddAnimation(TEXT("SchoolBoy\\sequences\\block.sqc"), ANIMATION_PATH, "Block", true);
    AddAnimation(TEXT("SchoolBoy\\sequences\\block_success.sqc"), ANIMATION_PATH, "Block_success", false, 0.1f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\block_end.sqc"), ANIMATION_PATH, "Block_end", false, 0.1f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\gethits.sqc"), ANIMATION_PATH, "Hit1", false, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\gethits2.sqc"), ANIMATION_PATH, "Hit2", false, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\gethits3.sqc"), ANIMATION_PATH, "Hit3", false, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\knockdown.sqc"), ANIMATION_PATH, "Knockdown", true, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\knockdown_fall.sqc"), ANIMATION_PATH, "Knockdown_fall", true, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\knockdown_ground.sqc"), ANIMATION_PATH, "Knockdown_ground", false, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\knockdown_midair.sqc"), ANIMATION_PATH, "Knockdown_midair", false, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\blownback.sqc"), ANIMATION_PATH, "Blownback", true, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\blownback_midair.sqc"), ANIMATION_PATH, "Blownback_midair", false, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\getup.sqc"), ANIMATION_PATH, "Getup", false, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\groundhit.sqc"), ANIMATION_PATH, "Groundhit", false, 0.5f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\down.sqc"), ANIMATION_PATH, "Down", true);
    AddAnimation(TEXT("SchoolBoy\\sequences\\grabbed.sqc"), ANIMATION_PATH, "Grabbed", true);
    AddAnimation(TEXT("SchoolBoy\\sequences\\heldhit.sqc"), ANIMATION_PATH, "Heldhit", false, 0.2f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\heldrelease.sqc"), ANIMATION_PATH, "Heldrelease", false, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\stunned.sqc"), ANIMATION_PATH, "Stunned", true, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\wallhit.sqc"), ANIMATION_PATH, "Wallhit", false, 0.25f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\axekick.sqc"), ANIMATION_PATH, "Axekick", false, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\elbow_uppercut.sqc"), ANIMATION_PATH, "Elbowupper", false, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\jump.sqc"), ANIMATION_PATH, "Jump", false, 1.0f, 1.0f, false, true);
    AddAnimation(TEXT("SchoolBoy\\sequences\\jump_punch.sqc"), ANIMATION_PATH, "Jumppunch", false, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\rightpunch.sqc"), ANIMATION_PATH, "Rightstraight", false, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\sandtoss.sqc"), ANIMATION_PATH, "Sandtoss", false, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\sidekick.sqc"), ANIMATION_PATH, "Sidekick", false, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\walk.sqc"), ANIMATION_PATH, "Walk", true, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\run.sqc"), ANIMATION_PATH, "Run", true, 0.7f);
    AddAnimation(TEXT("SchoolBoy\\sequences\\begging.sqc"), ANIMATION_PATH, "Begging", true, 0.2f);


    CAnimationSequence2DInstance::Play();

    return true;
}

CSchoolBoyAnimation2D* CSchoolBoyAnimation2D::Clone()
{
    return new CSchoolBoyAnimation2D(*this);
}
