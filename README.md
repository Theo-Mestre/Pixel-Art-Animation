# Pixel-Art-Animation

## Overview
Pixel-Art-Animation is a C++ project using SFML that simplifies the process of creating and managing animations with a single texture. The project includes:

1. **Animation Mapping System**: A method to map texture coordinates to animations, ensuring that all animations can reference a single texture. If the texture is modified, all animations are automatically updated.
2. **AnimatedSprite Class**: A reusable class for working with mapped animations, making it easy to integrate animations into your projects.
3. **Editor Tool**: An editor built with a custom SFML-based UI library (currently in development) to facilitate the creation and adjustment of texture coordinate mappings.

### Known Issues
- **Editor Bugs**:
  - Button hitboxes do not take their parent's transforms into account, which may result in incorrect click behavior.

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

#### Editor
Run the editor to create or modify animation mappings for your texture. Once completed, save the mappings to integrate them into your game or project.

## Contributing
Contributions are welcome! Feel free to submit issues or pull requests to help improve the project.

## License
This project is licensed under the [MIT License](LICENSE).

## Acknowledgments
- [**SFML**](https://www.sfml-dev.org/)

