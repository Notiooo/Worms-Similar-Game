#ifndef NODESCENE_H
#define NODESCENE_H

#include <memory>
#include <list>

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/System/NonCopyable.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"

// It derives from sf::Drawable as it is suppossed to be drawn on the screen.
// It derives from sf::Transformable which gives all members related with position, rotation and scale
// It derives from sf::NonCopyable as NodeScene like this should not be copied (it may give many problems in current state).
// This make copy constructor/assignment to be deleted
class NodeScene : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
public:
	// It will use dynamic linking to it's
	// derived classes
	using Node = std::unique_ptr<NodeScene>;

	NodeScene();
	virtual ~NodeScene() = default;

	// Steals ownership, and puts it into the vector of pinned_Nods
	void pinNode(Node);

	// Removes this NodeScene from pinned_nodes_ and returns it
	Node unpinNode(const NodeScene&);

	// Returns absolute position in the game plane
	sf::Vector2f getAbsolutePosition() const;

	// ====== Drawing Scenes ====== //

	// This function is provided inside sf::Drawable
	// RenderTarget tell where drawable object should be drawn to
	// RenderStates is optional, and provides informations about rendering process (transform, shader, blend mode)
	// Thanks to this if we pass this object to sf::RenderWindow::draw(), then it will implicitly call this function
	// to draw it!
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

	// This function cares only about drawing itself
	virtual void drawThis(sf::RenderTarget& target, sf::RenderStates states) const;


	// ====== Updating Scenes ====== //

	// In comparison to draw() this function is not derived. It is used to update all pinned_nodes_
	void update(sf::Time deltaTime);

	// Works analogues to the drawThis(), updates all things related to itself
	virtual void updateThis(sf::Time deltaTime);

	virtual void removeDestroyed();

	// ====== Handling events ====== //
	void handleEvents(const sf::Event& event);
	virtual void handleThisEvents(const sf::Event& event);

protected:
	// Returns the root node
	NodeScene* getRootNode();
	const NodeScene* getRootNode() const;

	// Tells if object should be destroyed_
	virtual bool is_destroyed();
	virtual void set_destroyed();

private:

	// It was vector before, but I had to change it to the list,
	// as some object inside range-for loop of this pinned_nodes_
	// started pin its nodes to this global pinned_nodes_
	// and that was risky that pinned_nodes_ may reallocate durning
	// its iteration of the range-for loop

	// What objects started to pin some object to the root_node?
	// Actually missles, I wanted them to be global, thus I put
	// them in the root node
	std::list<Node> pinned_nodes_;
	NodeScene* parent_;
	bool destroyed_ = false;
};

#endif
