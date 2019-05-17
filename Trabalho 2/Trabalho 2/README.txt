All the functionalities have been implemented
All the input parsing is tolerant to whitespace and fairly resilient against malformed input
The Clients and Travelpacks object management is handled through memory allocation and pointers
Utils.h is the only file thoroughly documented, the rest of the project is lacking in documentation
Due to the formatting in some functions, its recommended to run the program terminal in full screen, for readability purposes
We moved the management functionalities #9 and #10 to the "Clients" and "Travelpacks" submenus
We decided to add the possibility of a Client not having any travelpacks purchased, in that case, a '0' will be present in the "travelpacks purchased" data field in the clients' text file
