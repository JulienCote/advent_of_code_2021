/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as a list of pairs (input list;output list)
data:" " vs'' (" | " vs) each read0 `:../data

first_star:{[data]
  /count how many output strings contain 2, 3, 4 or 7 characters
  :sum over {x in 2 4 3 7} each {count each x} each last each data
  }

/ TODO: implement this
second_star:{[data]
  segment_per_number:0 1 2 3 4 5 6 7 8 9!6 2 5 5 4 5 6 3 7 6;
  }

-1 "First start value is: ", string first_star[data];
-1 "Second start value is: ", string second_star[data]; / this is too big and won't complete ever, maybe

exit 0