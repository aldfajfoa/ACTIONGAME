#pragma once
#include "Engine/GameObject.h"
class Player;
class BoxCollider;
class Stage;
class Timer;

class Arrow :
	public GameObject
{
	//定数
	const float MOVE_SPEED = 0.4;

	//ポインタ
	Player* player_;
	Stage* stage_;
	BoxCollider* AboxColl_;
	Timer* timer_;

	//ハンドル
	int hArrow_;

	//その他の変数
	bool PisRight_;//プレイヤーが右向きか左向きか
	bool isMove_;
	float dy_;
	float Gravity_;
	bool isAlive_;

	//関数
	void ArrowCollision();
public:
	Arrow(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	
	BoxCollider* GetAboxColl() { return AboxColl_; }
	void SetisAlive(const bool& isAlive) { isAlive_ = isAlive; }
	void SetArrowDir(const bool& PisRight);
	void ResetArrow();
};

