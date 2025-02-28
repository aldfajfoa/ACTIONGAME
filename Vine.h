#pragma once
#include "Engine/GameObject.h"
class BoxCollider;
class Stage;
class Player;
class Timer;

class Vine :
    public GameObject
{
	//ポインタ
	BoxCollider* vTipColl_;
	BoxCollider* VineColl_;
	Stage* stage_;
	Player* player_;
	Timer* timer_;

	//ハンドル
	int hVine_, hSpinyVine_, hLeafVine_;

	//その他の変数 
	bool isMove_;
	int Vines_;
	float MoveNum_;//移動量
	int addVinesLine_;
	int VineCollTall_;//当たり判定Boxの高さ
	int isUp_;//ツタが伸びるか引っ込むか
public:
	Vine(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }

	//何かに当たった
	//引数：pTarget 当たった相手
	void OnCollision(GameObject* pTarget) override;
};

