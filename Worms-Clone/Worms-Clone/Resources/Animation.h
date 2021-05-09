#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

/**
 * \brief A class that allows to create animated sprites based on a passed in spritesheet.
 */
class Animation final : public sf::Drawable, public sf::Transformable
{
public:
	Animation(const sf::Texture& spriteSheet, sf::Vector2i frameSize, std::size_t frameAmount, sf::Time animationLength);

	/**
	 * \brief Sets the animation as looped/repeatable or unlooped/non-repeatable.
	 * Looped animation, if the frames end, starts displaying the animation again from the first frame.
	 *
	 * \param isRepeatable True, if the animation is to loop/repeat, false otherwise
	 */
	void setRepeatable(bool isRepeatable) noexcept;

	
	/**
	 * \brief Sets the size of a single animation frame. These dimensions are: width and height.
	 * \param frameSize width, and height of the single animation frame
	 */
	void setFrameSize(sf::Vector2i frameSize) noexcept;

	/**
	 * \brief Sets the number of frames the animation has.
	 * The number of frames should be equal to the number of frames contained in the spritesheet.
	 *
	 * \param frameAmount Number of animation frames
	 */
	void setFrameAmount(std::size_t frameAmount) noexcept;

	/**
	 * \brief Specifies how long you want the animation to take over time.
	 * \param animationLength Duration of animation
	 */
	void setDuration(sf::Time animationLength) noexcept;

	/**
	 * \brief Sets the animation to "reversing", meaning that if the last frame of the animation is displayed,
	 * the animation will start displaying backwards. This is another form of looping/repeating the animation.
	 *
	 * \param isReversing True, if it should reverse, false otherwise.
	 * TODO: Possibly a good idea in the future is for "reversing" to play the animation twice,
	 * TODO: once forward and then backward without looping. At the end of the backward display,
	 * TODO the animation should stop, unless the animation is set as "Repeatable".
	 */
	void setReversing(bool isReversing) noexcept;

	/**
	 * \brief Starts the animation from the beginning (from the first frame).
	 */
	void restartAnimation() noexcept;

	/**
	 * \brief Checks whether the animation has already finished
	 * \return True, if the animations has already ended, false otherwise
	 *
	 * \warning Throws a logic error if the user tries to check if a looping animation has finished.
	 * I recognise this as a logical error worthy of notification.
	 */
	bool isFinished() const;

	/**
	 * \brief Updates the animation state based on the specified time (for example, moving to the next animation frame).
	 * \param deltaTime Time elapsed since the last update
	 */
	void update(sf::Time deltaTime);

protected:
	/**
	 * \brief Draws the animated sprite.
	 * \param target where this animated sprite should be drawn to.
	 * \param states provides information about rendering process (transform, shader, blend mode)
	 */
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	// === Animation === //
	sf::Sprite sprite; //!< Sprite to which subsequent frames of animation are displayed
	sf::Time animationLength; //!< Duration of the animation (since beginning to the end)
	sf::Time timeElapsed = sf::seconds(0); //!< The elapsed time since the previous animation frame 
	bool isRepeatable = false; //!< Flag informing if the animation looped/repeatable
	bool isReversing = false; //!< Flag informing if the animation reversing -- in other words looped by playing backwards when the animation ends.

	// === Frames === //
	sf::Vector2i frameSize; //!< Size of a single animation frame (as well as of the final sprite)
	std::size_t frameAmount; //!< Number of animation frames
	std::size_t frameCurrent = 0; //!< Currently displayed frame
	sf::Time timePerFrame; //!< Time spent per animation frame
	bool reverseDirection = false; //!< Flag indicating whether the animation is displayed backwards
};

#endif