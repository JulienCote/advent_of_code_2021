/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as a list of longs
data:"J"$ "," vs raze read0 `:../data
/ TODO: implement this

first_star:{[data]
  :0
  }

/ TODO: implement this
second_star:{[data]
  :0
  }

-1 "First start value is: ", string first_star[data];
-1 "Second start value is: ", string second_star[data];

exit 0