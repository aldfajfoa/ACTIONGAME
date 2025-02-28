#pragma once
#include "Engine/GameObject.h"
class Stage;
class BoxCollider;
class HP;
class Arrow;
class Player;

const int FBOX_NUM = 5;

class Flower :
    public GameObject
{
	//定数
	const int F_RIGHT_ANGLE = 160;
	const int F_LEFT_ANGLE = 360 - F_RIGHT_ANGLE;
	const float GRAVITY = 18.0f / 60.0f; //重力加速度
	const int FLOWER_HP = 6;//花の体力
	const float F_MOVE_SPEED = 0.04;//移動速度

	//ポインタ
	BoxCollider* FboxColl_[FBOX_NUM];//ビースト当たり判定
	BoxCollider* CheckScaffoldColl_;//進行方向に足場があるかどうかを調べる当たり判定
	Stage* stage_;
	Player* player_;
	HP* hp_;
	Arrow* arrow_;

	//ハンドル
	int hFlower_;

	//その他の変数
	int Ground_;//地面
	bool isRight_;//右向きかどうか
	bool isLeftMove_, isRightMove_;
	int FlowerHp_;//花の体力
public:
	Flower(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetGround(const float& ground) { Ground_ = ground; }
	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }

	void RotFlower(const bool& isRight);//花の向きを変える

	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;

	void MoveFlower();
};

