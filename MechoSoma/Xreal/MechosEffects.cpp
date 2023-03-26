
#include "StdAfx.h"
#include "Mechos.h"
#include "MechosPrm.h"
#include "sound.h"
#include "Mesh3ds.h"

///////////////////////////////////////////////////////////////////////////////////////////
//			Effects
///////////////////////////////////////////////////////////////////////////////////////////
EffectData Mechos::effect_data[EffectsMax] = 
{
	EffectData(0, "main", EffectData::Cycled), 
	EffectData(1000, "no", EffectData::Forward, EFF_ANIMATION_1_BACK), // "HugeAcceleration", // Сильное ускорение после наезда на специальные споты: одноразовая анимация.
	EffectData(1000, "no", EffectData::Forward), // "Landing", // Приземление после долгого полета: одноразовая анимация.
	EffectData(1000, "no", EffectData::Cycled), // "SpringBoardFly", // Свободный продолжительный полет после трамплина: зацикленная анимация.
	EffectData(main_period, "main", EffectData::Forward), // "Resting", // Cпокойное состоянии: периодически возникающая анимация.
	EffectData(outstrip_period, "exult", EffectData::Forward, EFF_ANIMATION_1_OUTRUN), // "OutstripTheNeighbour", // Обгон соперника (суматоху на старте не учитывать): одноразовая анимация, раньше была "outstrip"
	EffectData(sadness_period, "sadness", EffectData::Forward, EFF_ANIMATION_1_OUTDRIVEN), // "OutstrippedByNeighbour", // Обгон соперником (суматоху на старте не учитывать): одноразовая анимация.
	EffectData(1000, "no", EffectData::Forward), // "Collision", // Столкновение с соперником: одноразовая анимация.
	EffectData(sadness_period, "sadness", EffectData::Forward, EFF_ANIMATION_1_SADNESS), // "InfluenceOfArcane", // Действие чужого Аркана: несколько видов одноразовой анимации.
	EffectData(exult_period, "exult", EffectData::Forward, EFF_ANIMATION_1_LUCKY), // "ExultEffect", // Ликование от попадания в соперника Арканом, успешное финиширование: одноразовая анимация.
	EffectData(sadness_period, "sadness", EffectData::Forward), // "ObstacleAnnoying", // Раздражение от столкновения с препятствием-Арканом: одноразовая анимация.
	EffectData(1000, "no", EffectData::Forward, EFF_ANIMATION_1_CHECKDROP), // "CheckPointOmission", // Пропуск чекпойнта: одноразовая анимация (оборачивается и кричит в камеру).
	EffectData(2000, "no", EffectData::Cycled), // "FlyingByArcane", // Управляемый полет с помощью Аркана: зацикленная анимация (например, машет ручками).
	EffectData(2000, "no", EffectData::Cycled), // "Swimming", // Плавание по воде: зацикленная анимация (например, переворачивается  как поплавок).
	EffectData(hit_period, "hit", EffectData::ForwardThenBackwardThenForwardThenBackward, EFF_ANIMATION_1_CRUSH, hit_delay), // "StoneHit", // Падает камень на голову
	EffectData(run_period, "run", EffectData::Cycled), // "RunToSeed", 
	EffectData(exult_period, "exult", EffectData::Forward, EFF_ANIMATION_1_BONUS) // "BonusExultEffect", // Ликование от взятия бонуса
};

EffectData::EffectData(float period, const char* channel, Mode mode_, int sound_, int delay_)
: mode(mode_),
delay(delay_),
AnimationData(period, 0, mode_ == Cycled ? -1 : 1, channel),		
sound(sound_)
{}

///////////////////////////////////////////////////////////////////////////////////////////
//		AnimationCallBackEvent
///////////////////////////////////////////////////////////////////////////////////////////
class AnimationCallBackEvent : public EffectData, public ResourceUser 
{
	Mechos& mechos;
public:
	AnimationCallBackEvent(const EffectData& effect, Mechos& mechos);
	int quant();
};		

AnimationCallBackEvent::AnimationCallBackEvent(const EffectData& effect, Mechos& mechos_) 
: EffectData(effect),
ResourceUser(period), 
mechos(mechos_) 
{
	mechos.setChannel(channel);
	mechos.setAnimation(period, start, finish, M3D_ENGINE);
	if(sound && mechos.active())
		mechos.startSound(sound + mechos.personage_index*100, 1, mechos.features() & Mechos::HUMBLE ? 1 : 0);
	if(mode != Cycled){
		mechos.events_dispatcher.clear();
		mechos.events_dispatcher.attach(new AnimationCallBackEvent(*this));
		}
}

int AnimationCallBackEvent::quant() 
{ 
	switch(mode){
		case ForwardThenBackwardThenForwardThenBackward:
			mechos.setAnimation(0, finish, start, M3D_ENGINE);
			dtime = delay;
			mode = BackwardThenForwardThenBackward;
			return 1;
		case BackwardThenForwardThenBackward:
			mechos.setAnimation(-period, finish, start, M3D_ENGINE);
			mode = ForwardThenBackward;
			dtime = period;
			return 1;
		case ForwardThenBackward:
			mechos.setAnimation(period, start, finish, M3D_ENGINE);
			dtime = period;
			mode = ThenBackward;
			return 1;
		case ThenBackward:
			mechos.setAnimation(-period, finish, start, M3D_ENGINE);
			mode = Backward;
			dtime = period;
			return 1;
		case Forward:
		case Backward:
			mechos.runEffect(Mechos::NoEffect);
			return 0;
		default:
			assert(0);
			return 0;
		}
}

///////////////////////////////////////////////////////////////////////////////////////////
//		AnimationCallBackEvent
///////////////////////////////////////////////////////////////////////////////////////////
void Mechos::startEffect(Effect effect)
{
	if(current_effect == effect)
		return;
	if(effect == StoneHitEffect){
		runEffect(effect);
		return;
		}
	list<Effect>::iterator ei;
	FOR_EACH(effects_queue, ei)
		if(*ei == effect)
			return;
	effects_queue.push_back(effect);
}	

void Mechos::runEffect(Effect effect)
{
	current_effect_data = effect_data[current_effect = effect];
	
	AnimationCallBackEvent run_effect(current_effect_data, *this);

//	if(effect == NoEffect)
//		resting_disturb_timer.start(random(resting_disturb_random_delay));
}
     
void Mechos::effectQuant()
{
	if(!geometry)
		return;
	events_dispatcher.quant();

	if(current_effect_data.soundID() && active())
		updateSound(current_effect_data.soundID() + personage_index*100);

//	if(!effects_queue.empty() && current_effect == NoEffect){
	if(!effects_queue.empty() && (events_dispatcher.empty() || current_effect == RestingDisturbEffect)){
		if(!events_dispatcher.empty())
			events_dispatcher.clear();
		runEffect(effects_queue.front());
		effects_queue.pop_front();
		}

	if(current_effect == NoEffect) // && !resting_disturb_timer)
		startEffect(RestingDisturbEffect);

	// SwimmingEffect
	if(submersion() > 0.1){
		if(current_effect == NoEffect)
			runEffect(SwimmingEffect);
		}
	else
		if(current_effect == SwimmingEffect)
			runEffect(NoEffect);
		
	// FlyingByArcaneEffect
	if(isFlyingByArcane()){
		if(current_effect == NoEffect)
			runEffect(FlyingByArcaneEffect);
		}
	else
		if(current_effect == FlyingByArcaneEffect)
			runEffect(NoEffect);

	// SpringBoardFlyEffect		
	if(non_colliding_timer(!colliding() && is_dynamic(), 1000)){
		if(current_effect == NoEffect)
			runEffect(SpringBoardFlyEffect);
		}
	else
		if(current_effect == SpringBoardFlyEffect)
			runEffect(NoEffect);

	// RunToSeedEffect		
	if(state() == MOVING_PERSONAGE){
		if(current_effect == NoEffect)
			runEffect(RunToSeedEffect);
		}
	else
		if(current_effect == RunToSeedEffect)
			runEffect(NoEffect);
}

	
