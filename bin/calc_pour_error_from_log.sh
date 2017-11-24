grep statis serial*|sed  's/^.*requested_amount: //'|sed 's/, measured_amount: /+/'|grep -o '[0-9+-]*'|bc


grep Stats barwin-arduino/log/serialRMI_2014-05-10_MQ.log|sed  's/^.*requested_amount: //'|sed 's/, measured_amount: /-/'|cut -d',' -f1|grep -P '^[0-9]+-[0-9]+'|bc



