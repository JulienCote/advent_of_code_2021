/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as a list of longs
data:"J"$ "," vs raze read0 `:../data

first_star:{[fishes; n_days]
  while[0 <= n_days-:1;
    fishes:fishes - 1;
    births:where fishes = -1;
    fishes:@[fishes; births; :; 6];
    fishes:fishes, count[births]#8;
  ];
  : count fishes
  }

/ TODO: implement this
second_star:{[points]

  }

-1 "First start value is: ", string first_star[data; 80];
/-1 "Second start value is: ", string first_star[data; 256]; / this is too big and won't complete ever, maybe

/exit 0