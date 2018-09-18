// Lab 3
// COSC130
// This program allows the user to move around in a text-based dungeon
// Joey Lemon
// 9/21/18
// TA: Jacob

#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// Create the rooms class to hold a room's information
class Room {
    public:
    
    string title;
    string desc;
    int id;
    
    // Assign sentinel values to the exit rooms
    int n = -1;
    int s = -1;
    int e = -1;
    int w = -1;
    
    // Print the room's information
    void Look(){
        printf("%s\n", title.c_str());
        printf("%s\n", desc.c_str());
        
        // Only print the exits that exist
        printf("Exits: ");
        if(n != -1){
            printf("n ");
        }
        if(s != -1){
            printf("s ");
        }
        if(e != -1){
            printf("e ");
        }
        if(w != -1){
            printf("w");
        }
        printf("\n");
    }
};

// Create the dungeon struct to hold rooms
struct Dungeon {
    Room *rooms;
    int room_amnt;
} Dungeon;

// Get the number of rooms in the file
int GetNumRooms(ifstream &file);

// Add the room data to the dungeon struct
void SetRoomData(ifstream &file);

// Start the game
void PlayGame();

int main(int argc, char *argv[]) {
    // Ensure the user provided a file
    if(argc == 1){
        printf("Usage: %s <file>\n", argv[0]);
        return 0;
    }
    
    // Open the provided file
    string file_name(argv[1]);
    string line;
    ifstream file(file_name);
    
    // Ensure the file exists
    if(file.is_open()){
        // Set the amount of rooms in the file
        Dungeon.room_amnt = GetNumRooms(file);
        
        // Initialize the dungeon with correct amount of rooms
        Dungeon.rooms = new Room[Dungeon.room_amnt];
        
        // Fill the dungeon struct with room data
        SetRoomData(file);
        
        // Close the file after getting all the data
        file.close();
        
        // Begin the game
        PlayGame();
        
        // Return the memory
        delete [] Dungeon.rooms;
    }else{
        // Print an error if the file can't open
        printf("That file doesn't exist.\n");
    }
    
    return 0;
}

int GetNumRooms(ifstream &file) {
    string line;
    int line_num = 0;
    int room_num = 0;
    
    // Scan the file to find number of rooms
    while (getline(file, line)){
        // Count lines as separated by a tilde
        if(line[0] == '~'){
            line_num++;
            
            // If the line is the third line, increment room counter
            if(line_num % 3 == 0 && line_num != 0){
                room_num++;
            }
        }
    }
    
    return room_num;
}

void SetRoomData(ifstream &file) {
    // Go back to the beginning of the file
    file.clear();
    file.seekg(0, ios::beg);
    
    string line;
    int line_num = 0;
    int room_num = 0;
    
    // Scan the file to set room data
    while (getline(file, line)){
        // Track the type of data being read
        int step = line_num % 3;
        
        if(line[0] != '~'){
            if(step == 0){ // Title step
                // Set the room's title and id
                Dungeon.rooms[room_num].title = line;
                Dungeon.rooms[room_num].id = room_num;
            }else if(step == 1){ // Desc step
                // Set the room's description
                Dungeon.rooms[room_num].desc += line + "\n";
            }else if(step == 2){ // Directions step
                // Set the room's directions
                char dir;
                int to_room;
                
                // Use string stream to easily read data
                istringstream stream;
                stream.str(line);
                
                // Read the direction character
                stream >> dir;
                
                // Read the direction number
                stream >> to_room;
                
                // Set the correct direction room depending on character
                switch(dir){
                    case 'n':
                        Dungeon.rooms[room_num].n = to_room;
                        break;
                    case 's':
                        Dungeon.rooms[room_num].s = to_room;
                        break;
                    case 'e':
                        Dungeon.rooms[room_num].e = to_room;
                        break;
                    case 'w':
                        Dungeon.rooms[room_num].w = to_room;
                        break;
                }
            }
        }else{
            // Increment counters when line is a tilde
            line_num++;
            if(line_num % 3 == 0 && line_num != 0){
                room_num++;
            }
        }
    }
}

void PlayGame() {
    // Print console commands
    printf("Commands:\n");
    printf("    q - Quit\n");
    printf("    l - Look into your current room\n");
    printf("    n, s, e, w - Move a direction\n");
    
    // Track the room the user is in
    int current_room = 0;
    
    char action = ' ';
    
    // Continue asking for input until user enters q
    while(action != 'q'){
        printf("> ");
        
        // Read in a character from the user
        scanf(" %c", &action);
        
        // Get the room the user is in
        Room room = Dungeon.rooms[current_room];
        
        if(action == 'l'){
            // Print the room's information
            room.Look();
        }else if(action == 'n'){
            // Ensure there's a room in that direction
            if(room.n != -1){
                current_room = room.n;
                printf("You have moved NORTH.\n");
            }else{
                // Print error if there's no room
                printf("You can't go NORTH.\n");
            }
        }else if(action == 's'){
            if(room.s != -1){
                current_room = room.s;
                printf("You have moved SOUTH.\n");
            }else{
                printf("You can't go SOUTH.\n");
            }
        }else if(action == 'e'){
            if(room.e != -1){
                current_room = room.e;
                printf("You have moved EAST.\n");
            }else{
                printf("You can't go EAST.\n");
            }
        }else if(action == 'w'){
            if(room.w != -1){
                current_room = room.w;
                printf("You have moved WEST.\n");
            }else{
                printf("You can't go WEST.\n");
            }
        }else if(action == 'q'){
            break;
        }
    }
}
