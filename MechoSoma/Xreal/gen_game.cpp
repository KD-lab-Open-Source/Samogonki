//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript definition
//	Section: GamePrm
//	Number of variables: 9
//	This is computer generated code, don't try to change it
//////////////////////////////////////////////////////////////////////////////////////////////
float Forest5_water_damage = 8.f;
char const* bonus_men_cfg = "3 2 2 1";
float mechos_speed_increment_by_bonus_factor = 1.2f;
float mechos_max_bonus_to_take = 10.f;
float MountainBossMaxTraction = 1.2f;
float WaterBossMaxTraction = 1.07f;
float ForestBossMaxTraction = 1.f;
float LavaBossMaxTraction = 1.4f;
float WatchBossMaxTraction = 1.6f;

#ifdef _PRM_EDIT_
struct GamePrm_ParameterSection : ParameterSection
{
	GamePrm_ParameterSection() : ParameterSection("GamePrm")
{
	add_dependency("Scripts/Xreal/Game.prm");
	sourceCRC = 1915472593;
	description = 1908481757;
	reserve(9);
	add(&Forest5_water_damage, "Forest5_water_damage");
	add(&bonus_men_cfg, "bonus_men_cfg");
	add(&mechos_speed_increment_by_bonus_factor, "mechos_speed_increment_by_bonus_factor");
	add(&mechos_max_bonus_to_take, "mechos_max_bonus_to_take");
	add(&MountainBossMaxTraction, "MountainBossMaxTraction");
	add(&WaterBossMaxTraction, "WaterBossMaxTraction");
	add(&ForestBossMaxTraction, "ForestBossMaxTraction");
	add(&LavaBossMaxTraction, "LavaBossMaxTraction");
	add(&WatchBossMaxTraction, "WatchBossMaxTraction");
	add_parameter_section(this);
}
};
static GamePrm_ParameterSection GamePrm_ParameterSectionObject;
#endif  //  _PRM_EDIT_
//////////////////////////////////////////////////////////////////////////////////////////////
//	XScript end: GamePrm
//////////////////////////////////////////////////////////////////////////////////////////////
