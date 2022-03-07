# The Game

Yes, you guessed it - in today's episode of The Docs we're talking about the `Game` class. Who would've thought.

I think this is kinda a standard way to do things, at least in smaller games. A single `Game` class containing pretty much the whole game state and acting as the game loop. Some of the functions are identical to the ones found in `System`s, the main culprits being `update()`, `render()`, `pause()` and `resume()`, calling the `update()`, function on respectively each `System` and each `RenderingSystem` and `pause()` and `resume()` on `System`s.

The `Game` also contains the `sf::Font` being used throughout the whole game and game-wide state, including but not limited to the debug, pause and focus switches and the window scale.

As the game grows, `Game` is also undoubtedly going to grow, but opposed to the `System`s I don't think it's that big of an issue. After all there's only one `Game`, but multiple `System`s.
