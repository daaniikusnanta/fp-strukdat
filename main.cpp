/* Final Project Struktur Data
   Sistem Tracing dan Tracking Virus Berbasis Tempat dan Orang
   oleh Daani Nabil G K 05111940000163
   Struktur Data C, Informatika, ITS
*/

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <cmath>

using namespace std;

bool isAllDigit(string);
int digit(int);
int hashing(string);
vector<string> getCommand();
void showHelp();

class Place {
public:
    string name;
    string stat;
    vector<string> edgePerson;
};

class Person {
public:
    string id;
    string name;
    string stat;
    int age = 0;
    vector<string> edgePlace;
    vector<string> edgePerson;
};

class Graph {
    vector<list<Person>> listPerson;
    vector<list<Place>> listPlace;
    int pr = 0;
    int pl = 0;
    enum inputT { place = 0, person = 1, invalid = -1 };
public: 
    Graph();
    Person* findPerson(int, string);
    Place* findPlace(int, string);
    void add(string);
    void remove(string);
    void deleteEdge(string, string);
    void newEdge(string, string);
    void showInfo(string);
    void normal(string);
    void confirm(string);
    void clean(string);
    void show(string);
};

Graph::Graph() {
    listPerson.resize(365);
    listPlace.resize(256);
}

Person* Graph::findPerson(int id, string input) {

    int hashVal = id / 10;

    list<Person>::iterator it = listPerson[hashVal].begin();
    while (it != listPerson[hashVal].end() && it->id != input) {
        it++;
    }

    return (it == listPerson[hashVal].end()) ? nullptr : &(*it);
}

Place* Graph::findPlace(int id, string input) {

    int hashVal = id / 10;

    list<Place>::iterator it = listPlace[hashVal].begin();
    while (it != listPlace[hashVal].end() && it->name != input) {
        it++;
    }

    return (it == listPlace[hashVal].end()) ? nullptr : &(*it);
}

void Graph::normal(string input) {
    int id = hashing(input);
    inputT inpType = static_cast<inputT>(id % 10);
    Person* prInp = findPerson(id, input);
    if (inpType == person) {
        if (prInp != nullptr) {
            prInp->stat = "NORMAL";
            cout << "Done!\n";
        }
        else {
            cout << "ID doesn't exist on the system.\n";
        }
    }
    else {
        cout << "Invalid input.\n";
    }
    return;
}

void Graph::show(string input) {
    if (pr > 0 && input == "person") {
        for (auto index : listPerson) {
            for (auto person : index) {
                cout << person.stat;
                (person.stat == "NORMAL") ? cout << "\t\t" : cout << "\t";
                cout << person.id << "\t" << person.name << "\n";
            }
        }
    }
    else if (pl > 0 && input == "place") {
        for (auto index : listPlace) {
            for (auto place : index) {
                cout << place.stat;
                (place.stat == "CLEAN") ? cout << "\t\t" : cout << "\t"; 
                cout << place.name << "\n";
            }
        }
    }
    else if ((input == "person" || input == "place") && (pl <= 0 || pr <= 0)) {
        cout << "The list you requested is empty.\n";
    }
    else {
        cout << "Invalid input.\n";
    }
    return;
}
void Graph::clean(string input) {
    int id = hashing(input);
    inputT inpType = static_cast<inputT>(id % 10);
    Place* plInp = findPlace(id, input);
    if (inpType == place) {
        if (plInp != nullptr) {
            plInp->stat = "CLEAN";
            cout << "Done!\n";
        }
        else {
            cout << "Place doesn't exist on the system.\n";
        }
    }
    else {
        cout << "Invalid input.\n";
    }
}

void Graph::confirm(string input) {
    int id = hashing(input);
    inputT inpType = static_cast<inputT>(id % 10);
    if (inpType == person) {
        Person* prInp = findPerson(id, input);
        if (prInp != nullptr) {
            for(auto toObserve : prInp->edgePerson) {
                Person* prToObserve = findPerson(hashing(toObserve), toObserve);
                if (prToObserve->stat == "NORMAL") prToObserve->stat = "OBSERVED";
            }

            for (auto toInfect : prInp->edgePlace) {
                Place* plToInfect = findPlace(hashing(toInfect), toInfect);
                for (auto toObserve : plToInfect->edgePerson) {
                    Person* prToObserve = findPerson(hashing(toObserve), toObserve);
                    if (prToObserve->id != input && prToObserve->stat == "NORMAL") prToObserve->stat = "OBSERVED";
                }
                if (plToInfect->stat == "CLEAN") plToInfect->stat = "INFECTED";
            }

            prInp->stat = "CONFIRMED";
            cout << "Done!\n";
        }
        else {
            cout << "ID doesn't exist on the system.\n";
        }
    }
    else {
        cout << "Invalid input.\n";
    }
}

void Graph::deleteEdge(string src, string dst) {
    int idSrc = hashing(src);
    int idDst = hashing(dst);
    int hashValSrc = idSrc / 10;
    int hashValDst = idDst / 10;
    inputT inpSrc = static_cast<inputT>(idSrc % 10);
    inputT inpDst = static_cast<inputT>(idDst % 10);
    Person* prSrc = findPerson(idSrc, src);
    if (inpSrc == person && inpDst == person) {
        Person* prDst = findPerson(idDst, dst);
        if (prSrc != nullptr && prDst != nullptr) {
            int found = 0;
            vector<string>::iterator itSrc;
            for(auto itSrc = prSrc->edgePerson.begin(); itSrc != prSrc->edgePerson.end(); itSrc++) {
                if (*itSrc == dst) {
                    found = 1;
                    break;
                }
            }
            vector<string>::iterator itDst;
            for (auto itDst = prDst->edgePerson.begin(); itDst != prDst->edgePerson.end(); itDst++) {
                if (*itDst == src) {
                    break;
                }
            }
            if (found) {
                prSrc->edgePerson.erase(itSrc);
                prDst->edgePerson.erase(itDst);
                cout << "Connection deleted!\n";
            }
            else {
                cout << "No connection found.\n";
            }
            
            return;
        }
        if (prSrc == nullptr) cout << "ID 1 doesn't exist on the system.\n";
        if (prDst == nullptr) cout << "ID 2 doesn't exist on the system.\n";
        return;
    }
    else if (inpSrc == person && inpDst == place) {
        Place* plDst = findPlace(idDst, dst);
        if (prSrc != nullptr && plDst != nullptr) {
            int found = 0;
            vector<string>::iterator itSrc;
            for (auto itSrc = prSrc->edgePlace.begin(); itSrc != prSrc->edgePlace.end(); ++itSrc) {
                if (*itSrc == dst) {
                    found = 1;
                    prSrc->edgePlace.erase(itSrc);
                    break;
                }
            }
            vector<string>::iterator itDst;
            for (auto itDst = plDst->edgePerson.begin(); itDst != plDst->edgePerson.end(); ++itDst) {
                if (*itDst == src) {
                    plDst->edgePerson.erase(itDst);
                    break;
                }
            }
            (found) ? cout << "Connection deleted!\n" : cout << "No connection found.\n";
        }
        if (prSrc == nullptr) cout << "ID doesn't exist on the system.\n";
        if (plDst == nullptr) cout << "Place doesn't exist on the system.\n";
        return;
    }
    else {
        if (inpSrc == invalid || inpSrc == place) cout << "Invalid input 1.\n";
        if (inpDst == invalid) cout << "Invalid input 2.\n";
        return;
    }
}

void Graph::remove(string input) {
    int id = hashing(input);
    int hashVal = id / 10;
    inputT inpType = static_cast<inputT>(id % 10);
    if (inpType == person) {
        Person* prInp = findPerson(id, input);
        if (prInp != nullptr) {
            while (!(prInp->edgePerson.empty())) {
                string removeFrom = prInp->edgePerson.back();
                Person* prRemoveFrom = findPerson(hashing(removeFrom), removeFrom);

                vector<string>::iterator toRemove = prRemoveFrom->edgePerson.begin();
                while (*toRemove != input) {
                    toRemove++;
                }
                prRemoveFrom->edgePerson.erase(toRemove);

                prInp->edgePerson.pop_back();
            }

            while (!(prInp->edgePlace.empty())) {
                string removeFrom = prInp->edgePlace.back();
                Place* plRemoveFrom = findPlace(hashing(removeFrom), removeFrom);

                vector<string>::iterator toRemove = plRemoveFrom->edgePerson.begin();
                while (*toRemove != input) {
                    toRemove++;
                }
                plRemoveFrom->edgePerson.erase(toRemove);

                prInp->edgePlace.pop_back();
            }

            list<Person>::iterator toRemove = listPerson[hashVal].begin();
            while (toRemove->id != input) {
                toRemove++;
            }
            listPerson[hashVal].erase(toRemove);
            pr--;
            cout << "Removed!\n";
        }
        else {
            cout << "ID doesn't exist on the system.\n";
        }
    }
    else if (inpType == place) {
        Place* plInp = findPlace(id, input);
        if (plInp != nullptr) {
            while (!(plInp->edgePerson.empty())) {
                string removeFrom = plInp->edgePerson.back();
                Person* prRemoveFrom = findPerson(hashing(removeFrom), removeFrom);

                vector<string>::iterator toRemove = prRemoveFrom->edgePlace.begin();
                while (*toRemove != input) {
                    toRemove++;
                }
                prRemoveFrom->edgePlace.erase(toRemove);

                plInp->edgePerson.pop_back();
            }

            list<Place>::iterator toRemove = listPlace[hashVal].begin();
            while (toRemove->name != input) {
                toRemove++;
            }
            listPlace[hashVal].erase(toRemove);
            pl--;
            cout << "Removed!\n";
        }
        else {
            cout << "Place doesn't exist on the system.\n";
        }
    }
    else {
        cout << "Invalid input.\n";
    }
    return;
}

void Graph::showInfo(string input) {
    int id = hashing(input);
    int hashVal = id / 10;
    inputT inpType = static_cast<inputT>(id % 10);
    if (inpType == person) {
        Person* prInp = findPerson(id, input);
        if (prInp != nullptr) {

            cout << "ID\t: " << prInp->id << "\n";
            cout << "Name\t: " << prInp->name << "\n";
            cout << "Age\t: " << prInp->age << " years\n";
            cout << "Status\t: " << prInp->stat << "\n";
            cout << "Travel\t: ";
            if (!(prInp->edgePlace.empty())) {
                cout << "\n";
                for (auto it : prInp->edgePlace) {
                    Place* plIt = findPlace(hashing(it), it);
                    cout << "- " << plIt->stat;
                    (plIt->stat == "CLEAN") ? cout << "\t\t" : cout << "\t";
                    cout << plIt->name << "\n";
                }
            }
            else {
                cout << "-\n";
            }
            cout << "Contact\t: ";
            if (!(prInp->edgePerson.empty())) {
                cout << "\n";
                for (auto it : prInp->edgePerson) {
                    Person* prIt = findPerson(hashing(it), it);
                    cout << "- " << prIt->stat;
                    (prIt->stat == "NORMAL") ? cout << "\t\t" : cout << "\t";
                    cout << prIt->id << "\t" << prIt->name << "\n";
                }
            }
            else {
                cout << "-\n";
            }
        }
        else {
            cout << "ID doesn't exist on the system.\n";
        }
    }
    else if (inpType == place) {
        Place* plInp = findPlace(id, input);
        if (plInp != nullptr) {

            cout << "Name\t: " << plInp->name << "\n";
            cout << "Status\t: " << plInp->stat << "\n";
            cout << "Visited\t: ";
            if (!(plInp->edgePerson.empty())) {
                cout << "\n";
                for (auto it : plInp->edgePerson) {
                    Person* prIt = findPerson(hashing(it), it);
                    cout << "- " << prIt->stat;
                    (prIt->stat == "NORMAL") ? cout << "\t\t" : cout << "\t";
                    cout << prIt->id << "\t" << prIt->name << "\n";
                }
            }
            else {
                cout << "-\n";
            }
        }
        else {
            cout << "Place doesn't exist on the system.\n";
        }
    }
    else {
        cout << "Invalid input.\n";
    }
    return;
}

void Graph::add(string input) {
    int id = hashing(input);
    int hashVal = id / 10;
    inputT inpType = static_cast<inputT>(id % 10);
    if (inpType == person) {

        Person temp;
        temp.id = input;
        temp.stat = "NORMAL";

        cout << "Enter name > ";
        getline(cin, temp.name);

        cout << "Enter age  > ";
        cin >> temp.age;
        cin.ignore(1);

        listPerson[hashVal].push_back(temp);
        pr++;
        cout << "Added!\n";
    }
    else if (inpType == place) {
        Place temp;
        temp.name = input;
        temp.stat = "CLEAN";

        listPlace[hashVal].push_back(temp);
        pl++;
        cout << "Added!\n";
    }
    else {
        cout << "Invalid input.\n";
    }
    return;
}

void Graph::newEdge(string src, string dst) {
    int idSrc = hashing(src);
    int idDst = hashing(dst);
    int hashValSrc = idSrc / 10;
    int hashValDst = idDst / 10;
    inputT inpSrc = static_cast<inputT>(idSrc % 10);
    inputT inpDst = static_cast<inputT>(idDst % 10);
    Person* prSrc = findPerson(idSrc, src);
    if (inpSrc == person && inpDst == person) {
        Person* prDst = findPerson(idDst, dst);
        if (prSrc != nullptr && prDst != nullptr) {
            prSrc->edgePerson.push_back(dst);
            prDst->edgePerson.push_back(src);

  
            if (prSrc->stat == "CONFIRMED" && prDst->stat != "CONFIRMED") prDst->stat = "OBSERVED";
            if (prDst->stat == "CONFIRMED" && prSrc->stat != "CONFIRMED") prSrc->stat = "OBSERVED";
            cout << "New connection added!\n";
            return;
        } 
        if (prSrc == nullptr) cout << "ID 1 doesn't exist on the system.\n";
        if (prDst == nullptr) cout << "ID 2 doesn't exist on the system.\n";
        return;
    }
    else if (inpSrc == person && inpDst == place) {
        Place* plDst = findPlace(idDst, dst);
        if (prSrc != nullptr && plDst != nullptr) {
            prSrc->edgePlace.push_back(dst);
            plDst->edgePerson.push_back(src);

            if (plDst->stat == "INFECTED" && prSrc->stat != "CONFIRMED") prSrc->stat = "OBSERVED";
            if (prSrc->stat == "CONFIRMED") plDst->stat = "INFECTED";
            cout << "New connection added!\n";
            return;
        }
        if (prSrc == nullptr) cout << "ID doesn't exist on the system.\n";
        if (plDst == nullptr) cout << "Place doesn't exist on the system.\n";
        return;
    }
    else {
        if (inpSrc == invalid || inpSrc == place) cout << "Invalid input 1.\n";
        if (inpDst == invalid) cout << "Invalid input 2.\n";
        return;
    }
}

int main() {   

    showHelp();
    Graph myCity;
    vector<string> command = getCommand();

    while (command[0] != "exit") {
        
        if (command[0] == "add") { 
            myCity.add(command[1]);       
        }
        else if (command[0] == "remove") {
           myCity.remove(command[1]);
        }
        else if (command[0] == "info") {
            myCity.showInfo(command[1]);
        }
        else if (command[0] == "confirm") {
            myCity.confirm(command[1]);
        } 
        else if (command[0] == "clean") {
            myCity.clean(command[1]);
        }
        else if (command[0] == "normal") {
            myCity.normal(command[1]);
        }
        else if (command[0] == "new") {
            string word;
            int space = 0;
            vector<string> input;
            for(auto x : command[1]) {
                if(x == ' ' && input.size() == 0) {
                    input.push_back(word);
                    word = "";
                    space = 1;
                }
                else {
                    word += x;
                }
            }
            if (space) {
                input.push_back(word);
                myCity.newEdge(input[0], input[1]);
            }
            else {
                cout << "Invalid input.\n";
            }
        }
        else if (command[0] == "delete") {
            string word;
            int space = 0;
            vector<string> input;
            for (auto x : command[1]) {
                if (x == ' ' && input.size() == 0) {
                    input.push_back(word);
                    word = "";
                    space = 1;
                }
                else {
                    word += x;
                }
            }
            if (space) {
                input.push_back(word);
                myCity.deleteEdge(input[0], input[1]);
            }
            else {
                cout << "Invalid input.\n";
            }
        }
        else if (command[0] == "show") {
            myCity.show(command[1]);
        }
        else if (command[0] == "help") {
            showHelp();
        }
        else {
            cout << "Invalid command.\n";
        }
        
        command.clear();
        command = getCommand();
    }
}

bool isAllDigit(string str) {
    return all_of(str.begin(), str.end(), ::isdigit);
}

int digit(int input) {
    int res = 0;

    while (input != 0) {
        input /= 10;
        res++;
    }

    return res;
}

int hashing(string input) {
    unsigned long long int res = 0;

    if (isAllDigit(input) && (int)input.length() == 16) {
        string dob = input.substr(6, 4);
        int mon = stoi(dob) % 100;
        int dat = stoi(dob) / 100;
        bool thirty = false;
        for (int i = 1; i < mon; i++) {
            if (i == 2) {
                res += 29;
                thirty = false;
            }
            else if (i == 9) {
                res += 31;
                thirty = true;
            }
            else {
                (thirty) ? res += 30 : res += 31;
                thirty = !thirty;
            }
        }
        return (res + dat - 1) * 10 + 1;
    }
    else if (!isAllDigit(input)) {
        int p = 0;
        for (auto ch : input) {
            res += pow(7, p) * (unsigned long long int)ch;
            p++;
        }
        return (res % 256) * 10;
    }
    else {
        return -1;
    }
}

vector<string> getCommand() {
    string src;

    cout << "\n>";

    getline(cin, src);

    int space = 0;
    string word;
    vector<string> input;
    for (auto x : src) {
        if (x == ' ' && input.size() == 0) {
            input.push_back(word);
            word = "";
            space = 1;
        }
        else {
            word += x;
        }
    }
    input.push_back(word);
    if(!space) input.push_back("12");
    return input;
}

void showHelp() {
    cout << "add (id/place)";
    cout << "\t\tAdd new person or place to the system\n";

    cout << "remove (id/place)";
    cout << "\tRemove person or place from the system\n";

    cout << "info (id/place)";
    cout << "\t\tShow the person's or place's information\n";

    cout << "new (id) (id/place)";
    cout << "\tAdd new connection from person to person or person to place\n";

    cout << "delete (id) (id/place)";
    cout << "\tDelete connection from person to person or person to place\n";

    cout << "confirm (id)";
    cout << "\t\tConfirm person's infection and trace the connection\n";

    cout << "clean (place)";
    cout << "\t\tDisinfect place. Change place's status to CLEAN\n";

    cout << "normal (id)";
    cout << "\t\tChange person's status to NORMAL\n";

    cout << "show (\"person\"/\"place\")";
    cout << "\tShow list of person/place on the system\n";

    cout << "help";
    cout << "\t\t\tShow help\n";

    cout << "exit";
    cout << "\t\t\tExit the program\n";

    cout << "\n*id* is 16 digit national identification number or KTP number.\n";
    cout << "*place* is place's name with exact wording and case-sensitive.\n";
    return;
}