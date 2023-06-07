#include "shell.hh"

int main()
{ std::string input;

  // ToDo: Vervang onderstaande regel: Laad prompt uit bestand
  std::string prompt = "Laad prompt uit bestand: ";

  while(true)
  { std::cout << prompt;                   // Print het prompt
    std::getline(std::cin, input);         // Lees een regel
    if (input == "new_file") new_file();   // Kies de functie
    else if (input == "ls") list();        //   op basis van
    else if (input == "src") src();        //   de invoer
    else if (input == "find") find();
    else if (input == "seek") seek();
    else if (input == "exit") return 0;
    else if (input == "quit") return 0;
    else if (input == "error") return 1;

    if (std::cin.eof()) return 0; } }      // EOF is een exit

void new_file(){
    std::string naam;
    std::string bestand;
    
    std::cout << "Naam bestand: ";
    std::cin >> naam;
    
    std::cout << "Geef inhoud(<EOF> om af te sluiten): ";
    std::string text;
    while(std::getline(std::cin, text)){
        if(text == "<EOF>"){
            break;
        }
        bestand += text + '\n';
    }
    
    int bestand_maak = syscall(SYS_open, naam.c_str(), O_CREAT|O_TRUNC|O_WRONLY, 0644);
    syscall(SYS_write, bestand_maak, bestand.c_str(), bestand.size());
    syscall(SYS_close, bestand_maak);
}

void list(){
	std::cout << " LIST " << std::endl;
    pid_t pid = syscall(SYS_fork);

    if(pid == 0){
    std::cout << " HELLO I AM A CHILD, DO THE THING " << std::endl;
    	char const *arguments[] = {"/usr/bin/ls" , "-la", NULL};
        syscall(SYS_execve, arguments[0], arguments, NULL);
        syscall(SYS_exit, EXIT_SUCCESS);
    }else{
        syscall(SYS_wait4, 0, NULL, 0, 0);
    }
    return;
}

void find(){
    //cin
    std::string text;
    std::cout << "u zoekt: ";
    std::getline(std::cin, text);
    
    int pipefd[2];
    syscall(SYS_pipe, pipefd);
    
    pid_t child1;
    pid_t pid2;
    
    //pid1
    child1 = syscall(SYS_fork);
    if(child1 == 0){
        char const *arguments[] = {"/usr/bin/find", ".", NULL};
        syscall(SYS_dup2, pipefd[1], STDOUT_FILENO); 
        syscall(SYS_close, pipefd[0]);
        syscall(SYS_execve, arguments[0], arguments, NULL);
        syscall(SYS_exit, EXIT_SUCCESS);
        
    }
    
    //pid2
    pid2 = syscall(SYS_fork);
    if(pid2 == 0){
        char const *arguments[] = {"/usr/bin/grep", text.c_str(), NULL};
        syscall(SYS_dup2, pipefd[0], STDIN_FILENO); 
        syscall(SYS_close, pipefd[1]);
        syscall(SYS_execve, arguments[0], arguments, NULL);
        syscall(SYS_exit, EXIT_SUCCESS);
        
    }
    
    syscall(SYS_close, pipefd[0]);
    syscall(SYS_close, pipefd[1]);
    
    syscall(SYS_wait4, child1, NULL, 0, 0);
    syscall(SYS_wait4, pid2, NULL, 0, 0);
}

void seek(){  
    //maak seek en schrijf
    int bestand_maak_seek = syscall(SYS_open, "seek", O_CREAT|O_TRUNC|O_WRONLY, 0644);
    
    syscall(SYS_write, bestand_maak_seek , "x", 1);
    syscall(SYS_lseek, bestand_maak_seek, 5 * 1024 * 1024, SEEK_END); 
    syscall(SYS_write, bestand_maak_seek , "x", 1);
    syscall(SYS_close, bestand_maak_seek);
    
    //maak loop en schrijf
    int bestand_maak_loop = syscall(SYS_open, "loop", O_CREAT|O_TRUNC|O_WRONLY, 0644);
    syscall(SYS_write, bestand_maak_loop , "x", 1);
    for(int i = 0; i < 5 * 1024 * 1024; ++i){
        syscall(SYS_write, bestand_maak_loop, "\0", 1);
    }
    syscall(SYS_write, bestand_maak_loop , "x", 1);
    
    syscall(SYS_close, bestand_maak_loop);

  
}

void src() // Voorbeeld: Gebruikt SYS_open en SYS_read om de source van de shell (shell.cc) te printen.
{ int fd = syscall(SYS_open, "shell.cc", O_RDONLY, 0755); // Gebruik de SYS_open call om een bestand te openen.
  char byte[1];                                           // 0755 zorgt dat het bestand de juiste rechten krijgt (leesbaar is).
  while(syscall(SYS_read, fd, byte, 1))                   // Blijf SYS_read herhalen tot het bestand geheel gelezen is,
    std::cout << byte; }                                  //   zet de gelezen byte in "byte" zodat deze geschreven kan worden.
