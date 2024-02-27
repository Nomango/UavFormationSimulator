#include "MainStage.h"

MainStage::MainStage()
{
	world_ = new physics::World(b2Vec2(0.0f, 0.0f));
	this->AddComponent(world_);

	AddBounds();

	AddObstacle(Point(100, 100), Size(50, 80));
	AddObstacle(Point(GetWidth() - 100, 100), Size(50, 80));
	AddObstacle(Point(GetWidth() / 2 - 100, 400), Size(50, 200));
	AddObstacle(Point(GetWidth() / 2 + 100, 400), Size(50, 200));
	AddObstacle(Point(GetWidth() / 2 - 300, 600), Size(200, 50));
	AddObstacle(Point(GetWidth() / 2 + 300, 600), Size(200, 50));

	InitPlanes();
}

void MainStage::AddBounds()
{
	Vector<Point> vertices = {
		Point(),
		Point(GetWidth(), 0),
		Point(GetWidth(), GetHeight()),
		Point(0, GetHeight()),
	};

	b2BodyDef def;
	def.type = b2_staticBody;
	auto bounds = world_->AddBody(&def);
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		b2EdgeShape shape;
		if (i == vertices.size() - 1)
		{
			shape.Set(physics::LocalToWorld(vertices[i]), physics::LocalToWorld(vertices[0]));
		}
		else
		{
			shape.Set(physics::LocalToWorld(vertices[i]), physics::LocalToWorld(vertices[i + 1]));
		}

		b2FixtureDef def;
		def.shape = &shape;
		bounds->GetB2Body()->CreateFixture(&def);
	}
}

void MainStage::AddObstacle(Point position, Size size)
{
	RefPtr<Shape> shape = Shape::CreateRect(Rect(0, 0, size.x, size.y));
	RefPtr<ShapeActor> obstacle = new ShapeActor(shape);
	obstacle->SetAnchor(0.5f, 0.5f);
	obstacle->SetPosition(position);
	obstacle->SetSize(size);
	obstacle->SetStrokeColor(Color::Gray);

	b2BodyDef def;
	def.type = b2_staticBody;
	auto body = world_->AddBody(&def);
	obstacle->AddComponent(body);

	b2PolygonShape b2shape;
	auto b2size = physics::LocalToWorld(obstacle->GetSize() / 2);
	b2shape.SetAsBox(b2size.x, b2size.y);

	b2FixtureDef fixture_def;
	fixture_def.shape = &b2shape;
	body->GetB2Body()->CreateFixture(&fixture_def);

	this->AddChild(obstacle);
}

void MainStage::InitPlanes()
{
	Vec2 offset = Vec2(50, 50);
	Point size = this->GetSize();

	Point leader_pos = Point(size.x / 2, size.y / 2 + 150);
	RefPtr<Plane> leader = AddPlane(leader_pos);

	const int follower_num = 4;
	for (int i = 0; i < follower_num; ++i)
	{
		int row = i / 2 + 1;
		int neg = (i % 2 == 0) ? -1 : 1;

		RefPtr<Plane> follower = AddPlane(Point(leader_pos.x + offset.x * neg * row, leader_pos.y + offset.y * row));

		auto body1 = leader->GetPhysicBody()->GetB2Body();
		auto body2 = follower->GetPhysicBody()->GetB2Body();

		b2DistanceJointDef joint_def;
		joint_def.Initialize(body1, body2, body1->GetWorldCenter(), body2->GetWorldCenter());
		joint_def.frequencyHz = 1.0f;
		joint_def.dampingRatio = 0.0f;
		joint_def.collideConnected = true;

		world_->AddJoint(&joint_def);
	}

	for (int i = 0; i < follower_num; i += 2)
	{
		auto body1 = followers_[i]->GetPhysicBody()->GetB2Body();
		auto body2 = followers_[i + 1]->GetPhysicBody()->GetB2Body();

		b2DistanceJointDef joint_def;
		joint_def.Initialize(body1, body2, body1->GetWorldCenter(), body2->GetWorldCenter());
		joint_def.frequencyHz = 0.1f;
		joint_def.dampingRatio = 0.0f;
		joint_def.collideConnected = true;

		world_->AddJoint(&joint_def);
	}
}

RefPtr<Plane> MainStage::AddPlane(Point position)
{
	RefPtr<Plane> plane = new Plane(world_, position);
	AddChild(plane);

	if (!leader_)
	{
		leader_ = plane;
		leader_->SetStrokeColor(Color::White);
	}
	else
	{
		followers_.push_back(plane);
	}
	return plane;
}

void MainStage::OnUpdate(Duration dt)
{
	auto& input = Input::GetInstance();

	// 按D键开启Debug模式
	if (input.WasPressed(KeyCode::D))
	{
		static bool debug_mode = false;
		debug_mode = !debug_mode;
		world_->ShowDebugInfo(debug_mode);
	}

	if (input.IsDown(KeyCode::Left))
	{
		auto body = leader_->GetPhysicBody();
		body->GetB2Body()->ApplyTorque(-0.01f, true);
	}
	else if (input.IsDown(KeyCode::Right))
	{
		auto body = leader_->GetPhysicBody();
		body->GetB2Body()->ApplyTorque(0.01f, true);
	}

	Vec2 force;
	if (input.IsDown(KeyCode::Up))
	{
		force = Vec2(0, -2);
	}
	else if (input.IsDown(KeyCode::Down))
	{
		force = Vec2(0, 2);
	}

	if (!force.IsOrigin())
	{
		auto body = leader_->GetPhysicBody();
		body->GetB2Body()->ApplyForceToCenter(physics::LocalToWorld(body->GetWorldVector(force)), true);
	}

	for (auto follower : followers_)
	{
		float rotation = follower->GetRotation();
		follower->SetRotation(rotation + math::Sin(leader_->GetRotation() - rotation));
	}
}
