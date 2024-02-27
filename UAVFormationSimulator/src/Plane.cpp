#include "Plane.h"

Plane::Plane(RefPtr<physics::World> world, Point position)
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

	RefPtr<Shape> chassis = maker.GetShape();
	this->SetShape(chassis);

	this->SetSize(radius_ * 2, radius_ * 2);
	this->SetAnchor(0.5f, 0.5f);
	this->SetPosition(position);
	this->SetStrokeColor(Color::Gray);

	b2BodyDef def;
	def.type = b2_dynamicBody;
	def.angularDamping = 20.f;
	def.linearDamping = 0.5f;
	body_ = world->AddBody(&def);
	this->AddComponent(body_);

	b2CircleShape b2shape;
	b2shape.m_radius = physics::LocalToWorld(radius_);

	b2FixtureDef fixture_def;
	fixture_def.density = 2.0f;
	fixture_def.friction = 0.0f;
	fixture_def.shape = &b2shape;
	body_->GetB2Body()->CreateFixture(&fixture_def);

	border_brush_ = new Brush(Color::BlueViolet);
}

RefPtr<physics::Body> Plane::GetPhysicBody() const
{
	return body_;
}

void Plane::OnRender(RenderContext& ctx)
{
	ShapeActor::OnRender(ctx);

#if 0
	ctx.SetCurrentBrush(border_brush_);
	ctx.DrawCircle(Point(radius_, radius_), radius_);
#endif
}
