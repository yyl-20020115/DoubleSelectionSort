Double Selection Sort Algorithm


  Within single iteration, we get both max and min from the 

  searching range, and then swap the min with the first element

  of the range, and the max with the last element of 

  the range. Therefore we can sort from both direction,

  and get almost double speed.

  There are new algorithms utilizing AVX instructions to make
  
  the selections 5 times faster.

NEW:
Almost all kinds of sorting algorithms included.
Mostly in C#. 
Timing is used to compare the speed efficiency.

NEW:
AVX2 verson of Odd Even Sort in C#, and
AVX2 and AVX512 versions of Odd Even Sort algorithm added in C++ project.

DATA:
original data(count = 4096):


for system quick sort:
time:0.140100(ms)


for quick sort:
time:0.132600(ms)
correct:true


for fast quick sort:
time:0.121700(ms)
correct:true


for odd even sort:
time:4.588300(ms)
correct:true


for fast odd even sort 256:
time:2.145400(ms)
correct:true


for fast odd even sort 512:
time:2.230200(ms)
correct:true


Please check the source code.

Best Regards,

Yilin
