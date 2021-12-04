/this line sets the pwd in the directory of the this file
system "cd ", first[system "pwd"],"/", 1_string first ` vs  hsym .z.f

/read data as bool lists
data:"B"$'' flip read0 `:../data

get_majority:{ count[x] % 2 }

bits_to_short:{0b sv reverse reverse[x] @ til 16}

keep_where_bit_at_eq:{[data; pos; val]
  :data where val = @'[data;pos]
  }

first_star:{[data]
  majority_at:get_majority[first data];
  true_count:sum each data;

  gamma_reading:true_count > majority_at;
  gamma:bits_to_short[gamma_reading];
  epsilon:bits_to_short[not gamma_reading];
  : epsilon * gamma
  }

second_star:{[data]
  values:{[data; op]
          reduce:{[data; idx; op]
            majority_at:get_majority[first data];
            true_count:sum each data;
            :flip keep_where_bit_at_eq[flip data; idx; op[true_count[idx];majority_at]];
            };
          i:0;
          d:data;
          while[1<count first d; d:reduce[d; i; op]; i+:1];
          :d
          }[data;] each (>=;<);
  : (*) . bits_to_short each raze each values
  }

-1 "First start value is: ", string first_star[data];
-1 "Second start value is: ", string second_star[data];

exit 0