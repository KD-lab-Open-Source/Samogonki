////////////////////////////////////////////////////////////////////
//		Mechos
////////////////////////////////////////////////////////////////////
#ifndef __MECHOS_H__
#define __MECHOS_H__

#include "Body.h"
#include "PointControlledObject.h"
#include "Part.h"
#include "AnimatedBody.h"
#include "ArcaneStatistics.h"
#include "PersonageDifferetiationPrm.h"


class BodyDispatcher;

class Mechos : virtual public PointControlledObject, private PersonageDifferetiationData
{
public:
	enum State {	
		NORMAL_MECHOS, 	      // Нормальный, полнособранный, ездящий мехос
		STAYING_PERSONAGE,	// Стоящий после разрушения мехоса персонаж
		MOVING_PERSONAGE,	// Персонаж, двигающийся к котролпойнту
		WAITING_FOR_TELEPORTATION,  // Ожидающий телепортацию
		STARTING_TELEPORTATION,  // Начинающий телепортацию
		FINISHING_TELEPORTATION,  // Заканчивающий телепортацию
		ASSEMBLING_MECHOS,  // Собирающийся мехос 
		FORMIC_TRANSPORTED_MECHOS  //  Муравейник
		};

	enum Feature {
		NO_FEATURES = 0,

		NON_DESTRUCTING = 1,	// Неразрушаемый мехос
		MAGNETIC_CUSHION = 2,	
		NON_CONSTRUCTING = 4,	// Несобирающийся мехос
		NULIFICATOR = 8, // Нулификация - просачивание через 3D-объекты
		PROTECT_FROM_DAMAGE = 16, 

		IRON_HIP = 32,	// Железный бок
		ELASTIC_HIP = 64, // Упругий бок

		FIRE_PROTECTION = 128,
		LIGHTNING_PROTECTION = 256,
		STATIC_NULIFICATOR = 512, // Нулификация + остановка

		BUTTERFLY = 1 << 10,
		DIRIGIBLE = 1 << 11,
		JABASOMA = 1 << 12,
		DRAGON_POWER = 1 << 13,
		HELICOPTER = 1 << 14,

		RED_TRACK_FEATURE = 1 << 15,  // Алые следы

		FLYING_BY_JUMP_PAD = 1 << 16, // Выставляется автоматически
		ACCELERATED_BY_SPOT = 1 << 17, // Выставляется автоматически

		ROTATING = 1 << 18, // вращается
		FROZEN = 1 << 19,  // заморожен снежком
		TELEPORTATING = 1 << 20, // телепортирующийся

		HUMBLE = 1 << 21 // уменьшенный
		};


	enum Effect {
		NoEffect,
		HugeAccelerationEffect, // Сильное ускорение после наезда на специальные споты: одноразовая анимация.
		LandingEffect, // Приземление после долгого полета: одноразовая анимация.
		SpringBoardFlyEffect, // Свободный продолжительный полет после трамплина: зацикленная анимация.
		RestingDisturbEffect, // Cпокойное состоянии: периодически возникающая анимация.
		OutstripTheNeighbourEffect, // Обгон соперника (суматоху на старте не учитывать): одноразовая анимация.
		OutstrippedByNeighbourEffect, // Обгон соперником (суматоху на старте не учитывать): одноразовая анимация.
		CollisionEffect, // Столкновение с соперником: одноразовая анимация.
		InfluenceOfArcaneEffect, // Действие чужого Аркана: несколько видов одноразовой анимации.
		ExultEffect, // Ликование от попадания в соперника Арканом, успешное финиширование: одноразовая анимация.
		ObstacleAnnoyingEffect, // Раздражение от столкновения с препятствием-Арканом: одноразовая анимация.
		CheckPointOmissionEffect, // Пропуск чекпойнта: одноразовая анимация (оборачивается и кричит в камеру).
		FlyingByArcaneEffect, // Управляемый полет с помощью Аркана: зацикленная анимация (например, машет ручками).
		SwimmingEffect, // Плавание по воде: зацикленная анимация (например, переворачивается  как поплавок).
		StoneHitEffect, // Падает камень на голову
		RunToSeedEffect, 
		BonusExultEffect, // Ликование от взятия бонуса
		EffectsMax
		};

	enum DamageStyle {
		GENERIC_DAMAGE = 0,
		FIRE_DAMAGE,
		LIGHTNING_DAMAGE
		};

	enum WheelsKind { 
		BUTTERFLY_KIND = 3,
		DIRIGIBLE_KIND = 4,
		JABASOMA_KIND = 6,
		HELICOPTER_KIND = 7,
		DRAGON_POWER_KIND = 8,

		FURY_OF_SLIME_KIND = 6,

		MORPH_KIND = 64, // добавляется для получения kind морфинга
		ACTIVATED_KIND = 128 // добавляется для получения kind аркана
		};

	enum WheelMode {
		WHEEL = 1,
		FOOT = 2,
		WING = 3,
		NON_ROTATING_WHEEL = 4
		};

	enum PartIndex {
		//	4	2
		//	    1	
		//	   [0]
		//	5	3
		Engine,
		Front,
		RFWheel,
		RBWheel,
		LFWheel,
		LBWheel, 
		BorderSphere
		};	


		Mechos(const char* KindsStr);
		~Mechos();

	void set_activity(int flag);
	
	// Keyboard config
	void set_control_config(int config) { control_config = config; }
	int get_control_config() const { return control_config; }

	// Состояние мехоса /////////////
	const State& state() const { return state_; }

	// Angles [rad] /////////
	float yaw() const { return psi(); }
	float roll() const { return atan2(Zlocal().x, Zlocal().z); } // наклон вправо - положительное направление
	float pitch() const { return atan2(Zlocal().y, Zlocal().z); } // задирается нос - положительное направление

	// Управляемый полет с помощью аркана
	int isFlyingByArcane() const;

	int is_wheels_colliding() const { return wheels_colliding; }
	
	// Запускает (кладет в очередь)  ////
	void startEffect(Effect effect);

	// Смена детали
	void changePart(int part_index, int part_number);

	// Особенности в поведении /////
	int features() const { return features_; }
	void set_feature(Feature feature);
	void reset_feature(Feature feature);

	// Номер персонажа 0..9
	int personage() const { return personage_index; }
	const PersonageDifferetiationData& diff_data() const { return *this; }

	// Обращение к детали по типу
	cMesh* part_by_type(int partType) const;
	// Обращение к детали по индексу: 0-engine, 1-front, 2-back, 3-rf, 4-rb, 5-lf, 6-lb
	cMesh* part_by_index(int index) const;
	// Координаты детали
	Vect3f part_coords(int partType) const;

	// Colors & Alpha
	void setPermanentAlpha(float alpha);
	void setColor(int index, const struct sColor4f* diffuse, const sColor4f* specular);
	void getColor(int index, sColor4f* diffuse, sColor4f* specular);

	// Функции управления энергией /////////
	void damage(float dE, int damage_style = 0);  // -= dE
	void charge(float dE);  // += dE
	void discharge() { energy = 0; }  // Вместо disorganize
	float energy_max();

	// Арканы /////////////////
	void throwTitaniumBall(int damage);
	void cannon_ball_flight(); // полет на ядре
	void kinetic_impulse(const Vect3f& epicenter); // кинетический удар
	void formicTransport(const Vect3f& target, const Vect3f& next_point_for_direction);
	void animal_obstacle();
	void slime_obstacle();
	void beep();

	// Bonuses
	void jump();
	void speed_increment(); 
	int speed_bonus_counter() const; 

	// Sensor
	virtual void sensorIn(const class SensorFieldSource* sensor){}
	virtual void sensorOut(const SensorFieldSource* sensor){}

	// Teleportation
	void start_teleportation(const Vect3f& pos);
	void cancel_teleportation();

	void stopSounds();

	// Kron's sheet
	struct fxlabMechosProcessInformation& fxlabMechosInfo;

protected:

	State state_;
	int features_;
	int content;
	int personage_index;
	ShareHandle<Part> attracted_parts[NUM_PARTS];

	float alpha;

	// Controls /////////
	int control_config;
	float left_rudder_angle, right_rudder_angle;
	float obstacle_x_avr, obstacle_y_avr, obstacle_avr;
	float obstacle_rudder_t_avr;
	DurationTimer after_disorganize_latency;
	DurationTimer cannon_ball_timer;
	DurationTimer jump_timer;
	DurationTimer slime_timer;
	DurationTimer slime_obstacle_immunity_timer;
	DurationTimer animal_obstacle_immunity_timer;
	DurationTimer essence_slime_timer;
	DurationTimer acceleration_chance_timer;
	DelayTimer morph_finished;
	float forward_wheels_phase, backward_wheels_phase;
	HysteresisConditionTimer engine_sound_timer;
	int engineSound;
	Vect3f jump_pad_target;
	float jump_pad_dist0;
	float jump_pad_a, jump_pad_b, jump_pad_c; // parabola
	float speed_increment_factor;
	DurationTimer acceleration_spot_timer;
	Vect3f acceleration_spot_direction;

	// Essence visualization /////////
	int manual_essence_turn;
	Vect3f assembling_direction;

	// Quants and forces
	typedef int (Mechos::*QuantFunction)();
	typedef list<QuantFunction> QuantList;
	QuantList quants;
	QuantList forces;

	typedef void (Mechos::*WheelQuantFunction)();
	WheelQuantFunction forward_wheels_quant;
	WheelQuantFunction backward_wheels_quant;

	// Trail
	int trail_type;
	DurationTimer trail_started[4];
	Vect3f trail_pos0[4];
	Vect3f trail_pos1[4];
	int surface;
	HysteresisConditionTimer wheels_skidding_timer;

	// Breath
	MatXf part_offsets[NUM_PARTS];

	// Effects
	Vect3f character_angles;
	Vect3f character_angles_velocity;
	float velocity_y_prev;
	float swing_phase;
	Effect current_effect;
	EffectData current_effect_data;
	list<Effect> effects_queue;
	ResourceDispatcher events_dispatcher;
	AverageConditionTimer non_colliding_timer;
	DurationTimer resting_disturb_timer;
	static EffectData effect_data[EffectsMax];

	// Sensor
	DurationTimer sensor_timer;
	const SensorFieldSource* sensor_source;

	// Red Track
	ArcaneStatisticsService red_track_arcane_statistics;

	// Formic
	float formic_transport_finish_angle;

	// Rotation
	float rotation_angle;
	float rotation_speed;
	float prev_psi;

	// Teleportation
	AverageConditionTimer assembling_point_approach_timer;
	float assembling_point_dist;
	DurationTimer teleportation_timer;
	float teleportation_psi;

	// Jabasoma
	int jabasoma_jumping;
	float jabasoma_jumping_log;
	DurationTimer jabasoma_timer;

	// Beep
	int beep_pressed;

	// Teleportation
	MatXf teleportation_pose;
	Vect3f teleportation_dpos;

	// Collision correction
	Vect3f dV_applied;

	// Humble
	DurationTimer humble_timer;

	// Iron hip
	MeasurementTimer iron_hip_timer;

	/////////////////////////////////////////////////////////////
	//	Private methods
	/////////////////////////////////////////////////////////////

	void setPositionInit();
	void registerGeometry();
	void createBound();
	Part* detachPart(int part_id, int formic);
	Part* searchPart(int m3dType);
	void addPart(cMesh* geometry);
	void disorganize();
	void hide();
	void release_body_handles();

	void quant();
	void control();
	void post_quant();
	void calc_forces_and_drags(); 
	void body_overlap_reaction(Body& body);
	int is_colliding_with(const Body& body);
	void body_hit_reaction(const Body& body, const Contact& c);
	void ground_hit_reaction(const Contact& c);
	void keyboard_control();
	void write_state(XBuffer& buf);
	float damage_ability(const Body& b) const;
	
	void show() const;

	// Система обратной связи для PointControl
	void target_achieved();
	int forwardObstacle();
	int backwardObstacle();
	void resetObstacle();

	// Арканы, private part
	int iron_hip_quant();

	// Wheels tool
	WheelMode forward_wheels_mode();
	WheelMode backward_wheels_mode();
	int forward_wheels_kind();
	int backward_wheels_kind();
	int change_forward_wheels_kind(int new_kind);   // return old _kind_
	int change_backward_wheels_kind(int new_kind); // return old _kind_
	void set_forward_wheels_animation(float animation_period = 0, float start_phase = -1, float finish_phase = -1);
	void set_backward_wheels_animation(float animation_period = 0, float start_phase = -1, float finish_phase = -1);

	// Wheels quants
	void forward_rotated_quant();
	void forward_non_rotated_quant();
	void forward_foot_quant();
	void backward_rotated_quant();
	void backward_foot_quant();

	void fury_of_slime_foot_quant();


	// Butterfly
	void butterfly_start();
	void butterfly_stop();
	void butterfly_wing_quant();
	void butterfly_morph_quant();
	void butterfly_morph_inv_quant();
	int butterfly_force();
		
	// Jabasoma
	void jabasoma_start();
	void jabasoma_stop();
	void jabasoma_jumping_foot_quant();
	void jabasoma_morph_quant();
	void jabasoma_morph_inv_quant();
	int jabasoma_force();

	// Helicopter
	void helicopter_start();
	void helicopter_stop();
	void helicopter_morph_quant();
	void helicopter_quant();
	void helicopter_morph_inv_quant();
	int helicopter_force();

	// Dragon Power
	void dragon_power_start();
	void dragon_power_stop();
	void dragon_power_wing_quant();
	void dragon_power_morph_quant();
	void dragon_power_morph_inv_quant();
	int dragon_power_force();


	// Forces 
	int cannon_ball_quant();
	int jump_quant();
	int dirigible_quant();
	int magnetic_cushion_force();
	int acceleration_spot_quant();
	void acceleration_spot_start(const Vect3f& direction);
	int jump_pad_quant();
	void jump_pad_start(const Vect3f& target);
	void apply_flying_force(struct FlyingArcaneData& fly);

	// Axilliary
	void startEngineSound();
	void changeEngineSound(int newSound);
	void stopEngineSound();
	float speedSoundFactor();
	
	void changePart();

	// Effects
	void effectQuant();
	void runEffect(Effect effect);

	// Sensor
	void field_affection(const FieldSource* sensor);

	// Rotation
	int startRotation(float angle, float speed);
	int rotation_quant();

	void calc_forces_and_drags_of_moving_personage();
	void calc_forces_and_drags_of_normal_mechos();
	void calc_forces_and_drags_of_assembling_personage();
	void quant_of_normal_mechos();
	void quant_of_assembling_mechos();

	void start_teleportation();
	int teleportation_quant();

	void set_brake(int on);

	// Wheels
	enum WheelIndex {
		//	2	0
		//	    ^	
		//	   [I]
		//	3	1
		WheelRF,
		WheelRB,
		WheelLF,
		WheelLB
		};	

	struct Wheel
	{
		float radius;
		float TOI_inv;
		float wheel_penetration;
		float stiffness, damping;
		float phi, omega, angular_acceleration;
		float suspension_z;
		int dynamic_frictional;
		float brake_torque;
		float brake_damping;

		Vect3f position;  // crf
		Vect3f axle;  //  crf

		Vect3f position_global;  //  wrf
		Vect3f axle_global;  //  wrf

		float f_n, j_t;
		float v_n, v_t, v_t_;
		float skidding_factor;
		int skidding;

		int n_contacts;
		float total_penetration;
		Vect3f contact_point;  //  wrf 
		Vect3f normal;  // wrf
		WheelContactProperty* property;

		int n_contacts_non_central;
		float total_penetration_non_central;
		Vect3f contact_point_non_central;  //  wrf 
		Vect3f normal_non_central;  // wrf
		WheelContactProperty* property_non_central;

		Vect3f tangent;  // wrf
		Vect3f axle_cross_radius;
		float radius_cross_tangent_dot_axle; // 0 if wheel is blocked
		float brake_delta_omega;  // ability to stop omega by brake torque
		float max_friction_impulse;
		const Body* contacted_body;

		Mechos* chassis;

		Wheel();
		void init(float mass_, float radius_, float wheel_penetration, float damping_scale, Mechos* chassis);
		void set_steering_angle(float angle);
		void convert(const MatXf& pose);
		void add_contact(int central, float penetration, const Vect3f& point_, const Vect3f& normal_, WheelContactProperty* property, const Body* body = 0);
		void apply_torque(float torque);
		void post_collision();
		int   check_if_blocked(float j);
		void set_solution(float j);
		void integrate(float dt);
		float suspension_travel();
		void post_test();
	}; 
	int wheels_colliding;
	Wheel wheels[4];
	SphereList wheels_spheres;
	void applyWheelsForce();
	void add_contact(float penetration, const Vect3f& point, const Vect3f& normal, const Sphere& sphere, const Body* body);

	//  Friends
	friend Part;
	friend BodyDispatcher;
	friend Wheel;
	friend class AnimationCallBackEvent;
	friend class TestWheelsCollisionOp;
};

#endif // __MECHOS_H__
