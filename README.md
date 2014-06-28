SignalLibrary
=============

##Index

 1. Summary:
     * Resume the content of this Library and his purpose.

 2. Priorities:
     * List of priorities that will be followed in the development of this
     * library. We hope this will give users early knowledge of when they will
     * have access to some key features they might be interested in.

 3. Acknowledgments:
     * Thanks to severals communities.

 4. Special Thanks:

##Summary

A C++11 based signal/slot library. First part of a future Functional/Reactive
C++ Framework. The main goal is to offer a pure Standard based signal/slot library,
that cover all the fundamental necessities of this concept.

Also we want to offer some key features that are not so easy to found implemented
and can give a huge flexibility to the concept, some of them are:

   - Possibility to pass different parameters to slots at the moment the signal
     is emitted.
   - Multiple slots for one signal.
   - Threaded slots executions, or just slot execution in a parallel task.
   - Threaded multiple slots.
   - And some other magical tricks.

This document will be updated with the development of the project.

##Priorities

 1. The first goal is to make a strong design for the library, so we can ensure
    it's efficient, easy to use, and that it could easily handle typical problems
    like reference lost. Also keeping in mind that in a near future will be
    thread-safe.

 2. Ensure that the library is thread safe.

 3. Implement an efficient thread pool, so we can make use of task to parallel
    slots executions, or real threaded slot execution.

##Acknowledgments

Thanks to all the Open Source/Free Software communities, for they effort and
constance. Thanks to them it's possible to have access to the huge amount of
information that this project requires. Also thanks to the C++ community,
for their desire to share their knowledge.

With the development of the project the special thanks section will be updated.

##Special Thanks
