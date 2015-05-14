
# Post-mortem evaluation

This project, as all game projects, was fraught with developmental perils.

## Technical difficulties

Many development goals were cut back or dropped for a number of reasons. One
major difficulty proved to be the decision to use Unreal Engine 4 (UE4).
Although it is a magnificent tool that obviated our need to implement 3D
graphics, the game loop, multi-platform support, and input, one of us had
trouble getting it to run on a PC as originally planned, delaying initial
development.

![This function no docstring](sodoweusethisor.jpg?raw=true)

Documentation provided for UE4 was iffy for new features we were interested
in using, and often incomplete for C++ portions compared to Blueprint (visual
coding) counterparts. This meant we did not use as much C++ as we were hoping.

![This code draws the reticle in the middle of the screen.](spaghetti.jpg?raw=true)

Blueprints were good for rapid iteration. Many UE4 features are immediately
accessible and can be added by typing what you think you need. However, the
code we constructed tended to become unwieldy and difficult to navigate. Simply
having several parameters to a function or combining multiple arithmetic terms
could easily create significant visual clutter, reducing the effectiveness of
the visual paradigm. It happened at a small enough scale that abstracting
features into functions didn't seem to help. Moreover, we had some incidents
of clashing commits that could not be avoided due to using Blueprints, and a
merging tool not being ready for general, worry-free use with Git as of UE 4.7.

## Non-technical difficulties

Our project was a bit low on art assets. We overestimated how much art we could
make or get, and ended up having a very sparse look to the game due to minimal
assets. Due to time constraints, some minutes of music and sound were planned
but not implemented.

Many of our intended designed features could not be or just were not
implemented. In part, we had to learn the engine from scratch, and in part, we
had to manage time as students. In an open-ended project, it is easy to push
it off in favor of a technically challenging project for another class with
more concrete expectations. It could be argued some elements were out of scope
for two students, but the true villains here are the usual: time, motivation,
and skill.

The game we created matches many of our intended features in a checklist sense.
It is less so in that we wanted it to be easy to pick up and enjoy. We did
become aware that the controls were not very intuitive, and that navigation, a
core part of the gameplay, was difficult for a number of reasons including a
lack of visual reference in 3D space. In later classes, nobody was willing to
give new features a test run.

## Successes

Using source control was a definite plus. This allowed a level of collaboration
and mistake-making that does not exist without. For many student projects, a
public repository is not usually the way to go, but it was not a concern for
this project. Both of us got access to Unreal Engine through GitHub's Student
Developer Pack shortly before it went free for all.

Speaking of that, Unreal Engine was a good pick. Despite our difficulties in
picking up the engine, the experience we picked up in these growing pains was
invaluable and we look forward to creating new projects using this engine. It
appears to be gaining widespread currency, and it has taught us a few things
about game engines, such as when we researched its brand of serialization.

Milestone feedback was helpful in assessing the next step of development. We
were not always able to act on it, but sometimes you need to know when you are
barking up the wrong tree.

# Future Plans

To present a complete game, we would like to add some features such as tutorial
levels, a more informative HUD, and better-tuned controls.

We would also like to add more meaningful customization by visually
distinguishing choices. Adding different types of missions and varying the
rewards might make the game more interesting. It should inspire a better sense
of player accomplishment.

We could re-tune the combat to more closely match our original vision. Right
now, landing very difficult shots, ramming ships, or abusing missile tracking
are the only realistic options for players to defeat enemies.

The original intent for this game was to be playable with a controller. Right
now, controls using a controller are very limited and anywhere near as easy
as using a mouse. Controls are also currently hardcoded in and cannot be
customized. If we continue this project very far, that has to go and we're not
afraid to say it.

We hope to have more art to serve the game's needs. It will not solve all
gameplay issues. It will solve some confusion about 3D navigation, however.
No matter how impressive a game is or isn't, it's always more impressive with
better art.

A few minutes of music were composed for this project. The music is being
prepared but will only be ready after the cutoff. This will be more exciting
for players.
