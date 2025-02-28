#pragma once
#include "Engine/GameObject.h"
class BoxCollider;
class Stage;
class Player;
class Timer;

class Vine :
    public GameObject
{
	//�|�C���^
	BoxCollider* vTipColl_;
	BoxCollider* VineColl_;
	Stage* stage_;
	Player* player_;
	Timer* timer_;

	//�n���h��
	int hVine_, hSpinyVine_, hLeafVine_;

	//���̑��̕ϐ� 
	bool isMove_;
	int Vines_;
	float MoveNum_;//�ړ���
	int addVinesLine_;
	int VineCollTall_;//�����蔻��Box�̍���
	int isUp_;//�c�^���L�т邩�������ނ�
public:
	Vine(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetPositionXY(const float& x, const float& y) { transform_.position_ = { x,y,0 }; }

	//�����ɓ�������
	//�����FpTarget ������������
	void OnCollision(GameObject* pTarget) override;
};

