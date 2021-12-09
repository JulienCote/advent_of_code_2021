/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as a list of longs
data:"J"$ "," vs raze read0 `:../data

first_star:{[data]
  final_position:`long$med data;
  :sum abs data - final_position
  }

second_star:{[data]
  min_data:sums each 1 + til each abs data - min data;
  max_data:sums each 1 + til each abs data - max data;
  :min sum reverse'[min_data],' 0,' max_data
  }

-1 "First start value is: ", string first_star[data];
-1 "Second start value is: ", string second_star[data]; / this is too big and won't complete ever, maybe

exit 0
