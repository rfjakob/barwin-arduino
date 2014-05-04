grep statis serial*|sed  's/^.*requested_amount: //'|sed 's/, measured_amount: /+/'|grep -o '[0-9+-]*'|bc
