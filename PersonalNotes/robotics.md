---
layout: default
title: mobile robotics notes
---

## Aim: How can a mobile robot move unsupervised to fulfill a set task?

Aspects to consider:
- Low level locomotive ability
- Perception
- Localization
- Cognition, decision making

### Locomotive ability

This refers to the low level mechanism for movement, ie. walk, jump, fly, swim, etc...

Ie. legs vs wheels:
The railway is ideally engineered for wheeled locomotion because rolling friction is minimized on a hard and flat steel surface. But as the surface becomes soft, wheeled locomotion accumulates inefficiencies due to rolling friction whereas legged locomotion suffers much less because it consists only of point contacts with the ground.


### Kintematics

- Kinematics is the most basic study of how mechanical systems behave.
-  In mobile robotics, we need to understand the mechanical behavior of the robot both in order to design appropriate mobile robots for tasks and to understand how to create control software for an instance of mobile robot hardware.

> Since a mobile robot is a self-contained automaton that can move with respect to its environment, unlike manipulators such as fixed arms, position estimation is difficult.

Deriving a model for the whole robot’s motion is a bottom-up process. Each individual wheel contributes to the robot’s motion and, at the same time, imposes constraints on robot
motion.

### Perception

One of the most important tasks of an autonomous system of any kind is to acquire knowledge about its environment. This is done by taking measurements using various sensors and
then extracting meaningful information from those measurements. 

**Common sensors used in mobile robotics and information extracted**

Sensors are classified as proprioceptive/exteroceptive and passive/active.
> Proprioceptive sensors measure values internal to the system (robot); for example, motor speed, wheel load, robot arm joint angles, battery voltage.
> Exteroceptive sensors acquire information from the robot’s environment; for example, distance measurements, light intensity, sound amplitude.

Hence exteroceptive sensor measurements are interpreted by the robot in order to extract meaningful environmental features.

> Passive sensors measure ambient environmental energy entering the sensor. Examples of passive sensors include temperature probes, microphones, and CCD or CMOS cameras.
> Active sensors emit energy into the environment, then measure the environmental reaction.

Because active sensors can manage more controlled interactions with the environment, they often achieve superior performance.


However, active sensing introduces several risks:
The outbound energy may affect the very characteristics that the sensor is attempting to measure. Furthermore, an active sensor may suffer from interference between its signal and those beyond its control. For example, signals emitted by other nearby robots, or similar sensors on the same robot, may influence the resulting measurements. Examples of
active sensors include wheel quadrature encoders, ultrasonic (distance?) sensors, and laser rangefinders.

![Sensors in mobile robotics](https://github.com/selin-k/selin-k.github.io/blob/main/PersonalNotes/media/sensors.png?raw=true)

> Characterizing sensor errors in a challange in mobile robotics.


### Localization

Navigation is one of the most challenging competences required of a mobile robot. Success in navigation requires success at the four building blocks of navigation: 
- **perception**, the robot must interpret its sensors to extract meaningful data.
- **localization**, the robot must determine its position in the environment.
- **cognition**, the robot must decide how to act to achieve its goals.
- **motion control**, the robot must modulate its motor outputs to achieve the desired trajectory.

> Localization has recieved relatively greater attention and advances have been made in the past decade. 

![Mobile robot localization](https://github.com/selin-k/selin-k.github.io/blob/main/PersonalNotes/media/localization.png?raw=true)

**effector vs actuator**
An effector is any device that affects the environment. Robots control their effectors, which are also known as end effectors. Effectors include legs, wheels, arms, fingers, wings and fins. Controllers cause the effectors to produce desired effects on the environment.
An actuator is the actual mechanism that enables the effector to execute an action. Actuators typically include electric motors, hydraulic or pneumatic cylinders, etc. The terms effector and actuator are often used interchangeably to mean "whatever makes the robot take an action." - which is actually incorrect to do.

ie.
Two basic ways of using effectors:
- to move the robot around => locomotion
- to move other object around => manipulation


**Successful localization methodologies of recent years**

**how sensor and effector uncertainty is responsible for difficulties of localization**
- While GPS alleviates much of the localization problem there are still issues with limited network space (indoors/obstructed areas) and accuracy is within several meters hence unnacceptable for localizing human-scale mobile robots.
- Additionally, we might need to look beyond the robot's absolute position in Earth's reference frame (perhaps relative position with respect to some target).
- Sensors are the fundamental robot input for the process of perception, and therefore the degree to which sensors can discriminate the world state is critical. Sensor noise induces a limitation on the consistency of sensor readings in the same environmental state and, therefore, on the number of useful bits available from each sensor reading. Often, the source of sensor noise problems is that some environmental features are not captured by the robot’s representation and are thus overlooked.
  - For instance, consider the noise level (i.e., apparent random error) of ultrasonic range-measuring sensors (e.g., sonars). When a sonar transducer emits sound toward a           relatively smooth and angled surface, much of the signal will coherently reflect away, failing to generate a return echo.


**why must the robot determine its location?**
Localization may be mandatory in order to successfully navigate between waypoints and reach targets; the robot must have a way of detecting the goal location relative to itself.


**dealing with the challange of robot localization**
- behavior-based localization:
  - since sensors and effectors are noisy and information-limited, one should avoid creating a geometric map for localization.
  - instead we devise a set of behaviors that result in the desired robot motion without explicit reasoning about location/position and avoid path planning.
  - ie. following the left wall until we get to goal.
  - this is not scalable to larger environments, often heavily depends on specific robot hardware and environmental characteristics, there may also be multiple active behaviors    at a time which could backfire the performance optimization.
- explicit map-based localization:
  - attempts to map explicit location using sensor data, updating belief of location against map of the environment.
  - makes the system’s belief about position transparently available to the human operators.
  - The existence of the map itself represents a medium for communication between human and robot: the human can simply give the robot a new map if the robot goes to a new environment; hence scalable.
  - the internal map representation is constructed and trusted by the robot, if the model diverges from reality so will the behavior of the robot
   

The map-based approach will require more up-front development effort to create a navigating mobile robot. The hope is that the development effort results in an architecture that can successfully map and navigate a variety of environments, thereby amortizing the up-front design cost over time.

![Mapping techniques.](https://github.com/selin-k/selin-k.github.io/blob/main/PersonalNotes/media/localizationtech.png?raw=true)


**Successful localization systems and qualifying techniques**
...


### Cognition and Planning

Cognition generally represents the purposeful decision-making and execution that a system utilizes to achieve its highest-order goals.


**navigation competence**
Given partial knowledge about its environment and a goal position or series of positions, navigation encompasses the ability of the robot to act based on its knowledge and sensor values so as to reach its goal positions as efficiently and as reliably as possible.

**Aim: How can we utilize the tools discussed above to solve the problem of navigation and decision making?**

There are two notable competences required for mobile robot navigation:
1. Given a map and a goal waypoint, the robot must be capable of path planning such that a trajectory is identified which will reach the robot to the goal when executed. This could be the usage of a Global Positioning System (GPS) and some path finding algorithm.
2. Given real time sensor readings about the environment of the robot, as the robot executes the planned path to reach its goal is must be capable of improvising and modulating the trajectory to avoid collisions or harmful situations (to the mission) such as unexpected dead ends.


---

- note to self: disover answers to the localization questions above and see current industry solutions
- discover path planning and object avoidance solutions and some currently used solutions too.





