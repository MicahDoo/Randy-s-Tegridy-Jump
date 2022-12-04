# 2020-pd2-DoodleJump

## Video

Click the gif to watch the entire video.

[![](https://user-images.githubusercontent.com/57170797/203502021-44b04019-9897-4f91-896e-56d4c53cd3ed.gif)](https://www.youtube.com/watch?v=MqVeyzc9SXQ)


## How to play

Keys:

Left and Right Keys: control the direction the player (Randy Marsh) is going.
Up Key: shoots bullet (Mr Hankey) upwards.
Space Key: pauses the game.
Enter/Return Key: restarts the game after game is over.

## Requirements

1. Randy Marsh jumps after touching a platform while it's falling. He can also move accross the boundary.
2. Platforms: 1. Nomral: stationary, nothing special. 2. Elastic: makes Randy jump higher than a normal platform. 3. Horizontal: moving horizontally in a harmonic motion.
3. Props: 
    i. Butterfly wings: transforms Randy into a butterfly for a few seconds so that he can fly clear upwards. 
    ii. Tegridy Weed: relieves for a few seconds Randy's pain while jumping so that he has greater momentum (jumps higher)
4. Hazards: 
    i. Monsters: Satan or ManBearPig; both behaves the same. 
    ii. Bullets: in the form of Mr Hankey. (Up Key to generate) 
    iii. Monsters are dead (removed), upon touching the bullets. 
    iv. Another hazard: Eric Cartman at the right or left side of the screen shooting Member Berries. Player starts to hallucinates upong touching them, making the screen flipped upside down for a distance.
    v. Another way to shun hazards: The prop "Dead Kenny" puts a "Angel Kenny" upon Randy, acting as a proptective layer to shield Randy from any berry or monster attack.
5. The game goes on infinitely if Randy goes without touching the bottom, the monsters or the berries.
6. Press pause to pause the game.
7. The score shows on the upper left corner.
8. When Player loses, "Game Over" and the score slides up to the top of the screen.

## Polymorphism

The class "Platform" acts as a parent(parent in C++'s class inheritance system, not Qt's parent-child system) for all the types of platforms. In the class, there are two virtual functions: "virtual void behavior()" and "virtual void response()".
1. virtual void behavior(): it dictates how the platform behaves after it's creation, e.g. if it's a moving platform, the behavior() function makes it move periodically either horizontally or vertically.
2. virtual void response(): it dictates how the platform respond to Randy hopping onto it, e.g. if it's a normal or moving platform, response() makes Randy bounce upwards; if it's a spring platform, response() makes Randy bounce harder upwards; if it's a breakable platform, response() doesn't bounce Randy at all, in stead it changes the object's pixmap to a broken platform and falls down; if it's a spikey platform, it makes the player lose.
Both of these features are not the same for all types of plaform, so they should be made virtual can be specifically defined for each type of platforms. Right now, I have these classes inherited from Platform:

Normal: behaves normally, reponds by bouncing the player.

Spring: behaves normally, responds by bouncing the player hard.

## Bonus

Dark Mode: Once in a while the player comes into a well where no light gets to go in. Except a certain radius around the player, the screen is pitch black. This adds to the challenge of the game.

Flip Mode: The berries make the screen flip upside down, making the game more challenging.

Background music.

Sound effects for hazards and props.

Background have layers (tree, mountains, sky) that move down at different speeds, creating a 3D effect.

Game restartable after losing.
