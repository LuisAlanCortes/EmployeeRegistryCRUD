Employee Registry Create, Read, Update, Delete project that reads in a .dat file of known format and ports data into a struct.
The struct is a resizeable array using pointers.
Data is validated for correctness, and the user is prompted to fix any data after data file is read.
![image](https://github.com/LuisAlanCortes/EmployeeRegistryCRUD/assets/110257548/24954f35-116a-4150-9e42-2bfe62f17690)

Users can prefrom all CRUD utilities from console menu.
![image](https://github.com/LuisAlanCortes/EmployeeRegistryCRUD/assets/110257548/33a65aff-9173-431f-a9c1-9b786f9aae1d)

Code Breakdown:
Struct employee registry - Holds First/Last names, employee ID, and phone number
    Open file, read lines to size struct array
    Calc capacity of array to 1.5 factor size
    Create dynamically allocated struct array with capacity
    Process data into struct array, delimit by spaces
    Validate phone number and IDs for duplicates, length, correctness, and structure
        Loop through struct array checking each element for each criteria
    Sentienl controlled menu 
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

CPP1 Final project. 
