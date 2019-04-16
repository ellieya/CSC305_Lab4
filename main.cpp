//NOTE: ONLY WORKS ON LINUX AND LINUX-BASED SYSTEMS

#include <unistd.h>
#include <string.h>
#include <sys/wait.h> 
#include <iostream>

#define READ 0
#define WRITE 1
#define MAXSIZE 50

using namespace std;

char* phrase1 = "This is phrase 1!";
char* phrase2 = "This is phrase 2!";
char* phrase3 = "This is phrase 3!";


int main() {

	int pid;
	char buffer[MAXSIZE];

	//Create pipe
	int fd[2], byteValue;
	pipe(fd);

	//Fork
	pid = fork();

	//Different execution based on whether child or parent process
	//CHILD (Writer)
	if (pid == 0) {
		//#1: Close pipe end that will not be used by this process.
		cout << "Child: I have closed my READ end!" << endl;
		close(fd[READ]);

		//#2: Write to pipe
		write(fd[WRITE], phrase1, MAXSIZE);
		write(fd[WRITE], phrase2, MAXSIZE);
		write(fd[WRITE], phrase3, MAXSIZE);

		//#3: Send dying message, close pipe, and terminate.
		cout << "Child: I have finished WRITING! Terminating..." << endl;
		close(fd[WRITE]);
	}

	//PARENT (Reader)
	else {
		//#1: Close pipe end that will not be used by this process.
		cout << "Parent: I have closed my WRITE end!" << endl;
		close(fd[WRITE]);

		//#2: Wait until child is done writing
		cout << "Parent: I am going to wait for CHILD to WRITE!" << endl;
		wait(NULL);

		//#3: Read child data
		cout << "Parent: Ok! It's my turn. I am going to READ from PIPE and print data written by child process!" << endl;
		for (int i = 0; i < 3; i++) {
			read(fd[READ], buffer, MAXSIZE);
			cout << buffer << endl;
		}

		//#4: Send dying message, close pipe, and terminate.
		cout << "Parent: I have finished READING! Terminating..." << endl;
		close(fd[READ]);
	}

	return 0;
}