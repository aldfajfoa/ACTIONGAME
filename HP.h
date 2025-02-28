#pragma once
#include "Engine/GameObject.h"
class Sprite;
class Timer;

class HP :
    public GameObject
{
	//�萔
	const Size IMAGE_SIZE = { 512,32 };//�摜�T�C�Y
	const XMFLOAT2 HP_POS = { -0.9,-0.7 };
	const int MAX_HP = 500;//HP�ő�l

	//�n���h��
	int hHp_;

	//�|�C���^
	Timer* isDamageTime_;

	//���̑��̕ϐ�
	float currentHp_;//����HP
	bool isDamage_;
public:
	HP(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//HP�����炷�֐�
	//�����͌��炵�����l
	void DecreaseHP(const int& decreaseNum) {
		if (isDamage_)
		{
			isDamage_ = false;
			currentHp_ -= decreaseNum;
		}
	}
};

