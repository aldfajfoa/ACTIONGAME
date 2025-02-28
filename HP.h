#pragma once
#include "Engine/GameObject.h"
class Sprite;
class Timer;

class HP :
    public GameObject
{
	//定数
	const Size IMAGE_SIZE = { 512,32 };//画像サイズ
	const XMFLOAT2 HP_POS = { -0.9,-0.7 };
	const int MAX_HP = 500;//HP最大値

	//ハンドル
	int hHp_;

	//ポインタ
	Timer* isDamageTime_;

	//その他の変数
	float currentHp_;//現在HP
	bool isDamage_;
public:
	HP(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//HPを減らす関数
	//引数は減らしたい値
	void DecreaseHP(const int& decreaseNum) {
		if (isDamage_)
		{
			isDamage_ = false;
			currentHp_ -= decreaseNum;
		}
	}
};

