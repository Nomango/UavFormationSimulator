#include "Plane.h"

PlanePtr Plane::Create(PhysicWorldPtr world, Point position)
{
	PlanePtr ptr = new Plane;
	if (ptr->Init(world, position))
	{
		return ptr;
	}
	return nullptr;
}

bool Plane::Init(PhysicWorldPtr world, Point position)
{
	float side = radius * math::Sqrt(2.0f);
	float offset = radius - side / 2 + radius / 10;
	Vector<Point> vertices = {
		Point(radius, offset),
		Point(offset, side),
		Point(radius, radius),
		Point(radius * 2 - offset, side),
	};

	ShapeMaker maker;
	maker.BeginPath(vertices[0]);
	maker.AddLines(vertices);
	maker.EndPath(true);

	ShapePtr chassis = maker.GetShape();
	this->SetShape(chassis);

	this->SetSize(radius * 2, radius * 2);
	this->SetAnchor(0.5f, 0.5f);
	this->SetPosition(position);

	PhysicBodyPtr body = PhysicBody::Create(world, PhysicBody::Type::Dynamic);
	body->AddCircleShape(radius, 2.0f, 0.0f);
	body->SetAngularDamping(20.0f);
	body->SetLinearDamping(0.5f);
	this->AddComponent(body);

	SetStrokeColor(Color::White);
	return true;
}

void Plane::OnRender(RenderContext& ctx)
{
	ShapeActor::OnRender(ctx);

	if (show_border)
	{
		ctx.SetCurrentBrush(Brush::Create(Color::BlueViolet));
		ctx.DrawEllipse(Point(radius, radius), Vec2(radius, radius));
	}
}
