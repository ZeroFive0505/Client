#include "RCGGameManager.h"

CRCGGameManager::CRCGGameManager() :
	m_Event(nullptr)
{
}

CRCGGameManager::~CRCGGameManager()
{
	m_Enemies.clear();
	m_Event = nullptr;
}

void CRCGGameManager::LoadSounds()
{
	CResourceManager::GetInst()->LoadSound("BGM", true, "Detention", "RCG/Detention.mp3");
	CResourceManager::GetInst()->LoadSound("BGM", true, "Boss", "RCG/Boss.mp3");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Boss_Intro", "RCG/Sound Effects/global/boss_intro_placard.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Boss_finalhit", "RCG/Sound Effects/global/boss_finalhit.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "GameOver", "RCG/Sound Effects/interface_game_over.wav");

	LoadKyokoSounds();
	LoadMisuzuSounds();
	LoadSFX();
}

void CRCGGameManager::LoadKyokoSounds()
{
	CResourceManager::GetInst()->LoadSound("SFX", false, "Acrocircus", "RCG/Kyoko/player_kyoko_acrocircus.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Airstep", "RCG/Kyoko/player_kyoko_air_step.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Axekick", "RCG/Kyoko/player_kyoko_axe_kick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Backelbow", "RCG/Kyoko/player_kyoko_back_elbow.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Backkick", "RCG/Kyoko/player_kyoko_back_kick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Battlestart", "RCG/Kyoko/player_kyoko_battlestart.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Buttbump", "RCG/Kyoko/player_kyoko_butt_bump.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Cheerdrill", "RCG/Kyoko/player_kyoko_cheer_drill.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Chop", "RCG/Kyoko/player_kyoko_chop.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Crescentkick", "RCG/Kyoko/player_kyoko_crescent_kick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Dab", "RCG/Kyoko/player_kyoko_dab_slam.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Defeeated", "RCG/Kyoko/player_kyoko_defeated.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Dive", "RCG/Kyoko/player_kyoko_dive_intro.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Diveslide", "RCG/Kyoko/player_kyoko_dive_slide.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Donkeykick", "RCG/Kyoko/player_kyoko_donkey_kick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Dragonfeet", "RCG/Kyoko/player_kyoko_dragon_feet.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Dropkick", "RCG/Kyoko/player_kyoko_dropkick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Franken", "RCG/Kyoko/player_kyoko_frankensteiner.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Hookkick", "RCG/Kyoko/player_kyoko_hook_kick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Hurricanekick", "RCG/Kyoko/player_kyoko_hurricane_kick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Kicktoss", "RCG/Kyoko/player_kyoko_kicktoss.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Pogokick", "RCG/Kyoko/player_kyoko_pogokick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Snapkick", "RCG/Kyoko/player_kyoko_snapkick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Spinkick", "RCG/Kyoko/player_kyoko_spinkick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Stomp", "RCG/Kyoko/player_kyoko_stomp.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Volleyset", "RCG/Kyoko/player_kyoko_volleyset.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Volleyspike", "RCG/Kyoko/player_kyoko_volleyspike.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoCombo1", "RCG/Kyoko/vo_kyoko_combo_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoCombo2", "RCG/Kyoko/vo_kyoko_combo_05.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoCombo3", "RCG/Kyoko/vo_kyoko_combo_06.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoCombo4", "RCG/Kyoko/vo_kyoko_combo_07.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoCombo5", "RCG/Kyoko/vo_kyoko_combo_09.wav");
	
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEating17", "RCG/Kyoko/vo_kyoko_eating_17.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEating18", "RCG/Kyoko/vo_kyoko_eating_18.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEating19", "RCG/Kyoko/vo_kyoko_eating_19.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort1", "RCG/Kyoko/vo_kyoko_effort_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort2", "RCG/Kyoko/vo_kyoko_effort_03.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort3", "RCG/Kyoko/vo_kyoko_effort_04.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort4", "RCG/Kyoko/vo_kyoko_effort_07.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort5", "RCG/Kyoko/vo_kyoko_effort_08.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort6", "RCG/Kyoko/vo_kyoko_effort_15.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort7", "RCG/Kyoko/vo_kyoko_effort_16.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort8", "RCG/Kyoko/vo_kyoko_effort_18.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort9", "RCG/Kyoko/vo_kyoko_effort_22.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort10", "RCG/Kyoko/vo_kyoko_effort_26.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort11", "RCG/Kyoko/vo_kyoko_effort_35.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort12", "RCG/Kyoko/vo_kyoko_effort_36.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort13", "RCG/Kyoko/vo_kyoko_effort_37.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort14", "RCG/Kyoko/vo_kyoko_effort_38.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort15", "RCG/Kyoko/vo_kyoko_effort_39.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort16", "RCG/Kyoko/vo_kyoko_effort_40.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort17", "RCG/Kyoko/vo_kyoko_effort_44.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort18", "RCG/Kyoko/vo_kyoko_effort_45.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort19", "RCG/Kyoko/vo_kyoko_effort_46.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort20", "RCG/Kyoko/vo_kyoko_effort_50.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort_buildup1", "RCG/Kyoko/vo_kyoko_effort_buildup_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort_buildup2", "RCG/Kyoko/vo_kyoko_effort_buildup_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoEffort_buildup3", "RCG/Kyoko/vo_kyoko_effort_buildup_03.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoFrustrated1", "RCG/Kyoko/vo_kyoko_frustrated_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoFrustrated3", "RCG/Kyoko/vo_kyoko_frustrated_03.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit1", "RCG/Kyoko/vo_kyoko_gethit_05.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit2", "RCG/Kyoko/vo_kyoko_gethit_09.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit3", "RCG/Kyoko/vo_kyoko_gethit_10.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit4", "RCG/Kyoko/vo_kyoko_gethit_17.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit5", "RCG/Kyoko/vo_kyoko_gethit_22.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit6", "RCG/Kyoko/vo_kyoko_gethit_23.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit7", "RCG/Kyoko/vo_kyoko_gethit_24.wav");
	
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit_light1", "RCG/Kyoko/vo_kyoko_gethit_light_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit_light2", "RCG/Kyoko/vo_kyoko_gethit_light_04.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit_pain1", "RCG/Kyoko/vo_kyoko_gethit_pain_03.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGethit_pain2", "RCG/Kyoko/vo_kyoko_gethit_pain_35.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGiggle8", "RCG/Kyoko/vo_kyoko_giggle_08.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoGiggle11", "RCG/Kyoko/vo_kyoko_giggle_11.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "KyokoTired1", "RCG/Kyoko/vo_kyoko_tired_01.wav");
}

void CRCGGameManager::LoadMisuzuSounds()
{
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_block", "RCG/Misuzu/boss_misuzu_block.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_burst_through", "RCG/Misuzu/boss_misuzu_burst_through_wall.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_elbow_bonk", "RCG/Misuzu/boss_misuzu_elbow_bonk.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_fury", "RCG/Misuzu/boss_misuzu_fury.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_fury_projectile", "RCG/Misuzu/boss_misuzu_fury_projectile_impact");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_meteor_drop_impact", "RCG/Misuzu/boss_misuzu_meteor_drop_impact.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_meteor_drop_jump", "RCG/Misuzu/boss_misuzu_meteor_drop_jump2.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_meteor_drop_warning", "RCG/Misuzu/boss_misuzu_meteor_drop_warning.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_tackle_begin", "RCG/Misuzu/boss_misuzu_shoulder_tackle_begin.wav");
	CResourceManager::GetInst()->LoadSound("SFX", true, "Misuzu_tackle_step", "RCG/Misuzu/boss_misuzu_shoulder_tackle_footstep.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_tackle_pilar", "RCG/Misuzu/boss_misuzu_shoulder_tackle_impact_pillar.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_slap", "RCG/Misuzu/boss_misuzu_slap.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_slap_impact", "RCG/Misuzu/boss_misuzu_slap_impact.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_windup_punch", "RCG/Misuzu/boss_misuzu_windup_punch.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_taunt1", "RCG/Misuzu/Boss_taunt_Misuzu_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_taunt2", "RCG/Misuzu/Boss_taunt_Misuzu_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_taunt3", "RCG/Misuzu/Boss_taunt_Misuzu_03.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_taunt4", "RCG/Misuzu/Boss_taunt_Misuzu_04.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_angry", "RCG/Misuzu/vo_misuzu_angry_17.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_big_effort1", "RCG/Misuzu/vo_misuzu_big_effort_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_big_effort2", "RCG/Misuzu/vo_misuzu_big_effort_02.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_die", "RCG/Misuzu/vo_misuzu_die_08.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_effort1", "RCG/Misuzu/vo_misuzu_effort_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_effort2", "RCG/Misuzu/vo_misuzu_effort_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_effort3", "RCG/Misuzu/vo_misuzu_effort_03.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_effort4", "RCG/Misuzu/vo_misuzu_effort_26.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_effort_light2", "RCG/Misuzu/vo_misuzu_effort_light_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_effort_throw2", "RCG/Misuzu/vo_misuzu_effort_throw_02.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_frustrated6", "RCG/Misuzu/vo_misuzu_frustrated_06.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_gethit1", "RCG/Misuzu/vo_misuzu_gethit_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_gethit2", "RCG/Misuzu/vo_misuzu_gethit_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_gethit3", "RCG/Misuzu/vo_misuzu_gethit_03.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_gethit4", "RCG/Misuzu/vo_misuzu_gethit_04.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_gethit5", "RCG/Misuzu/vo_misuzu_gethit_07.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_gethit6", "RCG/Misuzu/vo_misuzu_gethit_09.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_gethit_heavy1", "RCG/Misuzu/vo_misuzu_gethit_heavy_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_gethit_heavy2", "RCG/Misuzu/vo_misuzu_gethit_heavy_03.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_land1", "RCG/Misuzu/vo_misuzu_land_01.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_laugh2", "RCG/Misuzu/vo_misuzu_laugh_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_laugh4", "RCG/Misuzu/vo_misuzu_laugh_04.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_laugh5", "RCG/Misuzu/vo_misuzu_laugh_05.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_laugh6", "RCG/Misuzu/vo_misuzu_laugh_06.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_laugh7", "RCG/Misuzu/vo_misuzu_laugh_07.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Misuzu_slam", "RCG/Misuzu/vo_misuzu_slam_01.wav");
}

void CRCGGameManager::LoadSFX()
{
	CResourceManager::GetInst()->LoadSound("SFX", false, "Knockdown", "RCG/Sound Effects/Player/gethit_knockdown_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Block", "RCG/Sound Effects/Player/player_block.wav");
	
	CResourceManager::GetInst()->LoadSound("SFX", false, "Run_footstep1", "RCG/Sound Effects/Player/player_footsteps_run_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Run_footstep2", "RCG/Sound Effects/Player/player_footsteps_run_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Run_footstep3", "RCG/Sound Effects/Player/player_footsteps_run_03.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Block2", "RCG/Sound Effects/Player/player_gethit_blocked.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Dizzy", "RCG/Sound Effects/Player/player_global_dizzy.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Parry", "RCG/Sound Effects/Player/player_global_parry.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Grab_enemy", "RCG/Sound Effects/Player/player_grab.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Grab_wall", "RCG/Sound Effects/Player/player_grab_wall.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Jump", "RCG/Sound Effects/Player/player_jump.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Jump_kick", "RCG/Sound Effects/Player/player_jump_kick.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Jump_punch", "RCG/Sound Effects/Player/player_jump_punch.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Land", "RCG/Sound Effects/Player/player_land.wav");

	CResourceManager::GetInst()->LoadSound("SFX", false, "Punch1", "RCG/Sound Effects/Player/player_punch_01.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Punch2", "RCG/Sound Effects/Player/player_punch_02.wav");
	CResourceManager::GetInst()->LoadSound("SFX", false, "Punch3", "RCG/Sound Effects/Player/player_punch_03.wav");
}
