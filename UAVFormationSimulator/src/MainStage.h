#pragma once
#include "Plane.h"

KGE_DECLARE_SMART_PTR(MainStage);
class MainStage : public Stage
{
public:
	static MainStagePtr Create();

	bool Init();

	void OnUpdate(Duration dt) override;

	void AddBounds();

	void AddObstacles(Point position, Size size);

	PlanePtr AddPlane(Point position);

private:
	PhysicWorldPtr world_;
	PlanePtr leader_;
	Vector<PlanePtr> followers_;
};
