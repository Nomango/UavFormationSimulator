#include "Plane.h"

Plane::Plane(PhysicWorldPtr world, Point position)
{
	float side = radius_ * math::Sqrt(2.0f);
	float offset = radius_ - side / 2 + radius_ / 10;
	Vector<Point> vertices = {
		Point(radius_, offset),
		Point(offset, side),
		Point(radius_, radius_),
		Point(radius_ * 2 - offset, side),
	};

	ShapeMaker maker;
	maker.BeginPath(vertices[0]);
	maker.AddLines(vertices);
	maker.EndPath(true);

	ShapePtr chassis = maker.GetShape();
	this->SetShape(chassis);

	this->SetSize(radius_ * 2, radius_ * 2);
	this->SetAnchor(0.5f, 0.5f);
	this->SetPosition(position);
	this->SetStrokeColor(Color::Gray);

	PhysicBodyPtr body = new PhysicBody(world, PhysicBody::Type::Dynamic);
	body->AddCircleShape(radius_, 2.0f, 0.0f);
	body->SetAngularDamping(20.0f);
	body->SetLinearDamping(0.5f);
	this->AddComponent(body);

	border_brush_ = new Brush(Color::BlueViolet);
}

void Plane::OnRender(RenderContext& ctx)
{
	ShapeActor::OnRender(ctx);

#if 0
	ctx.SetCurrentBrush(border_brush_);
	ctx.DrawCircle(Point(radius_, radius_), radius_);
#endif
}
