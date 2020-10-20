#pragma once
#include "common.h"

KGE_DECLARE_SMART_PTR(Plane);
class Plane : public ShapeActor
{
public:
	Plane(PhysicWorldPtr world, Point position);

	void OnRender(RenderContext& ctx) override;

private:
	bool show_border_ = false;
	float radius_ = 30.0f;
	BrushPtr border_brush_;
};
