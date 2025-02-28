#pragma once
#include "Engine/GameObject.h"
class BoxCollider;
class SphereCollider;
class Stage;
class Arrow;
class Player;
class HP;

const int BBOX_NUM = 5;

class Beast :
    public GameObject
{
	//定数
	const float B_JUMP_HEIGHT = 1.0f * 3.0f;//ジャンプの高さ
	const float GRAVITY = 9.0f / 650.0f;    //重力加速度
	const int BEAST_HP = 3;//ビーストの体力
	const float B_MOVE_SPEED = 0.02;//移動速度

	//ポインタ
	BoxCollider* BboxColl_[BBOX_NUM];//ビースト当たり判定
	SphereCollider* DetectCircle_;//プレイヤーを検知する範囲
	Stage* stage_;
	Arrow* arrow_;
	Player* player_;
	HP* hp_;

	//ハンドル
	int hBeast_;

	//その他の変数
	float jumpSpeed_;//ジャンプの高さ
	int Ground_;//地面
	bool isRight_;//右向きかどうか
	bool isGround_;
	bool isLeftMove_, isRightMove_, isJump_;//左右移動とジャンプができるかの判定
	int BeastHp_;//ビーストの体力
	bool isChase_;//プレイヤーを追いかけるか

	//関数
	void MoveBeast();//ビースト移動

public:
	Beast(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetGround(const float& ground) { Ground_ = ground; }
	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }

	void RotBeast(const bool& isRight);//ビーストの向きを変える

	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;
};

