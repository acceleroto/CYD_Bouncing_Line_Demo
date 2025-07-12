# CYD Line Bounce

An animated line bouncing demo for the Cheap Yellow Display (CYD) ESP32-2432S028R. This project creates a dynamic screen saver-style animation with bouncing lines that leave colorful trails.

## 🎯 What It Does

This Arduino sketch animates a line with two endpoints that bounce around the screen, leaving a gradient trail behind it. The animation features:

- **Bouncing Physics**: Lines bounce off screen boundaries with randomized velocities
- **Trail Effect**: Smooth gradient trail that fades from blood red to candy apple red
- **High Performance**: Runs at maximum frame rate with no artificial delays
- **Dynamic Movement**: Random starting positions and velocity changes on bounces

![CYD Bouncing Line screenshot](https://github.com/acceleroto/CYD_Bouncing_Line_Demo/blob/main/screenshots/CYD_bouncing_lines_screengrab1.png "CYD Bouncing Line Demo")


**📺 Watch it in action**: [CYD Line Bounce Demo](https://www.youtube.com/watch?v=ihmrLsRpidA)

## 🖥️ Hardware Requirements

- **Display**: Cheap Yellow Display (CYD) ESP32-2432S028R
- **Resolution**: 320x240 pixels
- **Interface**: SPI TFT display
- **Microcontroller**: ESP32

## 🎨 Visual Features

### Trail System
- **5 Trail Lines**: Smooth interpolation between stored positions
- **Gradient Colors**: Blood red to candy apple red gradient
- **Smooth Movement**: All trail lines move fluidly with the main line
- **Configurable Spacing**: Adjustable trail spacing via `TRAIL_SPACING`

### Animation Physics
- **Random Bounces**: Velocity changes randomly on boundary collisions
- **Speed Range**: Configurable minimum/maximum velocities
- **Boundary Detection**: Proper collision detection and position correction
- **No Sticking**: Robust edge handling prevents lines from getting stuck

## ⚙️ Configuration

### Key Parameters
```cpp
#define TRAIL_LENGTH 5      // Number of trail lines
#define TRAIL_SPACING 3     // Frames between trail position updates
#define VELOCITY_MIN 5      // Minimum bounce velocity
#define VELOCITY_MAX 12     // Maximum bounce velocity
#define FRAME_DELAY 0       // Frame delay (0 = maximum speed)
```

### Color Customization
The trail colors can be easily modified in the `trailColors` array:
```cpp
uint16_t trailColors[TRAIL_LENGTH] = {
  0x6000, // Blood red - back trail line
  0xCA0A, // 
  0xFB0F, // 
  0xE004, // 
  0xF840  // Candy apple red - front trail line
};
```

## 🚀 Installation

1. **Connect Hardware**: Wire your CYD display to the ESP32
2. **Install Libraries**: Ensure TFT_eSPI library is installed with the correct User_Setup.h file. I followed this to get my CYD running the firt time with Arduino: https://randomnerdtutorials.com/cheap-yellow-display-esp32-2432s028r/
3. **Configure TFT_eSPI**: Set up the library for your specific display
4. **Upload Code**: Compile and upload to your ESP32
5. **Enjoy**: Watch the bouncing line animation!

## 🔧 Technical Details

### Trail System
- Uses circular buffer to store trail positions
- Interpolates between stored positions for smooth movement
- Updates trail positions every `TRAIL_SPACING` frames
- Handles frame counter overflow protection

### Performance
- Runs at maximum hardware speed (no artificial delays)

## 🎮 Customization

### Changing Colors
Modify the `trailColors` array to create different gradient effects:
- Use RGB565 color format
- Create custom gradients from any color to any color
- Experiment with different color combinations

### Adjusting Speed
- **Increase Speed**: Raise `VELOCITY_MAX`
- **Decrease Speed**: Lower `VELOCITY_MIN`
- **Change Trail Density**: Modify `TRAIL_SPACING`

## 🐛 Known Issues

- The 5th trail line (closest to main line) is currently disabled due to visual inconsistencies
- Some color combinations may not display correctly depending on display configuration
- High speeds may cause occasional visual artifacts

## 📝 Code Notes

This project was created as a "vibe coding" experiment with Cursor AI. The code is functional but not optimized for production use. It serves as a rough starting point for TFT display animations and can be extended with additional features.

## 🤝 Contributing

Feel free to fork this project and experiment with:
- Different trail effects
- Multiple bouncing lines
- Interactive controls
- Sound effects
- Network connectivity features


---

*Created for the Cheap Yellow Display community - enjoy your bouncing lines! 🎯* 