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
original data(count = 65536):

for system quick sort:
time:3.126000(ms)

for quick sort:
time:2.794400(ms)
correct:true

for fast quick sort:
time:2.091200(ms)
correct:true

DATA:
original data(count = 1048576):


for system quick sort:
time:47.855000(ms)

for quick sort:
time:44.583900(ms)
correct:true

for fast quick sort:
time:35.247400(ms)
correct:true

DATA:
original data(count = 16777216):

for system quick sort:
time:690.385200(ms)

for quick sort:
time:1948.574800(ms)
correct:true

for fast quick sort:
time:900.292700(ms)
correct:true

DATA:
original data(count = 65536):

for system quick sort:
time:3.059600(ms)

for quick sort:
time:2.822800(ms)
correct:true

for fast quick sort 256:
time:2.095600(ms)
correct:true

for fast quick sort 512:
time:2.070200(ms)
correct:true

Please check the source code.

Best Regards,

Yilin
