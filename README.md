`The Challenge`:

My team was tasked with building a robot that can perform two sets of tasks:

| Round 1               | Round 2                             |
| ----------------------| ----------------------------------- |
| - Cross a bridge      | - Follow a wall                     |
| - Navigate a maze     | - Climb up a ramp                   |
| - Locate salamanders  | - Measure the salinity of a solution|

The course the robot would navigate on was the following:

![Course Plan](./etc/course_diagram.PNG)

`Team Members`:

Here are the members of my team followed by the roles:

- Odran Fitzgerald (Bridge and soldering)
- Georges Li (Building)
- Andrew Pan (CAD)
- Nicolas Morin (Wiring and Building)
- Noah Alban (Building)
- Kassi Bertrand (Software design and implementation)

`Techonologies, Tools, Framework used (optional)`: 

- Arduino
- KNWRobot (a library made available to us by SMU staff)

`Results`:

The following 3D scan showcases the robot the team built.

![3D Robot Scan](./etc/Robot_Scan_3D.gif)

`Complications`:

In my perspective, the first problem I encountered was getting 
to callibrate the motor so it goes straight. I spent significant
time guessing and checking hoping to find the right set of 
values for motors. It did not work. 

The first serious attempt was to use _encoders_, like the one below:

![Encoder image](./etc/Encoders.jpg)

We tried to use encoders and create P.I.D. control loop
to dynamically adjust the speed of our motor.
Unfortunately, encoders were "too big" in a sense
that adding them to the robot will make it go over
the 20 x 20 x 20 cm size requierment my team was aiming
for.

The second idea was to use _solid axle_ for the wheels. The arrangement looked liked
this:

![Solid axle](./etc/Solid_Axle.jpg)

Though using the encoders failed, the idea forced 
my team to come up with a new arrangement for the 
DC Motors. Instead of having four independent motors, we 
used a single axle, controlled by one DC motor, to control 
two wheels at the same time. 
The rationale was that such arrangement
would reduce guarantee that the wheels attached to the solid axle have the same RPM.

The idea was conceptually interesting, but failed for two reasons:

- _Uneven weight distribution_: The robot had a tendency to veer in one direction, when moving.

- _Weak DC motors_: The motors needed to turn very fast to turn the gears.

_Note: a lot og guesses and checks went into finding the right dimensions for the gears. That resulted in a lot of "wasted" time_.


With the previous ideas failing, the team had to revert back to its original motor arrangement. Instead of guessing and checking, motor calibration problem was handled using the following piece of code:

```cpp
for(int i = 90; i < 115; i++>){
    robot -> pcaDC2MotorsTime(2, 108, 4, i, 3000);
}
```
We set a fixed value for the left motor (`ID: 2`), and using a `for` loop, we continously assigned values to the right motor (`ID: 4`), and observed robot deviation after 3 seconds. We realized the robot was
getting straighter. So, we kept the value that matched the previously choosen value.

_Note: Tests were performed at 13.3V_

`Takeaways`:

I am proud of this experience because it is something I have never done before. Here are the few takeaways:

- _It's okay to ask for help_. I understood that there are no shame in asking for help. The TAs were really helpful and ready to dedicate time for my team and other teams as well.

- _I cannot do everything on your own_. This experience taught me the importance of relying on others. Trust that they will play their part as intended, so I can play mine as well.

`Potential Improvements`:

If I could do this over again, I would build a bigger robot.