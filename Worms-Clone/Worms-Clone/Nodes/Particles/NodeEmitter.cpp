#include "NodeEmitter.h"

NodeEmitter::NodeEmitter(NodeParticle& particle, float emissionPerSecond) : particleToEmit(particle), emissionPerSecond(emissionPerSecond)
{
	// Nothing to find here ._.
}

void NodeEmitter::updateThis(sf::Time deltaTime)
{
	// I want to emit a signal requesting a new particle every interval specified.
	// In other words for example, for a 60 emissions per seconds it is an interval
	// of one particle per 0.1(6) second...
	const auto interval = sf::seconds(1.f) / emissionPerSecond;
	timeSinceUpdate += deltaTime;

	// This loop allows you to control a certain regularity in this task
	while(timeSinceUpdate > interval)
	{
		timeSinceUpdate -= interval;
		particleToEmit.addParticle(getAbsolutePosition());
	}
}
