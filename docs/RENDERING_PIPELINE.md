# Rendering pipeline

The original game renders both 2d and 3d elements to the same memory buffer, called `gBack_screen`. 

Another variable, `gRender_screen`, points into that memory buffer and is where the 3d scene is drawn on top of any existing 2d pixels.

Rendering is done in a (standard for the time) 8 bit paletted mode.

```
+-----------------------------------------+
|gBack_screen                             |
|      +------------------------+         |
|      | gRender_screen         |         |
|      |                        |         | 
|      |                        |         |
|      |                        |         |
|      |                        |         |
|      +------------------------+         |
|                                         |
+-----------------------------------------+
```

The `RenderAFrame` function does the following:

1. Render 2d background content (horizon, map, etc) to `gBack_screen`
2. Start 3d scene rendering
3. Render 3d environment to `gRender_screen`
4. End 3d scene rendering
5. Render 2d foreground content into `gBack_screen` (HUD, messages, etc)
6. Swap buffers

If the rearview mirror is rendered, steps 3-4 are repeated, this time rendering into `gRearview_screen`

## Palette manipulation

The game palette is updated frequently:
1. Fade screen to black
2. Fade screen back up to normal brightness
3. Different palettes for the menu interface and for game play
4. The "On drugs" powerup

Palette animations run in a tight loop, assuming they are writing to the system color palette, so do not re-render the 3d scene etc. We handle this by hooking the palette functions and reusing the last-rendered scene from our framebuffer.

## OpenGL implementation

### Start 3d rendering hook
- Capture the current `gBack_screen` and convert it to a 32 bit OpenGL texture, and render it as a full-screen quad.
- Configure OpenGL framebuffer to do render-to-texture 
- Clear `gBack_screen`

### Render model hook
- Render the model as an OpenGL VBO, convert referenced materials to OpenGL textures.

### End 3d rendering hook
Render the framebuffer from above as a full-screen quad.

### Swap buffers hook
- Again capture `gBack_screen` to pick up HUD elements rendered after the 3d scene, convert it to 32 bit, and render it as a full-screen quad.
- Generate a palette-manipulation image which is blended over the top of everything as a full-screen quad to handle palette animations.
