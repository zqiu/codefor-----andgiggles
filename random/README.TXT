Included is the impelementation of the server class

it can do all of the following commands

Add <filename> <bytes>\n<filecontents>		with both binary and non binary files
Append <filename> <bytes>\n<filecontents> 	with both binary and non binary files
READ <filename>\n 			  	with both binary and non binary files
LIST \n
DELETE <filename>\n

if the command is not specified, our server will return invalid command
Important:make sure your commands are terminated with the character \ and then n not the newline character

All files are stored in .storage/

threading is done with a single mutex to represent when a file is being modified. I know this is really inefficient but I am choosing this because this is the simplest to implement. It garuantees synchorization. The only problem is that there can be starvation when lots of threads are accesed. 

If I had more time I would look into incorporateing a counting semaphore into the program. How it would work then is that add/append would still lock the file and release the mutex when it is done writing. Reading would lock and then unlock the file so that it waits for add/appends to finish and allows for other reads to access the file. Once it releases the lock, it the increments the counting semaphore reads the file and then decrements the counting semaphore. Write first locks the file. This ensures that it waits for writes to finish and that no more writes can be done. Then it waits for the counting semaphore to reach 0 so that no more reads are on the file. When that is done, it deletes the file. 
