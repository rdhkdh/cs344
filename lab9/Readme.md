# OS Lab 9, Q1, 210101088, Ridhiman Kaur Dhindsa

## Executing the file:
Run initially q1_init.c to create and initialize semaphore:  
`gcc q1_init.c -o q1_init`    
`./q1_init`   
Then run final file:  
`gcc q1_final.c -o q1_final`  
`./q1_final`  
If the code initially gets stuck at 'Trying to lock...', it has gone into deadlock.
Exit the present execution and run the code again.  

The initial database tables are set to -1.
The philosophers (represented by child pids) use semaphores and access 
the databases as printed in the console, then rotate in rounds. The last 
5 entries in console display the updated database tables.

> Note: The 'q1.c' file is used for token generation, and should not be deleted.  
