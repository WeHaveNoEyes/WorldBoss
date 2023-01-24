#pragma once

#include "..\dependencies\resolver\Resolver.hpp"

struct BoxCoords {
	Unity::Vector2 topLeft;
	Unity::Vector2 bottomRight;
};

class Player
{
public:
	bool initialized = false;
	bool local = false;

	Player();
	Player(Unity::CGameObject* object);

	// Pointers

	Unity::CGameObject* gameObject = nullptr;
	Unity::CComponent* playerController = nullptr;
	Unity::CTransform* transform = nullptr;
	Unity::CComponent* modelController = nullptr;
	Unity::CComponent* gladiatorModel = nullptr;
	Unity::CComponent* bodyRenderer = nullptr;
	Unity::CComponent* rootBone = nullptr;
	Unity::CComponent* boltEntity = nullptr;
	Unity::CComponent* realEntity = nullptr;
	Unity::CComponent* serializer = nullptr;

	// Accessors

	Unity::Vector3 position();
	Unity::Quaternion rotation();

	// Utility

	Unity::CCamera* getWorldCamera();

	double distance(Unity::Vector3 to);

	bool* isDead();
	bool* isBot();
	std::string name();

	Unity::il2cppArray<Unity::CComponent*>* getBones();
	Unity::CComponent* getBone(std::string search);

	BoxCoords calcBounds();

private:
	void init();
};

