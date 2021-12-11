/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as longs
data:"J"$ read0 `$"../data"

/compute number of times consecutive values increase
count_increases:{[data]
  :sum 0<deltas[first data; data]
  }

-1"Part 1: Number of time the measurement increases";
show count_increases[data];

-1"";

-1"Part 2: Number of time the measurement increases while considering a sliding windows of 3 measurements";
sliding_window_size:3
indexes_per_window:til[count data] +\: til[sliding_window_size] / generate all windows index groups (0,1,2),(1,2,3),etc.
indexes_per_window:neg[sliding_window_size - 1] _ indexes_per_window / last window_size-1 entries are out-of-bound, remove them
windows:sum each data @ indexes_per_window
show count_increases[windows]

exit 0