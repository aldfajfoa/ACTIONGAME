#pragma once
#include "Engine/GameObject.h"
class Fbx;
class Stage;

class PlayScene :
    public GameObject
{
	Stage* pField_;
public:
	PlayScene(GameObject* parent);
	void Initialize() override;
	void Update() override {};
	void Draw() override {};
	void Release() override;
};

