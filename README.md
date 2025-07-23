# Space-Warfare

## Overview  
A smart spaceship simulation built in **Unreal Engine 5** using **C++**, featuring autonomous combat behavior such as predictive targeting, evasive maneuvers, and a self-destruct mechanism. The simulation models tactical engagement and survival strategies, emphasizing dynamic interactions between entities.

## Key Features

### 🎯 Target Tracking & Shooting  
When an enemy spaceship enters the field of view, the AI-controlled spaceship locks on, predicts the enemy's future position based on its velocity, and fires with precision.

![Chase-Shoot-Dodge](https://user-images.githubusercontent.com/76828992/217697955-d10f8d49-092c-4dd7-b6a4-6a6c5b36545b.gif)

### 🌀 Evasive Maneuvers  
Upon detecting incoming enemy fire or being pursued, the spaceship executes evasive actions designed to dodge bullets and confuse enemy targeting systems.

![Dodge](https://user-images.githubusercontent.com/76828992/217697990-491dd3be-1b1b-4ec0-ad27-9b60baa945f5.gif)

### 💥 Collision & Destruction  
The spaceship is programmed to explode under two conditions:  
- Direct collision with another ship.  
- Sustained damage beyond a critical health threshold.  

This adds realism to space combat scenarios and enforces tactical awareness.

![Collide-Explode](https://user-images.githubusercontent.com/76828992/217698035-3dc622f1-f547-4bb8-9e2f-91724f34fcde.gif)  
![Shot-Explode](https://user-images.githubusercontent.com/76828992/217698038-591787ed-e7f4-40f1-84b8-76104a707473.gif)

## Development Details  
- **Platform:** Unreal Engine 5  
- **Programming Language:** C++  
- **Tools & Assets:** Blender (for models), UE5 AI Behavior Trees  
