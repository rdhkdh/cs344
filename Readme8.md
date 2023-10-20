# OS lab 8, 210101088, Ridhiman Kaur Dhindsa  

## Part f: multiple writers
Compile and run:  
gcc repository.c -o repository  
./repository 

First compile, then Run these 2 simultaneously:    
gcc q1_writer.c -o q1_writer  
./q1_writer  

gcc q1_writer2.c -o q1_writer2  
./q1_writer2  

It can be observed that writer 2 is unable to write as long as writer 1 is writing.  

## Part g: writer + reader
Compile and run:  
gcc repository.c -o repository  
./repository 

First compile, then Run these 2 simultaneously:  
gcc q1_writer.c -o q1_writer  
./q1_writer  

gcc q1_reader.c -o q1_reader  
./q1_reader 

It can be observed that reader is unable to read as long as writer 1 is writing.    

## Part h: multiple readers
Compile and run:  
gcc repository.c -o repository  
./repository 

gcc q1_writer.c -o q1_writer  
./q1_writer

First compile, then Run these 2 simultaneously:  
gcc q1_reader.c -o q1_reader  
./q1_reader  

gcc q1_reader2.c -o q1_reader2  
./q1_reader2

It can be observed that reader 2 is able to read simultaneously along with reader 1.      