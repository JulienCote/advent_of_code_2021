/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as bool lists
data:read0 `:../data
moves:"J"$"," vs first data

data:1_data
boards:5 cut' 25 cut except[;0N] "J"$ " " vs " " sv data

seq:{{x ? y}[moves;] each y}[moves;] each boards // the boards where instead of values you have the sequence in which it was pulled
win_at_move:{(max each x;max each flip x)} each seq // the move number at which each row/line of each board wins
win_at_move:min each raze each win_at_move // the move number at which each board wins

numbers_played:seq <= win_at_move // truth tables of which moves were played at the time of winning, per board
scores:(moves @ win_at_move) * {sum x where not y}'[raze each boards; raze each numbers_played]

first_star:{
  :scores @ win_at_move ? min[win_at_move]
  }

second_star:{[data]
  :scores @ win_at_move ? max[win_at_move]
  }

-1 "First start value is: ", string first_star[data];
-1 "Second start value is: ", string second_star[data];

/exit 0