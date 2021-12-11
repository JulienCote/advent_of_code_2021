/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as bool lists
data:read0 `:../data
points:2 cut "J"$"," vs' raze  ("->" vs) each data

get_sign:{(x>=0)-x<0};

point_map:{[points]
  length_components:(-) .'' flip each points; /for each point set, the x and y deltas
  signs:neg get_sign[length_components];

  // generates all intermediary components delts between src -> dst
  ranges:{
    x:x+1;
    $[x[0]=1; (x[1]#0; til x[1]);
      x[1]=1; (til x[0]; x[0]#0);
      (til x[0];til x[1])]
    } each abs length_components;

  all_points:raze flip each (first each points) + signs * ranges;
  :count each group all_points
  }

first_star:{[points]
  straight_lines:points where any each (=).'' flip each points;
  point_count:point_map[straight_lines];
  :count point_count where point_count >= 2
  }

second_star:{[points]
  point_count:point_map[points];
  :count point_count where point_count >= 2
  }

-1 "First start value is: ", string first_star[points];
-1 "Second start value is: ", string second_star[points];

exit 0