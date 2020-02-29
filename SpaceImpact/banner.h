#pragma once
class Banner : public GameObject
{
public:

	virtual ~Banner() {  }

	virtual void Create(){}

	virtual void Init()
	{
		horizontalPosition = 0;
		verticalPosition = 0;
		GameObject::Init();
	}
};