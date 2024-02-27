#pragma once
#include "common.h"

class Plane : public ShapeActor
{
public:
	Plane(RefPtr<physics::World> world, Point position);

	RefPtr<physics::Body> GetPhysicBody() const;

	void OnRender(RenderContext& ctx) override;

private:
	float radius_ = 30.0f;
	RefPtr<Brush> border_brush_;
	RefPtr<physics::Body> body_;
};
