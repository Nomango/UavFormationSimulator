#pragma once
#include "Plane.h"

class MainStage : public Stage
{
public:
	MainStage();

	void AddBounds();

	void AddObstacle(Point position, Size size);

	void InitPlanes();

	RefPtr<Plane> AddPlane(Point position);

	void OnUpdate(Duration dt) override;

private:
	RefPtr<physics::World> world_;
	RefPtr<Plane> leader_;
	Vector<RefPtr<Plane>> followers_;
};
