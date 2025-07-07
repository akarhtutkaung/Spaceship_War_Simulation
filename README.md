# Space-Warefare

## Summary
Simulation of the smart spaceships capable of shooting down enemy spaceships within its field of view by predicting their future position. The spaceship also has the ability to dodge and trick enemy spaceships if they detect they are being followed and shot at. The project was implemented using C++ programming language in Unreal Engine 5, and the design includes a self-destruct feature for the spaceship if its health drops below a certain threshold.

## Track and Shooting
When an enemy spaceship enters the field of view, the spaceship will track, predict its movements, and take action to shoot it down.

![Chase-Shoot-Dodge](https://user-images.githubusercontent.com/76828992/217697955-d10f8d49-092c-4dd7-b6a4-6a6c5b36545b.gif)

## Dodge

Upon detection of incoming enemy fire, the spaceship will initiate evasive maneuvers to dodge the bullets.

![Dodge](https://user-images.githubusercontent.com/76828992/217697990-491dd3be-1b1b-4ec0-ad27-9b60baa945f5.gif)

## Explosion

The spaceship is programmed to explode upon impact with other spaceships or sustained damage from multiple enemy hits.

![Collide-Explode](https://user-images.githubusercontent.com/76828992/217698035-3dc622f1-f547-4bb8-9e2f-91724f34fcde.gif)

![Shot-Explode](https://user-images.githubusercontent.com/76828992/217698038-591787ed-e7f4-40f1-84b8-76104a707473.gif)

## Development
Platform: Unreal Engine 5

Language: C++
