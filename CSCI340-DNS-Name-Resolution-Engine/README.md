CSCI340-DNS-Name-Resolution-Engine
==================================
##CSCI340 - Operating Systems
Adapted from University of Colorado at Boulder CSCI3753 Assignment

##Introduction
In this assignment you will develop a multi-threaded application that resolves domain names to IP addresses, similar to the operation preformed each time you access a new website in your web browser. The application is composed of two sub-systems, each with one thread pool: requesters and resolvers. The sub-systems communicate with each other using a bounded
queue.
