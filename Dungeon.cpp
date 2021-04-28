/*
    Dungeon.cpp

    This is the implementation file for a Dungeon object. It also
    serves as the implementation file for Room, Item, and Path objects.
*/

#include "Dungeon.h"

Path Path::NULL_PATH("NULL", "NULL");
Room Room::NULL_ROOM("NULL", "NULL", "NULL");
Item Item::NULL_ITEM("NULL", "NULL", "NULL");

Path::Path() : direction(""), to("") {}
Path::Path(string dir, string to) : direction(dir), to(to) {}
bool Path::operator==(const Path& obj) const {
    return direction == obj.direction && to == obj.to;
}
bool Path::operator!=(const Path& obj) const {
    return direction != obj.direction || to != obj.to;
}

Item::Item() : name(""), description(""), location("") {}
Item::Item(string nm, string desc, string loc) : name(nm), description(desc), location(loc) {}
bool Item::operator==(const Item& obj) const {
    return name == obj.name && description == obj.description && location == obj.location;
}
bool Item::operator!=(const Item& obj) const {
    return name != obj.name || description != obj.description || location != obj.location;
}

Room::Room() : visited(false), id(""), name(""), description("") {}
Room::Room(string id, string name, string desc) : visited(false), id(id), name(name), description(desc) {}
Path& Room::getPath(string dir) {
    for (unsigned int i=0; i<paths.size(); i++) {
        if (paths[i].direction == dir) return paths[i];
    }
    return Path::NULL_PATH;
}
bool Room::operator==(const Room& obj) const {
    return name == obj.name && description == obj.description && id == obj.id;
}
bool Room::operator!=(const Room& obj) const {
    return name != obj.name || description != obj.description || id != obj.id;
}

Room& Dungeon::getRoom(string id) {
    for (unsigned int i=0; i<rooms.size(); i++) {
        if (rooms[i].id == id) return rooms[i];
    }
    return Room::NULL_ROOM;
}

