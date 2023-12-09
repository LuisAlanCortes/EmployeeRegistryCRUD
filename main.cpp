/*
    Employee Registry Manager - Get registry data from a file, validate, and allow user to modify(add and remove),
        search all data(By last name or ID), & display managers/staff
*/
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;

struct employeeRegistry{//Struct for each employee's data
    string firstName;
    string lastName;
    string employeeID;
    string phoneNumber;
};

int getSize(ifstream& input){//Get size of array by get-line-ing our file and incrementing each line
    int i = 0;//Counter to designate size
    string temp = "";//Temp to just hold the line
    while(!input.eof()){//Keep reading line until the end of file
        getline(input, temp);//Get line to move the read marker for every line
        i++;//Size increment
    }
    input.clear();//Clear the eof record to read again in another function
    input.seekg(0, std::ios::beg);//Reset the read position to the beginning
    return i;//Return size
}

void processData(ifstream& input, struct employeeRegistry employees[]){//Get data into struct until end of file
    int i = 0;//Employee counter
    while(!input.eof()){//Until end of file
        getline(input, employees[i].firstName, ' ');//Fill each struct data in order, delimited by spaces
        getline(input, employees[i].lastName, ' ');
        getline(input, employees[i].employeeID, ' ');
        getline(input, employees[i].phoneNumber);//Don't delimit, no need, should be end of line
        i++;//Increment employee
    }
}

employeeRegistry* resizeEmployeeRegistry(struct employeeRegistry* employees, int size, int& capacity, int factor){//Resize struct based on capacity, return pointer to new resized struct array
    int oldCapacity = capacity;//Save old capacity to use in for loop
    capacity *= factor;//Multiply capacity by factor
    employeeRegistry* temp = new employeeRegistry[capacity];//New array with new bigger capacity  
    for(int i = 0; i < oldCapacity; ++i){//Replace new array with old arrays contents
        temp[i].firstName = employees[i].firstName;
        temp[i].lastName = employees[i].lastName;
        temp[i].employeeID = employees[i].employeeID;
        temp[i].phoneNumber = employees[i].phoneNumber;
    }
    delete[] employees;//Delete old array memory
    return temp;//Updates the pointer of array to instead point to our new array
}

void displayEmployee(struct employeeRegistry employees[], int index){//Display an employee info with the given index(Generally i)
    cout << left << setw(17) << employees[index].firstName << left << setw(15) << employees[index].lastName << left << setw(20) << employees[index].employeeID << left << setw(10) << employees[index].phoneNumber << endl;
}

void displayHeader(){//Display table header for employee display
    cout << left << setw(17) << "First Name" << left << setw(15) << "Last Name" << left << setw(20) << "Employee ID" << left << setw(20) << "Phone Number" << endl;//Headers seperated by setw
    cout << "----------------------------------------------------------------" << endl;//Line to separate header and employee 
}

void validateID(struct employeeRegistry employees[], int capacity, int size, int i){//Validate ID: Length, number correctness, & correct starting digit
    for(int k = 0; k < employees[i].employeeID.length(); k++){//IV - ID is a number - Loop through length and check each char
        if(!isdigit(employees[i].employeeID[k])){
            cout << "\nError: " << employees[i].firstName << " " << employees[i].lastName//Error message and re-entry
                 << " | ID given: " << employees[i].employeeID
                 << " | ID is not a number. Re-enter ID: ";
            getline(cin, employees[i].employeeID);
        }
    }

    while(employees[i].employeeID.length() != 8){//IV - ID Length
        cout << "\nError: " << employees[i].firstName << " " << employees[i].lastName
             << " | ID given: " << employees[i].employeeID
             << " | ID is an invalid length. ID must be 8 digits long. Re-enter ID: ";
        getline(cin, employees[i].employeeID);
    }

    string temp = "";//Temp for get line from user to convert to stoi & validate
    while((employees[i].employeeID[0] != '1') && (employees[i].employeeID[0] != '8')){//Check if employee ID starts correctly (1 or 8) by checking the first char
        cout << "\nError: " << employees[i].firstName << " " << employees[i].lastName//Display error message
             << " is not a manager or an employee according to their ID."
             << " | ID given: " << employees[i].employeeID
             << " | Input 1 to designate them as a manager, or 2 as a staff member: ";
        getline(cin, temp);//Store in temp to concat to front of old ID
        while((stoi(temp) != 1) && (stoi(temp) != 2)){//IV - If the user doesn't enter 1 or 2
            cout << "\nError: Input 1 for manager assignment, or 2 for staff: ";
            getline(cin, temp);
        }
        if(stoi(temp) == 2){//Assign 8 to temp if user selects manager - Use 2 for UX
            temp = "8";
        }
        employees[i].employeeID.front() = temp.at(0);//Concat selection to the front of ID
    }
}

void validateDuplicates(struct employeeRegistry employees[], int size){//Validate duplicates of both ID & Phone number
    for(int i = 0; i < size; i++){//Nested for loop to loop through struct array then every element after that element
        for(int k = i+1; k < size; k++){
            if(employees[i].employeeID == employees[k].employeeID){//Check every element against every other element
                cout << "\nError: " << employees[i].firstName << " " << employees[i].lastName//Display error if duplicate, display names of duplicate employees
                     << " | ID is a duplicate of: " << employees[k].firstName << " " << employees[k].lastName
                     << " | Re-enter ID for " << employees[i].firstName << " " << employees[i].lastName << ": ";
                getline(cin, employees[i].employeeID);//Get re-entry
            }
            if(employees[i].phoneNumber == employees[k].phoneNumber){//Check every element against every other element
                cout << "\nError: " << employees[i].firstName << " " << employees[i].lastName//Display error if duplicate, display names of duplicate employees
                     << " | Phone Number is a duplicate of: " << employees[k].firstName << " " << employees[k].lastName
                     << " | Re-enter Phone Number for " << employees[i].firstName << " " << employees[i].lastName << ": ";
                getline(cin, employees[i].phoneNumber);//Get re-entry
            }
        }
    }
}

void validatePhone(struct employeeRegistry employees[], int capacity, int size, int i){//Validate Phone Number: Length & Correct structure(Dashes & Number checking)
    while(employees[i].phoneNumber.length() != 12){//Check phone number length
        cout << "\nError: " << employees[i].firstName << " " << employees[i].lastName//Display error message
             << " | Phone Number given: " << employees[i].phoneNumber
             << " | Phone number is not the correct length, remember to include dashes. Re-enter Phone Number: ";
        getline(cin, employees[i].phoneNumber);//Get re-entry
    }

    bool phoneCharCheck = false, dataChange = false;//Bool to check if data is correct, and bool if data is incorrect
    while(!phoneCharCheck){//While data is not correct - IV: Correct format for Phone Number
        dataChange = false;//Reset "incorrect data" bool every loop
        for(int k = 0; k < 3; k++){//Loop through first 3 numbers
            if(!isdigit(employees[i].phoneNumber[k])){//Confirm they are numbers with is digit
                dataChange = true;//Flip our "this data is incorrect" bool
            }
        }
        if(employees[i].phoneNumber[3] != '-'){//Check if the 4th char is a -
            dataChange = true;//Flip incorrect
        }
        for(int k = 4; k < 7; k++){//Loop through middle 3 digits for number validity
            if(!isdigit(employees[i].phoneNumber[k])){
                dataChange = true;
            }
        }
        if(employees[i].phoneNumber[7] != '-'){//2nd dash check
            dataChange = true;
        }
        for(int k = 8; k < 11; k++){//Loop through last 4 numbers
            if(!isdigit(employees[i].phoneNumber[k])){
                dataChange = true;
            }
        }
        if(dataChange){//If data is incorrect, change from user. Loop again to confirm users input 
            cout << "\nError: " << employees[i].firstName << " " << employees[i].lastName//Display error message
                 << " | Phone Number given: " << employees[i].phoneNumber
                 << " | Phone Number is not a number, or is missing dashes. Re-enter Phone Number: ";
            getline(cin, employees[i].phoneNumber);
        }
        else{//If data was not incorrect, this data was clean, and we can break - Checks twice if incorrect to ensure re-entry is correct
            phoneCharCheck = true;
        }
    }
}

void validateData(struct employeeRegistry employees[], int capacity, int size){//Validates ID & phone number by calling duplicate check function and individual validation functions
    validateDuplicates(employees, size);//Validate ID & phone number for duplicates
    for(int i = 0; i < size; i++){//Loop through struct array by size, sending each elements index for validation
        validateID(employees, capacity, size, i);//Validate Phone Number: Length & Correct structure(Dashes & Number checking)
        validatePhone(employees, capacity, size, i);//Validate Phone Number: Length & Correct structure(Dashes & Number checking)
    }
}

void displayEmployeeRegistry(struct employeeRegistry employees[], int size){//Display all employees in a table
    displayHeader();//Display table header
    for(int i = 0; i < size; i++){//Loop through struct array by size
        displayEmployee(employees, i);//Display each employee in struct
    }
}

void searchLastName(struct employeeRegistry employees[], int size){//Sort registry by last name then Search registry by last name w/ binary search and display struct info for employee 
    string searchTerm = "";//Get search term from user
    cout << "Input the employee's last name you would like to find: ";
    getline(cin, searchTerm);

    bool found = false;//Found boolean
    for (int i = 0; i < size; i++){//Loop through employees struct array by size
        if (employees[i].lastName == searchTerm){ //Linear Search, if element = search term
            if(found){//In case of employees with multiple last names, just display the employee, instead of the table header
                displayEmployee(employees, i);//Display Employee info
            }
            else{//If this is the first employee found, display header and employee
                displayHeader();//Display table headers
                displayEmployee(employees, i);//Display Employee info
            }
            found = true;//Flip boolean to not display error if found
        }
    }
    if (!found){ //if name was not found, display error
        cout << "Error: Last name was not found." << endl;
    }
}

void removeEmployee(struct employeeRegistry employees[], int& size){//Remove employee by searching last name or employee ID then pushback all array elements
    string userChoice = "";//Get employee delete search choice
    cout << "Input 1 to delete employee by last name. Input 2 to delete employee by ID" << endl;
    getline(cin, userChoice);
    while (stoi(userChoice) < 1 || stoi(userChoice) > 2){//IV - Not 1 or 2
        cout << "Error: Input 1 or 2" << endl;
        getline(cin, userChoice);
    }

    if(stoi(userChoice) == 1){//Delete by last name
        string searchTerm = "";//Get Last name to search
        cout << "Input the employee's last name you would like to delete: ";
        getline(cin, searchTerm);

        bool found = false;//Found boolean
        for (int i = 0; i < size; i++){//Loop through employees struct array by size
            if (employees[i].lastName == searchTerm){ //Linear Search, if element = search term
                found = true;//Flip boolean
                cout << "Input 1 if you would you like to delete this user: " << endl;//Because multiple employees can have the same last name, we have to confirm each deletion
                displayHeader();//Display header
                displayEmployee(employees, i);//Display employee that will be deleted
                getline(cin, userChoice);//Get user choice on deletion
                if(stoi(userChoice) == 1){//Delete user by overwriting it with pushback
                    for(int k = i; k < size; k++){//Loop through struct array from position of employee to be deleted
                        employees[k].firstName = employees[k + 1].firstName;//Like a vector, push all employees up to "delete" our old element
                        employees[k].lastName = employees[k + 1].lastName;
                        employees[k].employeeID = employees[k + 1].employeeID;
                        employees[k].phoneNumber = employees[k + 1].phoneNumber;
                    }
                    size--;//Decrement size to track true size of struct array
                }
            }
        }
        if (!found){//Display error if not found
            cout << "Error: Last name was not found." << endl;
        }
    }
    else{//Delete by ID
        string searchTerm = "";//Get ID to search for
        cout << "Input the employee's ID you would like to delete: ";
        getline(cin, searchTerm);

        bool found = false;//Found boolean
        for (int i = 0; i < size; i ++ ){//Loop through employees struct array by size
            if (employees[i].employeeID == searchTerm){//Linear Search, if element = search term
                found = true;//Flip boolean 
                cout << "Employee being deleted: " << endl;//No need to confirm deletion, already validated for duplicates
                displayHeader();//Display header
                displayEmployee(employees, i);//Display Employee info
                for(int k = i; k < size; k++){//Loop through struct array from position of employee to be deleted
                    employees[k].firstName = employees[k + 1].firstName;//Like a vector, push all employees up to "delete" our old element
                    employees[k].lastName = employees[k + 1].lastName;
                    employees[k].employeeID = employees[k + 1].employeeID;
                    employees[k].phoneNumber = employees[k + 1].phoneNumber;
                }
                size--;//Decrement size to track true size of struct array
            }
        }
        if (!found){//Display error if not found
            cout << "Error: Employee ID was not found." << endl;
        }
    }
}

void addEmployee(struct employeeRegistry* &employees, int& size, int& capacity, int factor){//Add employee given by user, resize array if needed
    size++;//Increment size to check if array will reach out of bounds
    if(capacity <= size){//If our employee causes us to reach out of bounds, resize
        employees = resizeEmployeeRegistry(employees, size, capacity, factor);//Point to new struct array 
    }
    cout << "\nInput First Name: ";//Input all variables for new ID
    getline(cin, employees[size-1].firstName);//We can add to size, as size is not an array num, so it did not start from 0
    cout << "\nInput Last Name: ";
    getline(cin, employees[size-1].lastName);
    cout << "\nInput Employee ID: ";
    getline(cin, employees[size-1].employeeID);
    cout << "\nInput Phone Number: ";
    getline(cin, employees[size-1].phoneNumber);
    validateData(employees, capacity, size);//Validate our data again to ensure correctness
}

void displayStaff(struct employeeRegistry employees[], int size){//Display just staff members (IDs starting with 1)
    cout << "All Staff: " << endl;//Header of header
    displayHeader();//Display table header 
    for (int i = 0; i < size; i++){//Loop through struct array by size
        if (employees[i].employeeID[0] == '1'){ //Linear Search, if element = search term
            displayEmployee(employees, i);//Display Employee info
        }
    }
}

void displayManagers(struct employeeRegistry employees[], int size){//Display just manager members (IDs starting with 8)
    cout << "All Managers: " << endl;//Header of header
    displayHeader();//Display table header 
    for (int i = 0; i < size; i++){//Loop through employees struct array by size
        if (employees[i].employeeID[0] == '8'){ //Linear Search, if element = search term
            displayEmployee(employees, i);//Display Employee info
        }
    }
}

int displayMenu(){//Display menu options in table and return menu choice
    int menuChoice = 0;
    string temp = "";//Temp to hold get line for conversion
    cout << "----------------------------------------------------------------" << endl
         << "                             Menu" << endl
         << "----------------------------------------------------------------" << endl
         << "                   1. Display Employee Registry" << endl
         << "                   2. Search By Last Name" << endl
         << "                   3. Add Employee" << endl
         << "                   4. Remove Employee" << endl
         << "                   5. Display Manager List" << endl
         << "                   6. Display Staff List" << endl
         << "                   7. Exit" << endl
         << "----------------------------------------------------------------" << endl;
    getline(cin, temp);
    menuChoice = stoi(temp);//Convert temp to int for menu choice

    while((menuChoice <= 0) || (menuChoice > 7)){//Input Validation - <= 0 + > 7
        cout << "Choice cannot be less than 0 or greater than menu size. Re-Enter Input" << endl;
        getline(cin, temp);
        menuChoice = stoi(temp);
    }
    return menuChoice;
}

void processMenu(struct employeeRegistry employees[], bool& sentinel, int menuChoice, int& size, int& capacity, double factor){//Process menu input
    switch(menuChoice){//Switch for menu choice to decide which function to call
        case 1:{//Display all
            displayEmployeeRegistry(employees, size);
            break;
        }
        case 2:{//Search and display by last name
            searchLastName(employees, size);
            break;
        }
        case 3:{//Add an Employee
            addEmployee(employees, size, capacity, factor);
            break;
        }
        case 4:{//Remove an Employee
            removeEmployee(employees, size);
            break;
        }
        case 5:{//Display all managers
            displayManagers(employees, size);
            break;
        }
        case 6:{//Display all staff
            displayStaff(employees, size);
            break;
        }
        case 7:{//Exit 
            sentinel = true;//Set sentinel to false to exit menu loop
            cout << "Exit: Thanks For Playing!" << endl;//Exit message
            break;
        }
        default:{
            cout << "Switch Error" << endl;
        }
    }
}

int main()
{
    string registryFileAddress = R"(C:\Users\Cortes\CLionProjects\EmployeeRegistryCRUD\employees.dat)";
    ifstream input;//Open data file
    input.open(registryFileAddress);
    if(!input){//Display error and exit
        cout << "Error opening file\n";
        system("pause");
        exit(-1);
    }

    double factor = 1.5;//Factor that we will multiply size by
    int size = getSize(input);//Count every line of the file and use it as the size of our struct array
    int capacity = size * factor;//Calc capacity by multiplying size and factor
    employeeRegistry* employees = new employeeRegistry[capacity];//Dynamic Struct array

    processData(input, employees);//Add data from file to struct array

    validateData(employees, capacity, size);//Validate all data, including checking for duplicates

    bool sentinel = false;//Sentinel for menu break
    int menuChoice = 0;//User Menu choice
    while(!sentinel){//Sentinel controlled loop
        menuChoice = displayMenu();//Display menu and return choice
        processMenu(employees, sentinel, menuChoice, size, capacity, factor);//Process menu and allow for sentinel exit
    }
    input.close();//Close file
    delete[] employees;//Delete dynamic array
    return 0;
}
/*
    Struct employee registry - Holds First/Last names, employee ID, and phone number
    Open file, read lines to size struct array
    Calc capacity of array to 1.5 factor size
    Create dynamically allocated struct array with capacity
    Process data into struct array, delimit by spaces
    Validate phone number and IDs for duplicates, length, correctness, and structure
        Loop through struct array checking each element for each criteria
    Sentinel controlled menu
        List all employee details
            Loop through struct array and display all elements in a table using setw
        List employee by last name
            Linear Search through array with given last name from user and return the employee if found, if not error
                Can have duplicates so continue to search if found
        Add a new employee
            Resize struct array if needed, add employee element at size
        Remove an employee
            Search for employee by last name or ID, pushback array to overwrite for employee, error if not found
                If last name, ask for confirmation on deletion
        Show all managers only
            Search by the first element of the employee ID(8), display each element with match
        Show all staff only
            Search by the first element of the employee ID(1), display each element with match
        Quit
            Flip sentinel
    Close file
    Delete dynamic array
*/