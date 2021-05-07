#ifndef NODEEMITTER_H
#define NODEEMITTER_H
#include "NodeParticle.h"
#include "../NodeScene.h"

/**
 * \brief This class only deals with emitting the signal of creating a particle in the position
 * in which it is located (in its position) to the particular NodeParticle to which it is connected.
 *
 * Depending on the NodeParticle to which the emitter is attached, particles are created in its position.
 * If the attached NodeParticle generates smoke, it will be smoke. If it is fire, it will be fire.
 */
class NodeEmitter final : public NodeScene
{
public:
	explicit NodeEmitter(NodeParticle& particle, float emissionPerSecond = 60.f);

	
	/**
	 * \brief Updates the state of the emitter depending on the elapsed time since the previous frame.
	 * \param deltaTime Time elapsed since the last frame
	 */
	void updateThis(sf::Time deltaTime) override;

private:
	sf::Time timeSinceUpdate = sf::Time::Zero; //!< Time since last update. Helps to keep the time by which the signal is emitted synchronised.
	NodeParticle& particleToEmit; //!< "Particle" to which a signal is emitted to create a new particle.
	const float emissionPerSecond; //!< Number of emissions requesting the creation of a particle at a given location per second.
};






#endif