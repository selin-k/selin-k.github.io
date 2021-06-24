---
layout: default
title: ROS basics
---

## What
ROS is an open-source, meta-operating system for your robot. It provides the services you would expect from an operating system, including hardware abstraction, low-level device control, implementation of commonly-used functionality, message-passing between processes, and package management. It also provides tools and libraries for obtaining, building, writing, and running code across multiple computers. ROS is similar in some respects to 'robot frameworks,' such as Player, YARP, Orocos, CARMEN, Orca, MOOS, and Microsoft Robotics Studio.

## How
The ROS runtime "graph" is a peer-to-peer network of processes (potentially distributed across machines) that are loosely coupled using the ROS communication infrastructure. ROS implements several different styles of communication, including synchronous RPC-style communication over services, asynchronous streaming of data over topics, and storage of data on a Parameter Server.

The primary goal of ROS is to support code reuse in robotics research and development.

ROS is a distributed framework of processes (aka Nodes) that enables executables to be individually designed and loosely coupled at runtime. These processes can be grouped into Packages and Stacks, which can be easily shared and distributed. ROS also supports a federated system of code Repositories that enable collaboration to be distributed as well. This design, from the filesystem level to the community level, enables independent decisions about development and implementation, but all can be brought together with ROS infrastructure tools.

## Where
ROS currently only runs on Unix-based platforms. Software for ROS is primarily tested on Ubuntu and Mac OS X systems, though the ROS community has been contributing support for Fedora, Gentoo, Arch Linux and other Linux platforms.


## Brief and short clarifications

### What is a meta OS?
ROS is not an independent OS, it requires the presence of UNIX-like OS to work. However, it provides OS-like functions such as: interprocesses message passing, hardware abstraction, package management and other features.


### What exactly are nodes in this context?
Loosely coupled executables of various functionalities that could be added to a robot running a ROS instance.

### How are messages passed between nodes?
Publisher nodes stream messages along topic lines. Nodes that aim to recieve/communicate with publishing nodes can subscribe to publisher nodes to recieve the message data.


## Concepts in more detail

ROS has three levels of concepts: the Filesystem level, the Computation Graph level, and the Community level. 

1. ROS Filesystem Level
The filesystem level concepts mainly cover ROS resources that you encounter on disk, such as packages, metapackages, repositories, messages, service types.
- messages - Message descriptions, stored in `my_package/msg/MyMessageType.msg`, define the data structures for messages sent in ROS.
- service types - Service descriptions, stored in `my_package/srv/MyServiceType.srv`, define the request and response data structures for services in ROS.

2. ROS Computation Graph Level
The Computation Graph is the peer-to-peer network of ROS processes that are processing data together. The basic Computation Graph concepts of ROS are nodes, Master, Parameter Server, messages, services, topics, and bags, all of which provide data to the Graph in different ways.

### Nodes, Topics, and Messages
- Nodes: Nodes are processes that perform computation. ROS is designed to be modular at a fine-grained scale; a robot control system usually comprises many nodes. For example, one node controls a laser range-finder, one node controls the wheel motors, one node performs localization, one node performs path planning, one Node provides a graphical view of the system, and so on.
- Master: The ROS Master provides name registration and lookup to the rest of the Computation Graph (actions similar to DNS). Without the Master, nodes would not be able to find each other, exchange messages, or invoke services. The ROS master hosts the parameter server.
- Parameter Server: The Parameter Server allows data to be stored by key in a central location. It is currently part of the Master.
- Messages: Nodes communicate with each other by passing messages. A message is simply a data structure, comprising typed fields. Standard primitive types (integer, floating point, boolean, etc.) are supported, as are arrays of primitive types. Messages can include arbitrarily nested structures and arrays (much like C structs).

A better explanation and an example:

The computation graph is essentially a digraph where the nodes are the loosely coupled processes a robot executes and each one usually focuses on a single task. The edges are the topics that each node is capable of publishing and subscribing to. A node may publish AND subscribe to more than one topic. Publishing a topic means that the node can stream data along that topic and nodes that are subscribed to that topic will be able to recieve/listen to the data published. The data that streams along topics are called messages which are essentially data structures.

The ROS Master acts as a nameservice in the ROS Computation Graph. It stores topics and services registration information for ROS nodes. Nodes communicate with the Master to report their registration information. As these nodes communicate with the Master, they can receive information about other registered nodes and make connections as appropriate. The Master will also make callbacks to these nodes when this registration information changes, which allows nodes to dynamically create connections as new nodes are run.

[Here is some more information on slides](http://www.alaakhamis.org/teaching/SPC418/tutorials/Tutorial-2.pdf)

![Example of hor ROS nodes communicate](https://github.com/selin-k/selin-k.github.io/blob/main/PersonalNotes/media/rosnodes.JPG?raw=true)

Example above in python:

```py
#!/usr/bin//env_python
import rospy
from std_msgs.msg import String

def faxer():
    rospy.init('faxer', anonymous=True) # initalized ROS node, sets params
    pub = rospy.Publisher('fax_line', String, queue_size=10) # estbalishes as publisher node, streams String data.
    rate = rospy.Rate(10) # publishes at rate 10Hz

    # loops in an infinite loop until ros is told to shutdown.
    while not rospy.is_shutdown():
        message = raw_input('Enter message to fax:')
        pub.publish(message)
        rate.sleep() # sleep to publish at given rate and not consume CPU power.

if __name__ == "__main__":
    try:
        faxer()
    except rospy.ROSInterruptException:
        pass
```

```py
#!/usr/bin//env_python
import rospy
from std_msgs.msg import String

# handles given messages from topic line
def fax_handler(data):
    rospy.loginfo(data.data) 


def printer():
    rospy.init('printer', anonymous=True)
    rospy.Subscriber('fax_line', String, fax_handler) # subscribes to fax_line to recieve String data and passes it to fax_handler.
    rospy.spin() # makes sure the printer() doesn't die and continues to listen

if __name__ == "__main__":
    printer()
```

[Source](https://www.youtube.com/watch?v=izfXL3rIt10&list=PLChxZ9TvsWJ8iM86m000R__20T_Ym9HAE&index=1)

---

- [More detail on computation and some command info](https://subscription.packtpub.com/book/hardware_and_creative/9781783551798/1/ch01lvl1sec12/understanding-the-ros-computation-graph-level)
- [Interesting paper: message encrpytion in node communication](https://www.researchgate.net/figure/Establishing-connections-through-the-ROS-Topic-paradigm-Derivative-of-Establishing-a_fig1_323520311)

---

### Services and actions
...

3. ROS Community Level
The ROS Community Level concepts are ROS resources that enable separate communities to exchange software and knowledge. These resources include distributions, repos, ROS wiki and discussion boards etc.

## Graph Resource Names

[clicc here](http://wiki.ros.org/ROS/Concepts#:~:text=Names-,Graph%20Resource%20Names,-Graph%20Resource%20Names)


---

## Quick info on distributions

A ROS distribution is a versioned set of ROS packages. These are akin to Linux distributions (e.g. Ubuntu). The purpose of the ROS distributions is to let developers work against a relatively stable codebase until they are ready to roll everything forward. Therefore once a distribution is released, we try to limit changes to bug fixes and non-breaking improvements for the core packages (every thing under ros-desktop-full). And generally that applies to the whole community, but for "higher" level packages, the rules are less strict, and so it falls to the maintainers of a given package to avoid breaking changes.

- We're using melodic?? why not noetic?

[Distributions page](http://wiki.ros.org/Distributions)


