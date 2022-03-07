# Entity Component System

This is the architecture that I chose for this project. Why? Dunno it seemed kinda cool and I found the one and only library that's used in Minecraft (of course talking about [entt](https://github.com/skypjack/entt)).

If you don't how this architecture works and shit just go read up man not gonna hold your hand here just explaining how I implemented it.

## Entities and Components

I wrote my components how I like them - MINIMAL and without any BLOAT. They are simple structures with a few members, sometimes even a single one or none (I call them marker components). They don't have any functions and their members are either primitive types, strings or enums. I try not to put anything big and clunky in there since they're gonna be shuffled around a lot so they're small and tiny uwu.

I also try to separate them into, well not as many as possible, but into more than some people would prefer, just to keep them small: e.g. `Position` and `Hitbox` - I could've bundled them up into a single component and it maybe would've been simpler to use in the systems but what if something has a `Position`, but no `Hitbox`? Yeah.

I had to make an exception to my minimalist rule with `Sprite` - another way to make that would be a `Sprite` and an `AnimatedSprite` component, but then I'd need to remove and add them when for an example the player starts walking, or a breakable wall breaks. I'd rather switch a bool and set some values than do that.

## Systems

All of my systems derive from the base `System` class, which only contains a reference to the `entt::registry`, the `update()` function and the stuff used for pausing and resuming. On top (or bottom) of this tree I've got another tree - `RenderingSystem`s. These, besides the stuff in `System`, contain a reference to the `sf::RenderWindow`. Those are the main 2 types of `System`s that I use. Separating those 2 types allows for separation of updating and rendering, thanks to which I can pause the game by not calling the main `update()` function, but still calling `render()`. The systems are separated by just making 2 vectors, one of `System` and the other of `RenderingSystem` (I had a lot of fun with them with my lack of knowledge about pointers btw). For each update I pass the deltaTime, window scale and bool switch to each of the systems and I know I could just pass in a reference to those from `Game` in the constructor but since I have way more experience with Java than C++ I did it this way and I think now it's too late to refactor, it's not that big of a deal anyway.

Because of the design of the `System`s I can easily refactor them, for an example if I notice one `System` is getting really big or doing 2 separate things instead of one, I take its work and create a new `System` to handle it. In the way I designed `System`s, you can again clearly see my minimal and BLOATLESS philosophy.

I've also seen some people implement systems by just making standalone functions in separate headers, and then calling them when necessary. Again, considering my background with Java I haven't even considered that before seeing it. I think it also would be kinda tedious to call each single one of these functions instead of just iterating over a vector and calling a single function on each system, but what do I know.
