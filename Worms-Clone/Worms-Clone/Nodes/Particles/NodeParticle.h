#ifndef NODEPARTICLE_H
#define NODEPARTICLE_H
#include <deque>
#include <SFML/Graphics/VertexArray.hpp>

#include "../NodeScene.h"
#include "Particles.h"

/**
 * \brief A class that generates a particles at the indicated positions.
 * Such a particle depends on the texture passed to it. In other words,
 * it draws the texture in many ways, many times. It can be a fading trace
 * of a moving texture. With this you can do many effects like smoke effect.
 */
class NodeParticle final : public NodeScene
{
public:
	explicit NodeParticle(const sf::Texture& texture);

	
	/**
	 * \brief Draws a particle at the indicated position.
	 * \param position Position on which the particle should be drawn
	 */
	void addParticle(sf::Vector2f position);


	// === Setters === //
	
	/**
	 * \brief Sets how long a particular particle lives. After this time, the particle disappears (by continuous fading)
	 * \param time Time how long the particle lives
	 */
	void setParticleLifeTime(sf::Time time) noexcept;
	
	/**
	 * \brief Sets what of what colour the particles are
	 * \param color Color of the particles
	 */
	void setParticleColor(sf::Color color) noexcept;

	
	/**
	 * \brief Sets the time after which the whole node (all particles) disappears through a gentle fade.
	 * \param time Time to for the node to gently disappear.
	 */
	void setFadingOut(sf::Time time) noexcept;

	
	/**
	 * \brief Sets the maximum possible opacity that this node has.
	 * \param opacity Opacity from 0 - 1 (0 is invisible, 1 is perfectly visible)
	 */
	void setDefaultOpacity(float opacity) noexcept;

	
	/**
	 * \brief Sets how long particles are generated. Over time they become more transparent until they are no longer drawn.
	 * \param time Time to stop drawing by the increasing transparency of newer particles.
	 */
	void setFadingLasts(sf::Time time) noexcept;

	
	/**
	 * \brief A flag that controls whether particles should have eternal life or disappear over time.
	 * \param flag True if they should not disappear after time, false otherwise
	 */
	void setParticleInfiniteLifeTime(bool flag) noexcept;
	

private:
	/**
	 * \brief Updates the current status of all nodes and the object depending on the elapsed time
	 * \param deltaTime Time elapsed since the last frame
	 */
	void updateThis(sf::Time deltaTime) override;
	
	/**
	 * \brief Adds a vertex (one point out of four for particle) with the given properties to an
	 * array that holds the currently drawn points of particles (4 vertex per particle).
	 * 
	 * \param worldX Position x of this vertex on the game map/world
	 * \param worldY Position Y of this vertex on the game map/world
	 * \param texCoordX Position X on the texture for this vertex
	 * \param texCoordY Position Y on the texture for this vertex
	 * \param color Color for this vertex
	 */
	void addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const;

	
	/**
	 * \brief Clears the array containing all vertexes for particles, and refills it by
	 * re-computing vertexes from the available set of Particles (see deque particles).
	 */
	void computeVertices() const;

	
	/**
	 * \brief Draws all particles onto the screen
	 * \param target where it should be drawn to
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void drawThis(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	// === Particles === //
	const sf::Texture& texture; //!< The texture that this object draws as a particle

	// TODO: PLEASE CHANGE THIS MUTABLE IN THE FUTURE!
	// TODO: For now it is necessary for the "optimization" part to work correctly
	mutable std::deque<Particle> particles; //!< A collection of all particles which are then drawn onto the screen
	
	sf::Time particleLifeTime = sf::seconds(1); //!< The lifetime of a single particle after which it disappears.
	sf::Color particleColor = sf::Color::Black;
	
	// === Vertexes === ///
	mutable sf::VertexArray vertexArray; //!< A set of vertexes (a certain type of point) in which one rectangular
	                                     //!< figure drawing a particle texture is translated into 4 points.
	
	mutable bool shouldVertexUpdate = true; //!< Determine if the vertex set should be recalculated.

	// === Effects related === //
	float defaultOpacity = 1.f; //!< The maximum (default) opacity with which all particles are drawn (in the range 0 to 1).
	bool infiniteParticleLifeTime = false; //!< Determines whether the particles live forever or whether they expire after time
	
	/**
	 * \brief An effect that has a beginning and an end and can be activated.
	 * These are effects such as all particles fading.
	 */
	struct Effect
	{
		bool isActivated = false;
		sf::Time currentTime = sf::Time::Zero;
		sf::Time endTime = sf::Time::Zero;
	};
	Effect fadingOut; //!< An effect that makes the whole node (all particles) disappears through a gentle fade after given time.

	Effect fadingLast; //!< An effect that describes how long particles are generated.
	                   //!< Over time they become more transparent until they are no longer drawn.
};



#endif