#include "NodeParticle.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


NodeParticle::NodeParticle(const sf::Texture& texture) : texture(texture)
{
	vertexArray.setPrimitiveType(sf::Quads);
}

void NodeParticle::addParticle(sf::Vector2f pos)
{
	Particle particle;
	particle.position = pos;
	particle.color = particleColor;
	particle.lifeTime = particleLifeTime;

	if(fadingLast.isActivated)
	{
		// The closer it get to the end the less visible the next particles are.
		if (fadingLast.endTime > fadingLast.currentTime)
		{
			particle.color.a *= static_cast<sf::Uint8>((fadingLast.endTime - fadingLast.currentTime).asSeconds() / fadingLast.endTime.asSeconds());
		}
		// Until finally the time is exceeded and new particles are not drawn at all.
		else
		{
			return;
		}
	}

	particles.push_back(particle);
}

void NodeParticle::setParticleLifeTime(const sf::Time time) noexcept
{
	particleLifeTime = time;
}

void NodeParticle::setParticleColor(const sf::Color color) noexcept
{
	particleColor = color;
}

void NodeParticle::setFadingOut(const sf::Time time) noexcept
{
	fadingOut.isActivated = true;
	fadingOut.endTime = time;
}

void NodeParticle::setDefaultOpacity(const float opacity) noexcept
{
	defaultOpacity = opacity;
}

void NodeParticle::setFadingLasts(const sf::Time time) noexcept
{
	fadingLast.isActivated = true;
	fadingLast.endTime = time;
}

void NodeParticle::setParticleInfiniteLifeTime(const bool flag) noexcept
{
	infiniteParticleLifeTime = flag;
}

void NodeParticle::updateThis(sf::Time deltaTime)
{
	while (!particles.empty() && particles.front().lifeTime <= sf::Time::Zero)
		particles.pop_front();

	if (!infiniteParticleLifeTime)
	{
		for (auto& particle : particles)
			particle.lifeTime -= deltaTime;
	}

	if (fadingOut.isActivated)
		fadingOut.currentTime += deltaTime;

	if(fadingLast.isActivated)
		fadingLast.currentTime += deltaTime;
	

	shouldVertexUpdate = true;
}

void NodeParticle::addVertex(float worldX, float worldY, float texCoordX, float texCoordY, const sf::Color& color) const
{
	sf::Vertex vertex;
	vertex.position = sf::Vector2f(worldX, worldY);
	vertex.texCoords = sf::Vector2f(texCoordX, texCoordY);
	vertex.color = color;

	vertexArray.append(vertex);
}

void NodeParticle::computeVertices() const
{
	// Data needed to draw vertices as centred to the drawing point.
	auto size = static_cast<sf::Vector2f>(texture.getSize());
	auto half = size / 2.f;

	// We are creating new data, so these can be deleted
	vertexArray.clear();

	// Each particle is, of course, already created and saved.
	for(auto& particle : particles)
	{
		auto& pos = particle.position;
		auto& color = particle.color;

		// In the case of finite lifetimes, it has to start making the
		// particles transparent -- in other words creating a fading effect.
		if (!infiniteParticleLifeTime)
		{
			auto ratio = particle.lifeTime.asSeconds() / particleLifeTime.asSeconds();
			color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.f));
		}
		// Otherwise, they should always be fully visible.
		else
		{
			color.a = 255;
		}

		// In case of disappearance of all particles, the time to disappearance and the corresponding transparency shall be determined
		if (fadingOut.isActivated)
		{
			if (fadingOut.endTime > fadingOut.currentTime)
			{
				color.a = static_cast<sf::Uint8>(static_cast<float>(color.a) * (fadingOut.endTime - fadingOut.currentTime).asSeconds() / fadingOut.endTime.asSeconds());
			}
			else
			{
				color.a = 0;
			}
		}

		// Finally, the maximum possible opacity is applied. Of course, it does not define boundaries,
		// but defines a certain starting point in transparency.
		color.a = static_cast<sf::Uint8>(static_cast<float>(color.a) * defaultOpacity);

		if(color.a == 0)
		{
			// TODO: This part here forces the mutable!
			// As I want this particle to disappear when it is already transparent
			particle.lifeTime = sf::Time::Zero;
		}
		else
		{
			// Finally, vertexes are drawn in a way:
			//
			//    1 ------- 2
			//    |         |
			//    |    x    |
			//    |         |
			//    4 ------- 3
			//
			// Relative to the drawing point x

			addVertex(pos.x - half.x, pos.y - half.y, 0.f, 0.f, color);
			addVertex(pos.x + half.x, pos.y - half.y, size.x, 0.f, color);
			addVertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
			addVertex(pos.x - half.x, pos.y + half.y, 0.f, size.y, color);
		}
	}
}

void NodeParticle::drawThis(sf::RenderTarget& target, sf::RenderStates states) const
{
	// To optimise calculations, the update is done every drawn frame -- to avoid
	// calculating multiple updates before a frame is even drawn.
	if (shouldVertexUpdate)
	{
		computeVertices();
		shouldVertexUpdate = false;
	}
	states.texture = &texture;
	target.draw(vertexArray, states);
}
