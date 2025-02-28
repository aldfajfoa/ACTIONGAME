#pragma once
#include "Engine/GameObject.h"
class BoxCollider;
class Player;
class Timer;

class Gool :
    public GameObject
{
	//ポインタ
	BoxCollider* GboxColl_;
	Player* player_;
	Timer* timer_;

	//ハンドル
	int hGool_;
public:
	Gool(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }
};

