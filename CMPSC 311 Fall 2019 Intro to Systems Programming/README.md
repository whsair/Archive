# CMPSC 311 – Intro to Systems Programming - Fall 2019

## Course Objectives:
Systems programming concerns the development of software components and methods for their combination, independent of any particular application. This course will provide information and experience required to:
- Design and implement components of large software systems.
- Evaluate design alternatives according to standard practice, specifications, performance analysis, robustness, etc.
- Understand operating system services available to application programmers.
To concentrate attention, we investigate one system and one programming language in detail, through demonstration programs, short- and long-term programming assignments. The specific system is Unix, a family of operating systems forming a complete standardized programming environment based on the idea of software tools. The specific language is C, which is widely used for operating system implementations, and which forms the basis for the C++ and Java languages studied in the prerequisite courses.

At the conclusion of this course, students should:
- Understand computer systems, especially low-level influence on high-level goals. This includes the machine level representation of programs and data structures; the memory
hierarchy and its impact on performance; access information via files systems, and access to other computer systems via networks.
- Understand existing system software and software standards, especially the UNIX toolset. This includes preparing a program (editors, static analysis, development
environments); running a program (compilers and interpreters, assembler, linker, loader, debugger, profiler, tracer); controlling parts of a program (memory management,
threads); communication between programs (within one system using signals, between systems using sockets and communication protocols); and combinations of software tools
with scripting languages.
- Understand real world code, such as selections from the Linux operating system kernel and GNU utilities and libraries.
- Understand system performance, including experiments on program performance and optimization techniques.


## [Datalab](Project1%20Datalab)
The purpose of this assignment is to become more familiar with bit-level representations of integers andfloating point numbers. You’ll do this by solving a series of programming “puzzles.” Many of these puzzlesare quite artificial, but you’ll find yourself thinking much more about bits in working your way throughthem.

## [Shell Lab](Project3%20Shell%20Lab)
The purpose of this assignment is to become more familiar with the concepts of process control and sig-nalling. You’ll do this by writing a simple Unix shell program that supports job control.

## [Cartlab](Project4%20Cartlab)
The purpose of this lab is to create a user-space device driver for a in memory filesystem that is built on topof a hierarchical random access memory system (HRAM). At the highest level, you will translate file systemcommands into memory frame level memory operations (see memory interface specification below).  Thefile system commands include open, close, read, write and seek for files that are written to your file systemdriver. These operations perform the same as the normal unix I/O operations, with the caveat that they directfile contents to the HRAM storage device instead of the host filesystem.  The arrangement of software isdescribed below.

## [Proxylab](Project5%20Proxylab)
A Web proxy is a program that acts as a middleman between a Web browser and anend server.  Instead ofcontacting the end server directly to get a Web page, the browser contacts the proxy, which forwards therequest on to the end server.  When the end server replies to the proxy, the proxy sends the reply on to thebrowser.Proxies are useful for many purposes.  Sometimes proxies are used in firewalls, so that browsers behind afirewall can only contact a server beyond the firewall via the proxy.  Proxies can also act as anonymizers:by  stripping  requests  of  all  identifying  information,  a  proxy  can  make  the  browser  anonymous  to  Webservers. Proxies can even be used to cache web objects by storing local copies of objects from servers thenresponding to future requests by reading them out of its cache rather than by communicating again withremote servers.In this lab, you will write a simple HTTP proxy that caches web objects. For the first part of the lab, you willset up the proxy to accept incoming connections, read and parse requests, forward requests to web servers,read the servers’ responses, and forward those responses to the corresponding clients.  This first part willinvolve learning about basic HTTP operation and how to use sockets to write programs that communicateover network connections.  In the second part,  you will add caching to your proxy using a simple mainmemory cache of recently accessed web content.


