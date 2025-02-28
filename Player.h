#pragma once
#include "Engine/GameObject.h"
#include "Engine/Transform.h"
class FBX;
class BoxCollider;
class Stage;
class HP;
class Arrow;
class Timer;

const int PBOX_NUM = 5;
const int ARROW_NUM = 5;

class Player :
	public GameObject
{
	//定数
	const int P_RIGHT_ANGLE = 90;
	const int P_LEFT_ANGLE = 360 - P_RIGHT_ANGLE;
	const float P_JUMP_HEIGHT = 1.0f * 10.0f;//ジャンプの高さ
	const float GRAVITY = 9.0f / 650.0f;    //重力加速度
	const float P_MOVE_SPEED = 0.05;//移動速度

	//ポインタ
	BoxCollider* PboxColl_[PBOX_NUM];
	Arrow* arrows_[ARROW_NUM];
	Stage* stage_;
	HP* hp_;
	Timer* timer_;
	Timer* ArrowInterval_;

	//ハンドル
	int hPlayer_;

	//その他の変数
	bool isRight_;//右向きかどうか
	float jumpSpeed_;//ジャンプの高さ
	int Ground_;//地面
	float CameraStopGround_;//これより下にいくとカメラがプレイヤーを追わなくなる
	float DeathGround_;//ここまで落ちたらゲームオーバー
	bool isLeftMove_, isRightMove_, isJump_;//左右移動とジャンプができるかの判定
	bool isPdamage_[4];//このどれかがtrueの間はダメージを受ける
	bool isfirst_;//ダメージを受けるブロックに触れた瞬間かどうか
	int currentArrow_;
	float Dist_;
	int Dir_;

	//関数
	void MovePlayer();//プレイヤー移動
	void DiscernBrock(const int& x, const int& y, const Dir& dir);
public:

	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;

	void SetGround(const float& ground) { Ground_ = ground; }
	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }//プレイヤーのポジション
	//CameraStopGround_を設定しそれを元にDeathGround_の設定もする
	void SetCameraStopandDeathGround_(const float& Ground) { CameraStopGround_ = Ground;
		                                                     DeathGround_ = CameraStopGround_ - 20;}
	void SetisLeftMove(bool isLeftMove) { isLeftMove_ = isLeftMove; }
	void SetisRightMove(bool isRightMove) { isRightMove_ = isRightMove; }
	void setDistandDir(float dist, float dir) {
		Dist_ = dist;
		Dir_ = dir;
	}

	XMFLOAT3 GetPosition() { return transform_.position_; }
	bool GetisRight_() { return isRight_; }
	int GetARROW_NUM() { return ARROW_NUM; }
	Arrow* Getarrows(const int& index) { return arrows_[index]; }
	BoxCollider* GetCenterBoxColl() { return PboxColl_[CENTER]; }//プレイヤーの中心当たり判定を取得
	BoxCollider* GetPBoxColl(Dir dir) { return PboxColl_[dir]; }

	void RotPlayer(const bool& isRight);//プレイヤーの向きを変える
	void BlowawayPlayer();//プレイヤーを吹っ飛ばす
};

