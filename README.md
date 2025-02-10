# Pixel-Art-Animation

## Overview
Pixel-Art-Animation is a C++ project using SFML that simplifies the process of creating and managing animations with a single texture. The project includes:

1. **Animation Mapping System**: A method to map texture coordinates to animations, ensuring that all animations can reference a single texture. If the texture is modified, all animations are automatically updated.
2. **AnimatedSprite Class**: A reusable class for working with mapped animations, making it easy to integrate animations into your projects.
3. **Editor Tool**: An editor built with a custom SFML-based UI library (currently in development) to facilitate the creation and adjustment of texture coordinate mappings.

<img width="4242" alt="AnimationProcessDetails" src="https://github.com/user-attachments/assets/7615f787-e92b-400a-91bd-517624bac3d8" />

Sprites used in the demo can be found [here](https://craftpix.net/freebies/free-3-character-sprite-sheets-pixel-art/).

## Getting Started

All dependencies are included in the project so all you need to do is clone and build the project!

### Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/Theo-Mestre/Pixel-Art-Animation.git
   ```
3. Build the project:
   - Run the `Scripts/SetupUpSolution.bat` to create the project files using premake5

### Usage
#### AnimatedSprite Class
Include the `AnimatedSprite` class in your project to integrate animations. The class handles:
- Updating animation frames based on a timer.
- Rendering animations using mapped texture coordinates.

#### Example

```c++
#include "AnimatedSprite.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Animated Sprite");
	sf::Event event;
	sf::Clock clock;
	float deltaTime = 0.0f;

	Animation::AnimationData data
	{
		"AnimationName", // Animation file
		"TextureName", // Texture file
		{ 6, 1 }, // Frame number
		0.1f // Frame duration
	};
	Animation::AnimatedSprite sprite(data); // Create the animated sprite

	// The sprite can be initialized with the data after creation
	//sprite.initialize(data); 

	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
		}

		// Update the sprite
		sprite.update(deltaTime);

		window.clear();

		// Draw the sprite
		window.draw(sprite);

		window.display();
	}
}
```

#### Editor
Run the editor to create or modify animation mappings for your texture. Once completed, save the mappings to integrate them into your game or project.

The Editor is available [here](https://github.com/Theo-Mestre/Pixel-Art-Animation/releases), It's still in early stage so some features may be missing and some bug might be encountered.
There isn't any documention on the Editor yet. You can refer to the [figma board](https://www.figma.com/design/pwXmeGTGsy7Qp3yq8xyIjQ/Pixel-Art-Animator?node-id=0-1&t=cYlQ9sqwOG7hhH4U-1) to see what which buttons does. 

Figma Board: https://www.figma.com/design/pwXmeGTGsy7Qp3yq8xyIjQ/Pixel-Art-Animator?node-id=0-1&t=cYlQ9sqwOG7hhH4U-1

## Contributing
Contributions are welcome! Feel free to submit issues or pull requests to help improve the project.

## License
This project is licensed under the [MIT License](LICENSE).

## Acknowledgments
- [**SFML**](https://www.sfml-dev.org/)

