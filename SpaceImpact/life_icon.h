#pragma once
class LifeIcon : public GameObject
{
public:

	virtual ~LifeIcon() {  }

	virtual void Create() {}

	virtual void Init(int x, int y)
	{
		horizontalPosition = x+20;
		verticalPosition = y+5;
		GameObject::Init();
	}
};

