Double Selection Sort Algorithm


  Within single iteration, we get both max and min from the 

  searching range, and then swap the min with the first element

  of the range, and the max with the last element of 

  the range. Therefore we can sort from both direction,

  and get almost double speed.

  There are new algorithms utilizing AVX instructions to make
  
  the selections 5 times faster.


Please check the source code.

Best Regards,

Yilin