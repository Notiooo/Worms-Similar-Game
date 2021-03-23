#include "NodeScene.h"
#include <cassert>
#include <functional>
#include <algorithm>

NodeScene::NodeScene() :
	pinned_nodes_(),
	parent_(nullptr)
{
}

void NodeScene::pinNode(Node node)
{
	// Now this Scene is parent of given node
	node->parent_ = this;

	// So lets add it into pinned_nodes_ of this Scene
	pinned_nodes_.push_back(std::move(node));

}

NodeScene::Node NodeScene::unpinNode(const NodeScene& node_scene)
{
	// The address of given NodeScene have to match one of the addresses inside our vector of pinned_nodes_
	auto found_node = std::find_if(pinned_nodes_.begin(), pinned_nodes_.end(), [&node_scene](Node& pinned_node) -> bool { return pinned_node.get() == &node_scene; });

	// Make sure that we found such a Node to unpin
	assert(found_node != pinned_nodes_.end());

	// Now as we found the given NodeScene we must unpin it from our vector
	// First take the ownership from the vector
	Node stolen_node = std::move(*found_node);

	// std::move makes sure it is in valid state to run destructor, so we delete it
	pinned_nodes_.erase(found_node);
	
	// Right now the stolen_node is not part of this Scene anymore.
	// We may remove its "parentness"
	stolen_node->parent_ = nullptr;

	return stolen_node;
}

/*
void NodeScene::unpinItself()
{
	parent_->unpinNode(*this);
	pinned_nodes_.clear();
}
*/

sf::Vector2f NodeScene::getAbsolutePosition() const
{
	// To do this we have to add all transforms till the top of the hierarchy.
	sf::Transform transform; //sf::Transform::Identity;
	for (const NodeScene* node = this; node != nullptr; node = node->parent_)
		transform = node->getTransform() * transform;
	
	// By applying this transform on empty Vector2f we move it to the desired position
	return transform * sf::Vector2f();
}

NodeScene* NodeScene::getRootNode()
{
	if (parent_)
		return parent_->getRootNode();
	return this;
}

const NodeScene* NodeScene::getRootNode() const
{
	if (parent_)
		return parent_->getRootNode();
	return this;
}

bool NodeScene::is_destroyed()
{
	return destroyed_;
}

void NodeScene::set_destroyed()
{
	destroyed_ = true;
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
	for (const Node& pinned_node : pinned_nodes_)
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

	// And then we update all pinned_nodes_ -- which updates all nodes pinned to them
	for (auto& pinned_node : pinned_nodes_)
		pinned_node->update(deltaTime);
}

void NodeScene::updateThis(sf::Time deltaTime)
{
	// Nothing here
}

void NodeScene::removeDestroyed()
{
	auto removal_mark = std::remove_if(pinned_nodes_.begin(), pinned_nodes_.end(), std::mem_fn(&NodeScene::is_destroyed));
	pinned_nodes_.erase(removal_mark, pinned_nodes_.end());

	for (auto& node : pinned_nodes_)
		node->removeDestroyed();
}

void NodeScene::handleEvents(const sf::Event& event)
{
	// Handle event in this node
	handleThisEvents(event);

	// And next pass the event to all pinned nodes
	for (auto& pinned_node : pinned_nodes_)
		pinned_node->handleEvents(event);
}

void NodeScene::handleThisEvents(const sf::Event& event)
{
}
