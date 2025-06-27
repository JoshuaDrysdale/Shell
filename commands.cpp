#include "commands.hpp"
#include <filesystem>

using namespace std;

/**
 * @brief Constructor for the Commands class.
 * 
 * This constructor initializes an instance of the Commands class.
 */
Commands::Commands(){}

/**
 * @brief Manages the system's execution path.
 * 
 * If arguments are provided, add them to the list of executable paths.
 * If no arguments are provided, print the current list of paths.
 * 
 * @param userArguments Vector containing the paths to add or an empty vector to print current paths.
 */
void Commands::system_path(vector<string> userArguments){ 

    if (userArguments.size()==0){
        //std::cout<<"no args"<<std::endl;
        for (size_t i=0; i<filePaths.size(); i++){
        std::cout<<i<<". "<<filePaths[i]<<std::endl;
        }
    }else{
        for(size_t i=0; i<userArguments.size(); i++){
            filePaths.push_back(userArguments[i]);
            std::cout<<userArguments[i]<<" added to file path"<<std::endl;
        }
    } 
}
/**
 * @brief Executes a user command if present in the list of paths we know.
 * 
 * Search for the executable file in the stored paths. If found,forks a child process to execute the command.
 * 
 * @param userCommand The command to execute.
 * @param userArguments Arguments for the command.
 */
void Commands::command_execute(string userCommand,vector<string> userArguments){

    if(filePaths.size()>0){
        bool found =false;

        for(size_t i=0; i<filePaths.size(); i++){
            
            if (std::filesystem::exists(filePaths[i]+"/"+userCommand))
            {
               found = true;
                //cout << "File exists" << endl;

                int process = fork();

                if(process==0){
                    
                    std::vector<char*> cArgs;
                    cArgs.push_back(const_cast<char*>(userCommand.c_str())); 
                        for (const auto& arg : userArguments) {
                            cArgs.push_back(const_cast<char*>(arg.c_str()));
                        }
                    cArgs.push_back(nullptr); 
                    
                    execvp(cArgs[0], cArgs.data());
                    cout<<"child exec failed"<<endl;

                }else if(process <0){
                    cout<<"fork failed"<<endl;
                }else{
                    wait(NULL);
                    
                }

            }

        }
        if(found==false)
        {
          
            cout << "command "<<userCommand<<" not found" << endl;
        }

    }else{
        std::cout<<"no paths created"<<std::endl;
    }

    
}
/**
 * @brief Changes the current working directory.
 * 
 * This function takes a single directory as an argument and changes the working directory to it.
 * 
 * @param userArguments Vector containing a single directory name.
 */
void Commands::system_cd(std::vector <std::string> userArguments){

        if(userArguments.size()>0){

            //for(size_t i=0; i<userArguments.size(); i++){
                const char* cstring = userArguments[0].c_str();

                int value = chdir(cstring);

                if(value==0){
                std::cout<<value<<" success"<<std::endl;
                }else if (value ==1){
                    cout<<value<<" failure"<<endl;
                }
            //}

        }else{
            std::cout<<"no directory given"<<endl;
        }

}

/**
 * @brief Exits the shell.
 * 
 * This function terminates the program.
 */
void Commands::system_exit(){

    exit(0);
}

/**
 * @brief Displays help information.
 * 
 * This function should provide details about the available commands.
 */

void Commands::system_help(){
       
	std::cout<<"Available commands: "<<endl;
	std::cout<<"exit               - exits the shell \nhelp               - displays a list of the available commands \npath               - provides a list of the execuatable paths \npath <directories> - adds to the list of executable paths\ncd <directory>     - change working directory \n<command> [args]   - executes a command if found in the path"<<endl;
}
