#ifndef NODESCENE_H
#define NODESCENE_H

#include <list>
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>


/**
 * \brief Scene on which objects and other pinned scenes will be drawn.
 *
 * Individual nodes (objects inside the game) can be parents of other nodes.
 * Thus, moving the parent will make the relativistic position of the nodes
 * (children) pinned to it also to change. This makes it very easy to create
 * any hierarchies and scenes within the game. Nodes that are pinned to a
 * parent node at the time of drawing receive **transform** of node they
 * are pinned to, so that they can draw themselves relative to their parent.
 *
 * We can say that the way the nodes are executed resembles "Depth-first search".
 * It starts at the root node, and it explores as far as possible along each branch
 * before backtracking. This also means that objects higher up in the hierarchy
 * (parents) will be drawn on the screen underneath their pinned nodes (children).
 *
 *               +-----------+
 *               |(Root) Node|
 *               +-----+-----+
 *                     |
 *     +---------+-----+------------+
 *     |         |                  |
 *   +-v--+    +-v--+             +-v--+
 *(1)|Node| (2)|Node|             |Node|(7)
 *   +----+    +-+--+             +-+--+
 *               |                  |
 *             +-v--+             +-v--+
 *          (3)|Node|             |Node|(8)
 *       +-----+-+--+-----+       +-+--+
 *       |       |        |         |
 *     +-v--+  +-v--+  +--v-+     +-v--+
 *     |Node|  |Node|  |Node|     |Node|(9)
 *     +----+  +----+  +----+     +----+
 *       (4)     (5)     (6)
 *
 * It derives from sf::Drawable as it is suppossed to be drawn on the screen.
 * It derives from sf::Transformable which gives all members related with position, rotation and scale
 * It derives from sf::NonCopyable as NodeScene like this should not be copied (it may give many problems in current state)
 * This make copy constructor/assignment to be deleted
 */
class NodeScene : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
public:
	friend class GameplayManager;
	// It will use dynamic linking to it's
	// derived classes
	using Node = std::unique_ptr<NodeScene>;

	NodeScene();
	virtual ~NodeScene() override = default;

	/**
	 * \brief Steals ownership, and puts it into the vector of pinned nodes
	 * \param node node to steal
	 *
	 * After this function, the "node" is a pinned node to this node.
	 * The node that uses this function becomes the parent of the "node".
	 */
	void pinNode(Node node);

	/**
	 * \brief Removes this NodeScene from pinnedNodes and returns it
	 * \param node node to unpin
	 * \return unique_ptr to the unpinned node
	 *
	 * Removes this NodeScene from pinned_Nodes and returns it.
	 * After this operation, this node is no longer a parent "node".
	 */
	Node unpinNode(const NodeScene& node);

	// Returns absolute position in the game plane
	
	/**
	 * \brief A function that calculates the absolute position of this node.
	 * \return Returns the absolute position of the node object on the screen.
	 */
	sf::Vector2f getAbsolutePosition() const;

	
	// ====== Drawing Scenes ====== //

	/**
	 * \brief Draws this node, and all pinned nodes to the passed target.
	 * \param target where this node, and all pinned nodes should be drawn to.
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 *
	 * This function is provided inside sf::Drawable. Thanks to this if we pass this object to
	 * sf::RenderWindow::draw(), then it will implicitly call this function to draw it!
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override final;

	
	/**
	 * \brief Draws only this node to the passed target
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	virtual void drawThis(sf::RenderTarget& target, sf::RenderStates states) const;


	// ====== Updating Scenes ====== //

	/**
	 * \brief Updates the object logic of this node and the pinned nodes.
	 * \param deltaTime the time that has passed since the game was last updated.
	 *
	 * In comparison to draw() this function is not derived. It is used to update all pinnedNodes
	 */
	virtual void update(sf::Time deltaTime);

	/**
	 * \brief Updates the logic of this node only
	 * \param deltaTime the time that has passed since the game was last updated
	 *
	 * Works analogues to the drawThis(), updates all things related to itself
	 */
	virtual void updateThis(sf::Time deltaTime);

	
	/**
	 * \brief Deletes pinned nodes that are marked as deleted.
	 * Performs operations for lower nodes as well.
	 */
	virtual void removeDestroyed();

	
	// ====== Handling events ====== //
	
	/**
	 * \brief It takes input (event) from the user and interprets it, then passes it on to the pinned nodes
	 * \param event user input
	 */
	void handleEvents(const sf::Event& event);

	/**
	 * \brief It takes input (event) from the user and interprets it
	 * \param event user input
	 */
	virtual void handleThisEvents(const sf::Event& event);


	// ==== Tells if object should be destroyed ==== //

	/**
	 * \brief Checks whether the object is to be deleted
	 * \return True if the object should be deleted, false if not
	 */
	virtual bool isDestroyed();


	/**
	 * \brief Indicates the object as ready for deletion
	 */
	virtual void setDestroyed();

protected:
	
	/**
	 * \brief Recursively finds the root node
	 * \return the root node
	 */
	virtual NodeScene* getRootNode();

	/**
	 * \brief Recursively finds the root node
	 * \return the root node
	 */
	virtual const NodeScene* getRootNode() const;

private:

	/**
	 * \brief A list holding all the nodes attached (children) to this node
	 *
	 * It was vector before, but I had to change it to the list, as some object
	 * inside range-for loop of this pinnedNodes started pin its nodes to this
	 * global pinnedNodes and that was risky that pinnedNodes may reallocate
	 * during its iteration of the range-for loop
	 *
	 * What objects started to pin some object to the root_node?
	 * Actually bullets, I wanted them to be global, thus I put
	 * them in the root node
	 */
	std::list<Node> pinnedNodes;

	/**
	 * \brief Parent of this node. The node to which this node is attached.
	 *
	 * Nullptr if this node is a root node.
	 */
	NodeScene* parent;

	/**
	 * \brief Indicates if the object is to be destroyed
	 */
	bool destroyed = false;
};

#endif
