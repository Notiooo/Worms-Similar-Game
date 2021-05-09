#include "Animation.h"



#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

Animation::Animation(const sf::Texture& spriteSheet, sf::Vector2i frameSize, std::size_t frameAmount,
                     sf::Time animationLength) :
sprite(spriteSheet),
animationLength(animationLength),
frameSize(frameSize),
frameAmount(frameAmount)
{
	timePerFrame = animationLength / static_cast<float>(frameAmount);
}

void Animation::setRepeatable(bool isRepeatable) noexcept
{
	this->isRepeatable = isRepeatable;
}

void Animation::setFrameSize(sf::Vector2i frameSize) noexcept
{
	this->frameSize = frameSize;
}

void Animation::setFrameAmount(std::size_t frameAmount) noexcept
{
	this->frameAmount = frameAmount;
	timePerFrame = animationLength / static_cast<float>(frameAmount);
}

void Animation::setDuration(sf::Time animationLength) noexcept
{
	this->animationLength = animationLength;
	timePerFrame = animationLength / static_cast<float>(frameAmount);
}

void Animation::setReversing(bool isReversing) noexcept
{
	this->isReversing = isReversing;
	if (!isReversing)
		reverseDirection = false;
}

void Animation::restartAnimation() noexcept
{
	frameCurrent = 0;
	reverseDirection = false;
	timeElapsed = sf::Time::Zero;
}

bool Animation::isFinished() const
{
	if (isRepeatable || isReversing)
		throw std::logic_error("The looped animations never end");
	
	if (frameCurrent >= frameAmount)
		return true;
	return false;
}

void Animation::update(sf::Time deltaTime)
{
	timeElapsed += deltaTime;
	const auto textureSize(sprite.getTexture()->getSize());
	auto textureRect = sprite.getTextureRect();

	if(frameCurrent == 0)
		textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);

	while(timeElapsed >= timePerFrame && (frameCurrent <= frameAmount || isRepeatable || isReversing))
	{
		timeElapsed -= timePerFrame;

		// If the animation is played forward, I display the following frames on the right
		// If the frames on the right are missing, I move to the first frame (far left) a level down.
		if (!reverseDirection)
		{
			textureRect.left += textureRect.width;

			if (textureRect.left + textureRect.width > textureSize.x)
			{
				textureRect.left = 0;
				textureRect.top += textureRect.height;
			}
		}
		// If I display the animation backwards, I display consecutive frames to the left of the current frame.
		// If the frame on the left is missing, I display the frame on the far right of the level above.
		else
		{
			textureRect.left -= textureRect.width;

			if (textureRect.left < 0)
			{
				textureRect.left = textureSize.x - textureRect.width;
				textureRect.top -= textureRect.height;
			}
		}

		if (isRepeatable || isReversing)
		{
			if(isReversing)
			{
				if (frameCurrent == frameAmount-1)
					reverseDirection = true;

				if(frameCurrent == 0)
					reverseDirection = false;


				// === Be carefull to set the new textureRect before frame incrementation. === //
				
				if (frameCurrent == 0)
					textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);

				if (frameCurrent == frameAmount-1)
					textureRect = sf::IntRect(textureSize.x - textureRect.width, textureSize.y - textureRect.height, frameSize.x, frameSize.y);

				// === The incrementation part === //
				if (reverseDirection)
					--frameCurrent;
				else
					++frameCurrent;
			}
			else
			{
				frameCurrent = (frameCurrent + 1) % frameAmount;

				if (frameCurrent == 0)
					textureRect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
			}
		}
		else
		{
			++frameCurrent;
		}
	}
	sprite.setTextureRect(textureRect);
	
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{	
	target.draw(sprite, states);
}
