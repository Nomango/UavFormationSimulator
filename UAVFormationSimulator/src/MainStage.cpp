#include "MainStage.h"

MainStagePtr MainStage::Create()
{
	MainStagePtr ptr = new MainStage;
	if (ptr->Init())
	{
		return ptr;
	}
	return nullptr;
}

bool MainStage::Init()
{
	world_ = PhysicWorld::Create(Vec2(0.0f, 0.0f));
	//world_->ShowDebugInfo(true);
	this->AddComponent(world_);

	AddBounds();

	AddObstacles(Point(100, 100), Size(50, 80));
	AddObstacles(Point(GetWidth() - 100, 100), Size(50, 80));
	AddObstacles(Point(GetWidth() / 2 - 100, 400), Size(50, 200));
	AddObstacles(Point(GetWidth() / 2 + 100, 400), Size(50, 200));
	AddObstacles(Point(GetWidth() / 2 - 300, 600), Size(200, 50));
	AddObstacles(Point(GetWidth() / 2 + 300, 600), Size(200, 50));

	Vec2 offset = Vec2(50, 50);
	Point size = this->GetSize();

	Point leader_pos = Point(size.x / 2, size.y / 2 + 150);
	PlanePtr leader = AddPlane(leader_pos);

	const int follower_num = 4;
	for (int i = 0; i < follower_num; ++i)
	{
		int row = i / 2 + 1;
		int neg = (i % 2 == 0) ? -1 : 1;

		PlanePtr follower = AddPlane(Point(leader_pos.x + offset.x * neg * row, leader_pos.y + offset.y * row));

		auto body1 = leader->GetPhysicBody();
		auto body2 = follower->GetPhysicBody();

		DistanceJoint::Param param(body1, body2, body1->GetWorldCenter(), body2->GetWorldCenter());
		param.frequency_hz = 1.0f;
		param.damping_ratio = 0.0f;
		param.collide_connected = true;

		DistanceJointPtr joint = DistanceJoint::Create(param);
		world_->AddJoint(joint);
	}

	for (int i = 0; i < follower_num; i += 2)
	{
		auto body1 = followers_[i]->GetPhysicBody();
		auto body2 = followers_[i + 1]->GetPhysicBody();

		DistanceJoint::Param param(body1, body2, body1->GetWorldCenter(), body2->GetWorldCenter());
		param.frequency_hz = 0.1f;
		param.damping_ratio = 0.0f;
		param.collide_connected = true;

		DistanceJointPtr joint = DistanceJoint::Create(param);
		world_->AddJoint(joint);
	}
	return true;
}

void MainStage::OnUpdate(Duration dt)
{
	auto& input = Input::GetInstance();

	if (input.IsDown(KeyCode::Left))
	{
		PhysicBody* body = leader_->GetPhysicBody();
		body->ApplyTorque(-1.0f, true);
	}
	else if (input.IsDown(KeyCode::Right))
	{
		PhysicBody* body = leader_->GetPhysicBody();
		body->ApplyTorque(1.0f, true);
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
		PhysicBody* body = leader_->GetPhysicBody();
		body->ApplyForceToCenter(body->GetWorldVector(force));
	}

	for (auto follower : followers_)
	{
		float rotation = follower->GetRotation();
		follower->SetRotation(rotation + math::Sin(leader_->GetRotation() - rotation));
	}
}

void MainStage::AddBounds()
{
	Vector<Point> vertices = {
		Point(),
		Point(GetWidth(), 0),
		Point(GetWidth(), GetHeight()),
		Point(0, GetHeight()),
	};

	PhysicBodyPtr bounds = PhysicBody::Create(world_, PhysicBody::Type::Static);
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		if (i == vertices.size() - 1)
		{
			bounds->AddEdgeShape(vertices[i], vertices[0], 0.0f);
		}
		else
		{
			bounds->AddEdgeShape(vertices[i], vertices[i + 1], 0.0f);
		}
	}
}

void MainStage::AddObstacles(Point position, Size size)
{
	ShapePtr shape = Shape::CreateRect(Rect(0, 0, size.x, size.y));
	ShapeActorPtr obstacle = ShapeActor::Create(shape);
	obstacle->SetAnchor(0.5f, 0.5f);
	obstacle->SetPosition(position);
	obstacle->SetSize(size);
	obstacle->SetStrokeColor(Color::White);

	PhysicBodyPtr body = PhysicBody::Create(world_, PhysicBody::Type::Static);
	body->AddRectShape(obstacle->GetSize(), 0.0f);
	obstacle->AddComponent(body);

	this->AddChild(obstacle);
}

PlanePtr MainStage::AddPlane(Point position)
{
	PlanePtr plane = Plane::Create(world_, position);
	AddChild(plane);

	if (!leader_)
	{
		leader_ = plane;
	}
	else
	{
		followers_.push_back(plane);
	}
	return plane;
}
