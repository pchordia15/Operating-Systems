CSCI340 Virtual Paging Assignment
==================================

CSCI340 (Operating Systems)  
California State University, Chico  
Public Code

Slight additions   
By Bryan Dixon - 2014

Adopted from assignment  
By Andy Sayler - 2012  
http://www.andysayler.com

That was adopted from  
assignment by Dr. Alva Couch  
http://www.cs.tufts.edu/~couch/

With help from:  
Junho Ahn - 2012

##Assignment Introduction

All modern operating systems use virtual memory and paging in order to effectively utilize the computer’s memory hierarchy. Paging is an effective means of providing memory space protection to processes, of enabling the system to utilize secondary storage for additional memory space, and of avoiding the need to allocate memory sequentially for each process.

We have studied how virtual memory systems are structured and how the MMU converts virtual memory addresses to physical memory addresses by means of a page table and a translation lookaside buffer (TLB). When a page has a valid mapping from VM address to physical address, we say the page is “swapped in”. When no valid mapping is available, the page is either invalid (a segmentation fault), or more likely, “swapped out”. When the MMU determines that a memory request requires access to a page that is currently swapped out, it calls the operating system’s page-fault handler. This handler must swap-in the necessary page, possibly evicting another page to secondary memory in the process. It then retries the offending memory access and hands control back to the MMU.

As you might imagine, how the OS chooses which page to evict when it has reached the limit of available physical pages (sometime called frames) can have a major effect on the performance of the memory access on a given system. In this assignment, we will look at various strategies for managing the system page table and controlling when pages are paged in and when they are paged out.

##Task

The goal of this assignment is to implement a paging strategy that maximizes the performance of the memory access in a set of predefined programs. You will accomplish this task by using a paging simulator that has already been created for you. Your job is to write the paging strategy that the simulator utilizes (roughly equivalent to the role the page fault handler plays in a real OS). Your initial goal will be to create a Least Recently Used paging implementation. You will then need to implement some form of predictive page algorithm to increase the performance of your solution. You will be graded on the throughput of your solution (the ratio of time spent doing useful work vs time spent waiting on the necessary paging to occur).

