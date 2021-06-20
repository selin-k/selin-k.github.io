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

> Since a mobile robot is a self-contsined automaton that can move with respect to its environment, unlike manipulators such as fixed arms, position estimation is difficult.

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


