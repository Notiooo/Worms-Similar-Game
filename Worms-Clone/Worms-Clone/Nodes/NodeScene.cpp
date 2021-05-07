#include "NodeScene.h"
#include <cassert>
#include <functional>
#include <algorithm>

NodeScene::NodeScene() :
	pinnedNodes(),
	parent(nullptr)
{
}

void NodeScene::pinNode(Node node)
{
	// Now this Scene is parent of given node
	node->parent = this;

	// So lets add it into pinnedNodes of this Scene
	pinnedNodes.push_back(std::move(node));

}

NodeScene::Node NodeScene::unpinNode(const NodeScene& node_scene)
{
	// The address of given NodeScene have to match one of the addresses inside our vector of pinnedNodes
	auto foundNode = std::find_if(pinnedNodes.begin(), pinnedNodes.end(), [&node_scene](Node& pinned_node) -> bool { return pinned_node.get() == &node_scene; });

	// Make sure that we found such a Node to unpin
	assert(foundNode != pinnedNodes.end());

	// Now as we found the given NodeScene we must unpin it from our vector
	// First take the ownership from the vector
	auto stolenNode = std::move(*foundNode);

	// std::move makes sure it is in valid state to run destructor, so we delete it
	pinnedNodes.erase(foundNode);
	
	// Right now the stolen_node is not part of this Scene anymore.
	// We may remove its "parentness"
	stolenNode->parent = nullptr;

	return stolenNode;
}

sf::Vector2f NodeScene::getAbsolutePosition() const
{
	// To do this we have to add all transforms till the top of the hierarchy.
	sf::Transform transform; //sf::Transform::Identity;
	for (const auto* node = this; node != nullptr; node = node->parent)
		transform = node->getTransform() * transform;
	
	// By applying this transform on empty Vector2f we move it to the desired position
	return transform * sf::Vector2f();
}

NodeScene* NodeScene::getRootNode()
{
	if (parent)
		return parent->getRootNode();
	return this;
}

const NodeScene* NodeScene::getRootNode() const
{
	if (parent)
		return parent->getRootNode();
	return this;
}

bool NodeScene::isDestroyed()
{
	return destroyed;
}

void NodeScene::setDestroyed()
{
	destroyed = true;
}

void NodeScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// We want to draw this object in relation to the parent
	// states.transform stores information about parent scene render position
	// by multiplying it by our getTransform() position we get position related
	// to our parent

	states.transform *= getTransform(); // Absolute world transform is result of this operation

	// As our object is in the right position, we can finally draw it
	drawThis(target, states);

	// Right after we did this, we can forward drawing to the nodes lower in the hierarchy.
	// This way we will draw all nodes to the screen starting from the root.
	// Also it will help us to maintain the order of which should be in front or in the back of the screen
	for (const auto& pinnedNode : pinnedNodes)
		pinnedNode->draw(target, states);
}

void NodeScene::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Nothing here
}

void NodeScene::update(sf::Time deltaTime)
{
	// We starting by updating this object
	updateThis(deltaTime);

	// And then we update all pinnedNodes -- which updates all nodes pinned to them
	for (auto& pinnedNode : pinnedNodes)
		pinnedNode->update(deltaTime);
}

void NodeScene::updateThis(sf::Time deltaTime)
{
	// Nothing here
}

void NodeScene::removeDestroyed()
{
	const auto removalMark = std::remove_if(pinnedNodes.begin(), pinnedNodes.end(), std::mem_fn(&NodeScene::isDestroyed));
	pinnedNodes.erase(removalMark, pinnedNodes.end());

	for (auto& node : pinnedNodes)
		node->removeDestroyed();
}

void NodeScene::handleEvents(const sf::Event& event)
{
	// Handle event in this node
	handleThisEvents(event);

	// And next pass the event to all pinned nodes
	for (auto& pinnedNode : pinnedNodes)
		pinnedNode->handleEvents(event);
}

void NodeScene::handleThisEvents(const sf::Event& event)
{
}
