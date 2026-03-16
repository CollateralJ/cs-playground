/*
    Description: re3 pathfinder, escape nemesis and rpd
    Input: adjList file for map, values of each room (hurt or heal)
    Output: path taken or death status
*/

// Includes
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <list>
using namespace std;

struct room
{
    list<string> adjList;     // doors to other rooms
    string predecessor;             // room we used to get here
    int healthInRoom;               // the health jill had when she last entered this room
    int value;                      // damage/heal value of the room
        // if value = 0, nemesis is not in that room
        // if value > 0, nemesis will deal this much dmg in that room
        // if value < 0, room has healing item
};

// Function prototypes
bool escapeRPD(string currentRoom, string goalRoom, 
    int hp, unordered_map<string, room>& rpdMap);
void outputPathTo(string startRoom, string goalRoom, unordered_map<string, room> rpdMap);

int main(int args, char * argv[])
{
    // room name -> struct that contains the rooms values
    unordered_map<string, room> rpdMap;
    
    // make adjLists and set room values
    ifstream adjFile;
	adjFile.open(argv[1]);
    string r, n; // room, neighbor
    while(adjFile >> r)
    {
        adjFile >> n;
        rpdMap[r].adjList.push_back(n);
    }
    adjFile.close();

    // set room values
    ifstream valFile;
    valFile.open(argv[2]);
    int v; // value
    while(valFile >> r)
    {
        valFile >> v;
        rpdMap[r].value = v;
    }
    valFile.close();
    
    // escape from rpd
    if (escapeRPD("WestStairway1F", "Courtyard", 20, rpdMap))
    { // path found!
        // output path
        outputPathTo("WestStairway1F", "Courtyard", rpdMap);
        cout << endl;
        cout << "Somehow...I'm still alive...\n";
    } else cout << "STARS!!!\n"; // died

    // output the path taken
    // output(rpdMap);

    return 0;
}

bool escapeRPD(string currentRoom, string goalRoom, 
    int hp, unordered_map<string, room>& rpdMap)
{
    // update hp
    hp -= rpdMap[currentRoom].value;

    if(rpdMap[currentRoom].healthInRoom > hp) // check if current path is worse than before
        return false; // this is not worth trying again at lower health...
    rpdMap[currentRoom].healthInRoom = hp; // maybe this is the correct path

    if(hp <= 0) return false; // X_X ?

    if(currentRoom == goalRoom) return true; // mission success

    for(auto neighbor : rpdMap[currentRoom].adjList)
    {
        rpdMap[neighbor].predecessor = currentRoom;
        if (escapeRPD(neighbor, goalRoom, hp, rpdMap)) // search neighboring rooms
        {
            return true; 
        }
    }
        
    return false;
}

void outputPathTo(string startRoom, string goalRoom, unordered_map<string, room> rpdMap)
{
    string room = goalRoom;
    list<string> order;
    while(1)
    {
        order.push_back(room);
        string nextRoom = rpdMap[room].predecessor;
        if ((nextRoom == "") || (room == startRoom)) break;
        room = rpdMap[room].predecessor;
    }
    cout << "Path: ";
    for(auto room = --(order.end()); room != order.end(); )
    {
        cout << *room;
        if (--room != order.end()) cout << " -> ";
    }
    cout << endl;
}
