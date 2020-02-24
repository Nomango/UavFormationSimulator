#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Plane);
class Plane : public ShapeActor
{
public:
	static PlanePtr Create(PhysicWorldPtr world, Point position);

	bool Init(PhysicWorldPtr world, Point position);

	void OnRender(RenderContext& ctx) override;

	bool show_border = false;
	float radius = 30.0f;
};
