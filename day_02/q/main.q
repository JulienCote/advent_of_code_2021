/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as longs
data:("SJ";" ")0: `$"../data"

first_star:{[data]
  actions:(`forward`down`up)!({(0;x)}; {(x;0)}; {(neg[x];0)});
  :(*) . sum each flip actions[first data] @' last data
  }

second_star:{[data]
  actions:(`forward`down`up)!({0}; {x}; {neg[x]});

  forward_values:@[last data;where `forward <> first data; :; 0];
  aim_values:sums actions[first data] @' last data;

  horizontal:sum forward_values;
  depth:sum forward_values *' aim_values;

  :horizontal * depth
  }

-1 "First start value is: ", string first_star[data];
-1 "Second start value is: ", string second_star[data];

exit 0