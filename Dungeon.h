/*
    Dungeon.h

    This is the header file for a Dungeon object. It also
    serves as the header file for Room and Path objects.
*/

#ifndef __DUNGEON_H__
#define __DUNGEON_H__

#include "LinkedList.h"
#include <string>
using std::string;

class Path {
public:
    string direction;
    string to;
    Path();
    Path(string dir, string to);
    bool operator==(const Path& obj) const;
    bool operator!=(const Path& obj) const;
    static Path NULL_PATH;
};

class Item {
public:
    string name;
    string description;
    string location;
    Item();
    Item(string name, string description, string location);
    bool operator==(const Item& obj) const;
    bool operator!=(const Item& obj) const;
    static Item NULL_ITEM;
};

class Room {
public:
    bool visited;
    string id;
    string name;
    string description;
    LinkedList<Path> paths;
    LinkedList<Item> items;
    Room();
    Room(string id, string name, string desc);
    Path& getPath(string dir);
    bool operator==(const Room& obj) const;
    bool operator!=(const Room& obj) const;
    static Room NULL_ROOM;
};

class Dungeon {
public:
    LinkedList<Room> rooms;
    string currentRoom;
    Room& getRoom(string id);
};

#endif
