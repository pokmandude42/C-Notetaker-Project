#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
using namespace std;

class user{
private:
    string password;
    string username;
public:
    char user_directory[65] = "C:\\NoteTaker\\Users\\";
    char filelist_directory[65];
    char current_filepath[65];
    ofstream w_file;
    ifstream r_file;

    void display_users(){
        cout<<"Available users: "<<endl;
        ifstream list_file;
        list_file.open("C:\\NoteTaker\\Users\\list.txt");
        string users;
        while(getline(list_file,users)){//This is inspired by the File handling-MO.pptx in canvas. Credit goes to Michael Osheroff
            cout<<users<<endl;
        }
        list_file.close();
    }

    void log_in() {
        display_users();//displays the available users.
        char users[50], directory[65] = "C:\\NoteTaker\\Users\\";
        //Base directory
        strcpy(user_directory,directory);
        bool worked=false, loop=true;
        while(loop) {//This loop allows the program to make sure you selected a proper input.
            ifstream list_file;
            string user;
            list_file.open("C:\\NoteTaker\\Users\\list.txt");
            cout << endl<<"Which user are you?: ";
            cin >> users;
            while (getline(list_file, user)) {
                if (users == user) {
                    worked = true;
                }
            }
            list_file.close();
            if (worked) {
                strcat(directory, users);
                strcat(directory, "\\password.txt");//This gets the password file's directory.

                ifstream password_file;
                password_file.open(directory);
                password_file >> password;
                password_file.close();//This chunk of code sets the password to test and keep if correct

                bool logged_in = false;
                char password_input[50];
                while (!logged_in) {//This loops for the log-in
                    cout << endl << "Enter your password: ";
                    cin >> password_input;
                    //The code below tests the user input to see if it is the correct password.
                    if (password_input == password) {
                        cout << "Log-in successful!";
                        logged_in = true;
                        username = users;
                        strcat(user_directory, users);//This sets the user directory after you log in.
                        loop=false;//This stops the log-in loop
                    } else {
                        cout << "log-in failed, try again!";
                    }
                    char temp_directory[65];
                    strcpy(temp_directory,user_directory);
                    strcat(temp_directory,"\\filelist.txt");
                    strcpy(filelist_directory,temp_directory);
                }
            } else if (!worked) {
                cout << "invalid user, try again!";//This is the code that runs if the user input an invalid username
            }
        }
    }//This code is a log-in system.

    void create_file(){
        char filepath[65], filename[50], file_list_path[50];
        strcpy(filepath,user_directory);
        cout<<endl<<"Enter the new file's name: ";
        cin>>filename;
        strcat(filepath,"\\");
        strcpy(file_list_path,filepath);//This copies the filepath to the user's files
        strcat(file_list_path,"filelist.txt");//This creates the path to the list of user files.
        strcat(filepath,filename);
        strcat(filepath,".txt");
        w_file.open(filepath);
        w_file.close();

        w_file.open(file_list_path,ios::app);
        w_file<<filename<<endl;
        w_file.close();//This adds the file to the user's filelist.

    }//This function creates a new file for a logged in user.

    void create_user(){
        CreateDirectory("C:\\Notetaker",NULL);
        CreateDirectory("C:\\Notetaker\\Users",NULL);//These top two lines are used to create the folders for the program
        char new_user[50], new_password[50], new_directory[65]="C:\\Notetaker\\Users\\";
        cout<<"Create a Username without spaces: ";
        cin>>new_user;
        strcat(new_directory,new_user);
        strcpy(user_directory,new_directory);
        CreateDirectory(new_directory,NULL);//This code creates the new user's directory/folder

        cout<<"Create a password without spaces: ";
        cin>>new_password;
        strcat(new_directory,"\\password.txt");
        w_file.open(new_directory);
        w_file<<new_password;
        w_file.close();//This section of code creates the password file

        w_file.open("C:\\Notetaker\\Users\\list.txt", ios::app);
        w_file<<new_user<<endl;
        w_file.close();//This section of code adds the new users to the user list.

        username=new_user;
        password=new_password;
    }

    void open_file(){
        bool valid_filename=false;
        ifstream filelist;
        string display_list;
        filelist.open(filelist_directory);
        cout<<endl<<"AVAILABLE FILES: "<<endl;
        while(getline(filelist,display_list)){
            cout<<display_list<<endl;
        }//This while loop and the first few lines display the user's available files.
        filelist.close();

        char open_file_input[50];
        do {
            filelist.open(filelist_directory);
            cout<<endl<<"Select the file you want to open: ";
            cin>>open_file_input;
            while(getline(filelist,display_list)){
                if(open_file_input==display_list){
                    valid_filename=true;
                }
            }
            if(!valid_filename){
                cout<<"Invalid filename";
            }
            filelist.close();
        }while(!valid_filename);//This do-while loop make sure user input is a valid file from the list.

        strcpy(current_filepath,user_directory);
        strcat(current_filepath,"\\");
        strcat(current_filepath,open_file_input);
        strcat(current_filepath,".txt");
    }//Sets the current_file_path to the file the user wants to edit

    void display_file_contents(){
        r_file.open(current_filepath);
        string read_lines;
        cout<<"CURRENT NOTES: "<<endl<<"*____________________*"<<endl;
        while(getline(r_file,read_lines)){
            cout<<read_lines<<endl;
        }
        cout<<"*____________________*"<<endl;
        r_file.close();
    }//This code displays the contents of the current file.

    void file_write(){
        bool more=true, correct_input=false;
        char cont;
        string input_notes;
        w_file.open(current_filepath,ios::app);
        while(more){
            correct_input=false;
            cout<<endl<<"Enter a line of Notes: ";
            getline(cin >> ws,input_notes);
            w_file<<input_notes<<endl;
            while(!correct_input){
                cout<<"Do you want to continue writing?(y or n): ";
                cin>>cont;
                if(cont=='y'){
                    correct_input=true;
                    continue;
                }
                else if(cont=='n'){
                    correct_input=true;
                    more=false;
                }
                else{
                    cout<<endl<<"Invalid input."<<endl;
                }
            }
        }
        w_file.close();

    }

    void erase_file(){
        w_file.open(current_filepath);
        w_file.close();
    }

    void file_mode() {//This is used to select whether you will be writing or erasing
        int file_mode;
        bool valid_file_mode = false;
        while (!valid_file_mode) {
            cout << endl
                 << "Do you want to write into the file(1), or erase the file contents(2)? Input the corresponding number: ";
            cin >> file_mode;
            if (file_mode == 1) {//Is the code for writing to an existing file
                valid_file_mode = true;
                file_write();
            } else if (file_mode == 2) {//Is the code for erasing the contents of an existing file
                valid_file_mode = true;
                erase_file();
            } else {
                cout << "Invalid input, try again.";
            }
        }
    }

    void file_utilities(){//Gives the user the options to erase, write, display, and quit from the file
        bool again = false,valid_loop_input=false;
        int loop_var=0;
        do {//This loop reruns the option to change modes, and is stopped when the user wants to quit
            again=false;
            display_file_contents();
            file_mode();
            valid_loop_input=false;
            while(!valid_loop_input) {//This loop displays the modes and options.
                loop_var=0;
                cout << endl
                     << "Do you want to quit(1), select a different mode(2), or display file contents(3)? Input the corresponding number: ";
                cin >> loop_var;
                if(loop_var==1){
                    valid_loop_input=true;
                    cout<<endl<<"Thanks for using the program!";
                }
                else if(loop_var==2){
                    valid_loop_input=true;
                    again=true;
                }
                else if(loop_var==3){
                    display_file_contents();
                }
                else{
                    cout<<endl<<"Invalid input. Try again."<<endl;
                }
            }
        }while(again);
    }

};





int main() {
    user u1;
    int usermode, cont = 0;
    while (!cont) {//This while loop allows the user to select whether they are a returning user or a new user
        cout << "Are you an existing user(1) or a new user(2)? Input the corresponding number: ";
        cin >> usermode;

        if(usermode!=2){//This if ladder ensures that users input a valid response.
            if(usermode!=1){
                cout<<"Invalid input, try again!";
            }
            else{
                cont=1;
            }
        }
        else{
            cont=1;
        }
    }
    if(usermode==1){//This is for existing users
        u1.log_in();
        int choice;
        bool applicable=false;
        while(!applicable) {//This loop ensures that a valid integer is given
            cout << endl << "Do you want to open an existing file(1), or create a new file(2)?: ";
            cin >> choice;
            switch(choice){//This switch statement checks if the input is one of the possible valid inputs.
                case 1:
                    applicable=true;
                    break;
                case 2:
                    applicable=true;
                    break;
            }
        }
        if(choice==1) {//Code for opening existing file
            u1.open_file();
            u1.file_utilities();
        }


        else if(choice==2){//Code for creating new file
            u1.create_file();
            u1.open_file();
            u1.file_utilities();
        }

    }

    else if(usermode==2) {//code for new user
        u1.create_user();//function to create the user
        u1.log_in();
        u1.create_file();
        u1.open_file();
        u1.file_utilities();
    }

    return 0;
}