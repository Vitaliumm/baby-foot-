I've created an electronic foosball table that includes the following components:

2 Nextion screens, model: NX4024T032.

2 speed sensors, placed at each goal. Each speed sensor consists of 2 lasers and 2 laser sensors. When a goal is scored, the ball crosses the path of one laser and then the other. This allows me to calculate the time it takes for the ball to travel through the goal and thereby determine its speed.

4 RGB LED strips, each containing 40 LEDs.

8 push buttons on each side for controlling various functions: plus button, minus button, reset button, and LED button.

2 switches that are activated by the ball hitting the goal. These switches enable automatic scoring and trigger LED animations when a goal is scored.

To manage all these components, I'm using 2 separate Arduino Mega boards.Here's the breakdown of their responsibilities:

The main board handles:
- Control of the 2 Nextion screens.
- Processing input from the 6 buttons on each side (plus, minus, reset).
- Monitoring the 2 switches activated by the ball hitting the goal.

The secondary board manages:
-Control of the 4 RGB LED strips.
-Handling input from the 2 LED buttons on each side.
-Monitoring the 2 switches activated by the ball.

 
