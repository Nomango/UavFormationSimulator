#pragma once
#include "Plane.h"

KGE_DECLARE_SMART_PTR(MainStage);
class MainStage : public Stage
{
public:
	MainStage();

	void AddBounds();

	void AddObstacles(Point position, Size size);

	void InitPlanes();

	PlanePtr AddPlane(Point position);

	void OnUpdate(Duration dt) override;

private:
	PhysicWorldPtr world_;
	PlanePtr leader_;
	Vector<PlanePtr> followers_;
};
