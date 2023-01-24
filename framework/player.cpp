#include "pch-il2cpp.h"

#include <iostream>

#include "player.h"
#include "util.h"
#include "store.h"

Player::Player() {}

Player::Player(Unity::CGameObject* object) {
	gameObject = object;
	init();
}

void Player::init()
{
	playerController = gameObject->GetComponent("PlayerController");

	transform = gameObject->GetTransform();

	local = playerController->GetMemberValue<bool>("controlInitialised");
	modelController = playerController->GetMemberValue<Unity::CComponent*>("modelController");
	gladiatorModel = modelController->GetMemberValue<Unity::CComponent*>("gladiatorModel");
	bodyRenderer = gladiatorModel->GetMemberValue<Unity::CComponent*>("bodyRenderer");
	rootBone = bodyRenderer->GetMemberValue<Unity::CComponent*>("rootBone");

	boltEntity = gameObject->GetComponent("BoltEntity");
	realEntity = boltEntity->GetMemberValue<Unity::CComponent*>("_entity");
	serializer = realEntity->GetMemberValue<Unity::CComponent*>("Serializer");

	initialized = true;
}

Unity::CCamera* Player::getWorldCamera()
{
	return playerController->GetMemberValue<Unity::CCamera*>("worldCamera");
}

Unity::Vector3 Player::position()
{
	return transform->GetPosition();
}

Unity::Quaternion Player::rotation()
{
	return transform->GetRotation();
}

double Player::distance(Unity::Vector3 to)
{
	return Util::distance(position(), to);
}

bool* Player::isDead()
{
	return serializer->GetMemberValue<bool*>("dead");
}

// Photon.Bolt.GladiatorState
std::string Player::name()
{
	Unity::System_String* n = serializer->GetMemberValue<Unity::System_String*>("name");
	return n->ToString();
}

bool* Player::isBot()
{
	return serializer->GetMemberValue<bool*>("isAI");
}

Unity::il2cppArray<Unity::CComponent*>* Player::getBones()
{
	gladiatorModel = modelController->GetMemberValue<Unity::CComponent*>("gladiatorModel");
	bodyRenderer = gladiatorModel->GetMemberValue<Unity::CComponent*>("bodyRenderer");
	return bodyRenderer->GetMemberValue<Unity::il2cppArray<Unity::CComponent*>*>("bones");
}

Unity::CComponent* Player::getBone(std::string search)
{
	Unity::il2cppArray<Unity::CComponent*>* bones = getBones();
	Unity::CComponent* bone = nullptr;

	if (bones->m_uMaxLength > 0)
	{
		bone = Util::findBone(bones, search);
	}

	return bone;
}

BoxCoords Player::calcBounds()
{
	BoxCoords coords;

	coords.bottomRight.x = 0;
	coords.topLeft.x = 0;
	coords.bottomRight.y = 0;
	coords.topLeft.y = 0;

	Unity::CComponent* head = getBone("head");
	if (head)
	{
		Unity::Vector3 headScreenPos;
		Unity::Vector3 footScreenPos;
		Unity::Vector3 footPos = position();
		Unity::Vector3 headPos = head->GetTransform()->GetPosition();

		Store::worldCamera->WorldToScreen(headPos, headScreenPos, 2);
		Store::worldCamera->WorldToScreen(footPos, footScreenPos, 2);

		float top = headScreenPos.y;
		float bottom = footScreenPos.y;
		float height = top - bottom;

		coords.topLeft.x = headScreenPos.x - (height / 4.f);
		coords.topLeft.y = Store::ScreenH - top;
		coords.bottomRight.x = footScreenPos.x + (height / 4.f);
		coords.bottomRight.y = Store::ScreenH - bottom;
	}

	return coords;
}