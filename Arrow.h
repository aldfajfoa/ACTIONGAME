#pragma once
#include "Engine/GameObject.h"
class Player;
class BoxCollider;
class Stage;
class Timer;

class Arrow :
	public GameObject
{
	//�萔
	const float MOVE_SPEED = 0.4;

	//�|�C���^
	Player* player_;
	Stage* stage_;
	BoxCollider* AboxColl_;
	Timer* timer_;

	//�n���h��
	int hArrow_;

	//���̑��̕ϐ�
	bool PisRight_;//�v���C���[���E��������������
	bool isMove_;
	float dy_;
	float Gravity_;
	bool isAlive_;

	//�֐�
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

