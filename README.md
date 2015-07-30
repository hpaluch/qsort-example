Quick Sort /etc/passwd fullnames in C
=====================================

Example how to use quick sort to sort /etc/passwd entries by fullname
(expected format of fullname is "LastName FirstName").

Setup
-----

On CentOS 5.x,6.x,7.x:

	yum install gcc binutils make glibc-devel valgrind

Run
---

	make run

Run with valgrind memory leaks checker
-------------------------------------

	make valgrind


