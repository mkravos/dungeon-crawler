/*
    PlayDungeon.cpp

    This is the driver to play a simple text-based dungeon
    game.
*/
#include "LinkedList.h"
#include "Dungeon.h"
#include <cstdlib>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstddef>
using std::cerr;
using std::cin;
using std::cout;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

string toLowerCase(string);
string toUpperCase(string);
void processLine(Dungeon&, string&);
string trim(string);
void readFile(Dungeon&, const char*);
void describeRoom(Room&);
Item& findItem(Room& room, string nm);

int main(int argc, char* argv[]) {
    //LinkedList<int>::test(); // calls LinkedList test function
    LinkedList<Item> inv;
    bool done = false;
    char* buf = new char[500];
    const char* fileName = "dungeon.txt";
    int datafileCount = 0;
    int i;
    bool debug = false;
    bool error = false;
    string action, object;
    vector<string> commands;

    // create dungeon
    Dungeon dungeon;

    // parse options and arguments to program
    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "-d") == 0) {
            debug = true;
        } else if ((strlen(argv[i]) > 0) && (argv[i][0] == '-')) {
            cerr << "Unrecognized option: " << argv[i] << endl;
            error = true;
        } else {
            if (datafileCount > 1) {
                cerr << "Error: Multiple data files specified" << endl;
                cerr << "Usage: " << argv[0] << " datafileName\n";
                error = true;
            } else {
                fileName = argv[i];
                datafileCount++;
            }
        }
    }

    // end program if error encountered
    if (error) {
        exit(1);
    }

    try {
        // read in data file to populate dungeon
        readFile(dungeon, fileName);

        // initialize dungeon
        if (dungeon.rooms.size() == 0) {
            throw string("Error: No rooms in dungeon");
        }
        if (dungeon.currentRoom == "") {
            dungeon.currentRoom = dungeon.rooms[0].id;
        }
        // display debugging info if requested
        if (debug) {
            cout << "Debugging information:\n";
            cout << "Number of rooms: " << dungeon.rooms.size() << endl;
            cout << "List of rooms:\n";
            for (unsigned int i=0; i<dungeon.rooms.size(); i++) {
                Room& room = dungeon.rooms[i];
                cout << "  Room #" << i << ": " << room.id << " (" << room.name << ")\n";
                cout << "        " << room.description << "\n";
                if (room.paths.size() > 0) cout << "  Paths:\n";
                for (unsigned int k=0; k < room.paths.size(); k++) {
                    Path& tempPath = room.paths[k];
                    cout << "    Path: " << tempPath.direction << " to " << tempPath.to << '\n';
                }
                if (room.items.size() > 0) cout << "  Items:\n";
                for (unsigned int m=0; m< room.items.size(); m++) {
                    cout << "    " << room.items[m].description << '\n';
                }
            }
            cout << "Starting room: " << dungeon.currentRoom << endl;
            cout << "And now... on to the game\n\n\n";
        }

        // play the game
        while (!done) {
            cout << '\n';
            Room& current = dungeon.getRoom(dungeon.currentRoom);
            if (current == Room::NULL_ROOM) throw string("Error: Current room is unknown.\n");
            describeRoom(current);
            if (toLowerCase(current.id) == "outside") {
                cout << "Congratulations! You have won the game.\n";
                done = true;
                break;
            }
            cout << "Enter command: ";
            cin.getline(buf, 500, '\n');
            string command(buf);
            command = trim(toLowerCase(command));
            commands.clear();
            char* cstr = new char[command.length() + 1];
            strcpy(cstr, command.c_str());
            char* cmdPtr = strtok(cstr, " ");
            while (cmdPtr != NULL) {
                commands.push_back(string(cmdPtr));
                cmdPtr = strtok(NULL, " ");
            }

            if (commands.size() == 0 || commands.size() > 2) cout << "Command not understood\n";
            else { // command is one or two tokens long
                action = commands[0];
                object = (commands.size() > 1) ? commands[1] : "";
                if (action == "go") {
                    action = object;
                    object = "";
                }

                if (action == "drop") {
                    if (object == "") cout << "You must specify an object to drop\n";
                    else {
                        for (i=inv.size()-1; i>=0; i--) {
                            Item item = inv[i];
                            if (item.name == object || object == "all") {
                                current.items.push(item);
                                inv.remove(item);
                                if (object != "all") break;
                            }
                        }
                        Item item1 = findItem(current, "bike");
                        Item item2 = findItem(current, "instructor");
                        if (item1 != Item::NULL_ITEM && item2 != Item::NULL_ITEM) {
                            current.items.remove(item1);
                            current.items.remove(item2);
                            cout << "The instructor wakes up and gets on the bike.\n";
                            cout << "Before you can ask him what's happening, he pedals off\n";
                            cout << "and leaves the building going south from the east hall.\n";
                            Path p("s", "outside");
                            Room& r = dungeon.getRoom("east hall south");
                            r.paths.push(p);
                            string old(" locked");
                            std::size_t found = r.description.rfind(old);
                            if (found != std::string::npos) {
                                (r.description).replace(found, old.length(), " now unlocked");
                            }
                            r.visited = false;
                        }
                    }
                } else if (action == "take") {
                    if (object == "") cout << "You must specify an object to take\n";
                    else {
                        for (i=current.items.size()-1; i>=0; i--) {
                            Item item = current.items[i];
                            if (item.name == object || object == "all") {
                                inv.push(item);
                                current.items.remove(item);
                                break;
                            }
                        }
                    }
                } else if (action == "inv") {
                    cout << "You are carrying: ";
                    if (inv.size() == 0) cout << "nothing";
                    else {
                        for (unsigned i=0; i<inv.size(); i++) {
                            if (i > 0) cout << ", ";
                            cout << inv[i].name;
                        }
                    }
                    cout << '\n';
                } else if (action == "quit") {
                        done = true;
                } else if (action == "exit") {
                        cout << "Use 'quit' to end the game.\n";
                } else if (action == "help") {
                        cout << "Commands are: help, quit, look, drop, take, go, inv, and the exit directions\n";
                } else if (action == "look") {
                        current.visited = false;
                } else if (action == "xyzzy") {
                    // check for regalia in inventory
                    bool hasRegalia = false;
                    for (unsigned int i=0; i<inv.size(); i++) {
                        if (inv[i].name == "regalia") hasRegalia = true;
                    }
                    if (hasRegalia) {
                        if (dungeon.currentRoom == "A-1342") dungeon.currentRoom = "A-1374";
                        else if (dungeon.currentRoom == "A-1374") dungeon.currentRoom = "A-1342";
                        else cout << "Nothing happens.\n";
                    } else {
                        cout << "Does this look like a colossal cave?\n";
                    }
                } else {
                        Path& path = current.getPath(action);
                        if (path == Path::NULL_PATH) {
                            cout << "Unknown command. Try again.\n";
                        } else {
                            Room& room = dungeon.getRoom(path.to);
                            if (room == Room::NULL_ROOM) {
                                cout << "Path doesn't lead to a known room.\n";
                            } else {
                                dungeon.currentRoom = room.id;
                            }
                        }
                }
            }
        }
        cout << "Thanks for playing. Visit again soon.\n";
    } catch (string msg) {
        cerr << msg << endl;
    }

    return 0;
}
// routine to process a dungeon data file
// this will throw an exception if it has any problems
void readFile(Dungeon& dungeon, const char* filename) {
    ifstream ifile(filename);
    if (!ifile) {
        throw string("Error: Could not open data file");
    }
    char buf[2000];
    string linein, previousLine;
    do {
        ifile.getline(buf, 2000, '\n');
        if (ifile.eof()) break;
        if (!ifile) {
            throw string("Error: Problem reading data file");
        }
        string temp(buf);
        if (temp.length() == 0) continue;
        string start = temp.substr(0,5);
        if (start=="ROOM:" || start=="PATH:" || start=="INIT:" || start=="ITEM:") {
            processLine(dungeon, previousLine);
            previousLine = "";
        }
        if (previousLine.length() > 0) previousLine = previousLine + " ";
        previousLine = previousLine + trim(temp);
    } while (!ifile.eof());
    processLine(dungeon, previousLine);
    ifile.close();
}
// routine to process one line of a dungeon data file
// this will throw an exception if it has any problems
void processLine(Dungeon& dungeon, string& line) {
    string start = line.substr(0,5);
    string field1, field2, field3;
    field1 = field2 = field3 = "";
    if (start == "ROOM:" || start == "PATH:" || start == "ITEM:") {
        int pos1 = line.find(':', 0);
        if (pos1==(int)string::npos) {
            throw string("Error: Problem parsing data file");
        }
        int pos2 = line.find(':', pos1+1);
        if (pos2==(int)string::npos) {
            throw string("Error: Problem parsing data file");
        }
        int pos3 = line.find(':', pos2+1);
        if (pos3==(int)string::npos) {
            throw string("Error: Problem parsing data file");
            return;
        }
        field1 = line.substr(pos1+1, pos2-pos1-1);
        field1 = trim(field1);
        field2 = line.substr(pos2+1, pos3-pos2-1);
        field2 = trim(field2);
        field3 = line.substr(pos3+1, line.length()-pos3-1);
        field3 = trim(field3);
        if (start == "ROOM:") {
            if (dungeon.getRoom(field1) != Room::NULL_ROOM) {
                throw string("Error: Duplicate room ID found in input file");
            }
            dungeon.rooms.push(Room(field1, field2, field3));
        } else if (start == "PATH:") {
            Room& room = dungeon.getRoom(field2);
            if (room == Room::NULL_ROOM) {
                throw string("Error: Path from unknown room encountered in input file");
            }
            Path& path = room.getPath(field1);
            if (path != Path::NULL_PATH) {
                throw string("Error: Duplicate path source encountered in input file");
            }
            room.paths.push(Path(field1, field3));
        } else { // start must be "ITEM:"
            Room& room = dungeon.getRoom(field3);
            if (room == Room::NULL_ROOM) {
                throw string("Error: Item placed in unknown room");
            } else {
                room.items.push(Item(field1, field2, field3));
            }
        }
    } else if (start == "INIT:") {
        field2 = line.substr(5);
        field2 = trim(field2);
        dungeon.currentRoom = trim(field2);
    }
    return;
}

// eliminates spaces from beginning and end of a string
string trim(string src) {
    string s(src);
    int pos1 = s.find_first_not_of(" \t\n");
    int pos2 = s.find_last_not_of(" \t\n");
    if ((pos1 != (int)string::npos) && (pos2 != (int)string::npos)) {
        s = s.substr(pos1,pos2-pos1+1);
    }
    return s;
}

// prints room description
// diplays short description if room is marked as
// visited and long description otherwise
void describeRoom(Room& room) {
    cout << "You are in " << room.id << '\n';
    if (!room.visited) cout << room.description << '\n';
    if (room.paths.size() == 0) {
        cout << "There are no exits\n";
    } else {
        for (unsigned int i=0; i< room.items.size(); i++) {
            cout << room.items[i].description << '\n';
        }
        cout << "Exits are: ";
        for (unsigned int i=0; i< room.paths.size(); i++) {
            if (i > 0) cout << ", ";
            cout << room.paths[i].direction;
        }
        cout << endl;
    }
    room.visited = true;
}

// converts a string to all lower case
string toLowerCase(string src) {
    string s(src);
    for (int i=0; i<(int)s.length(); i++) s[i] = tolower(s[i]);
    return s;
}

// converts a string to all upper case
string toUpperCase(string src) {
    string s(src);
    for (int i=0; i<(int)s.length(); i++) s[i] = toupper(s[i]);
    return s;
}

Item& findItem(Room& room, string nm) {
    for (unsigned i=0; i< room.items.size(); i++) {
        if (room.items[i].name == nm) return room.items[i];
    }
    return Item::NULL_ITEM;
}

