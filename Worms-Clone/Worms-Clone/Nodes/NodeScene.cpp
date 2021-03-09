#include "NodeScene.h"
#include <cassert>

void NodeScene::pinNode(Node node)
{
	// Now this Scene is parent of given node
	node->_parent = this;

	// So lets add it into pinned_Nodes of this Scene
	pinned_Nodes.push_back(std::move(node));

}

NodeScene::Node NodeScene::unpinNode(const NodeScene& node_scene)
{
	// The address of given NodeScene have to match one of the addresses inside our vector of pinned_Nodes
	auto found_node = std::find_if(pinned_Nodes.begin(), pinned_Nodes.end(), [&node_scene](Node& pinned_node) -> bool { return pinned_node.get() == &node_scene; });

	// Make sure that we found such a Node to unpin
	assert(found_node != pinned_Nodes.end());

	// Now as we found the given NodeScene we must unpin it from our vector
	// First take the ownership from the vector
	Node stolen_node = std::move(*found_node);

	// std::move makes sure it is in valid state to run destructor, so we delete it
	pinned_Nodes.erase(found_node);
	
	// Right now the stolen_node is not part of this Scene anymore.
	// We may remove its "parentness"
	stolen_node->_parent = nullptr;

	return stolen_node;
}

sf::Vector2f NodeScene::getAbsolutePosition() const
{
	// To do this we have to add all transforms till the top of the hierarchy.
	sf::Transform transform; //sf::Transform::Identity;
	for (const NodeScene* node = this; node != nullptr; node = node->_parent)
		transform = node->getTransform() * transform;
	
	// By applying this transform on empty Vector2f we move it to the desired position
	return transform * sf::Vector2f();
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
	for (const Node& pinned_node : pinned_Nodes)
		pinned_node->draw(target, states);
}

void NodeScene::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Nothing here
}

void NodeScene::update(sf::Time deltaTime)
{
	// We starting by updating this object
	updateThis(deltaTime);

	// And then we update all pinned_Nodes -- which updates all nodes pinned to them
	for (auto& pinned_node : pinned_Nodes)
		pinned_node->update(deltaTime);
}

void NodeScene::updateThis(sf::Time deltaTime)
{
	// Nothing here
}

void NodeScene::handleEvents(const sf::Event& event)
{
	// Handle event in this node
	handleThisEvents(event);

	// And next pass the event to all pinned nodes
	for (auto& pinned_node : pinned_Nodes)
		pinned_node->handleEvents(event);
}

void NodeScene::handleThisEvents(const sf::Event& event)
{
}
