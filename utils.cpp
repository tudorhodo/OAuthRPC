#include "utils.hpp"

map <string, int> userIDs;
vector<string> resources;
vector<string> approvals;
map<string, user_info> users;
int currentApprovals = -1;
string ttl;

/**
 * @brief
 * 
 * Function to read and store the userIds in the database from a given file
 * 
 * 
 * @param filename - input file
 */
void read_userIDs(char *filename) {
    fstream fin;
    int n;
    fin.open(filename, ios::in);
 
    if (!fin.is_open()) {
        fprintf(stderr, "Error opening file %s\n", filename);;
        exit(1);
    }

    fin >> n;

    for (int i = 0; i < n; i++) {
        string name;
        fin >> name;
        userIDs[name] = 0;
    }

    fin.close();
}

/**
 * @brief
 * 
 * Function to read and store the resources in the database from a given file
 * 
 * 
 * @param filename - input file
 */
void read_resources(char *filename) {
    fstream fin;
    int n;
    fin.open(filename, ios::in);
 
    if (!fin.is_open()) {
        fprintf(stderr, "Error opening file %s\n", filename);;
        exit(1);
    }

    fin >> n;

    for (int i = 0; i < n; i++) {
        string resource;
        fin >> resource;
        resources.push_back(resource);
    }

    fin.close();
}

/**
 * @brief
 * 
 * Function to read and store the approvals in the database from a given file
 * 
 * 
 * @param filename - input file
 */
void read_approvals(char *filename) {
    fstream fin;
    fin.open(filename, ios::in);
 
    if (!fin.is_open()) {
        fprintf(stderr, "Error opening file %s\n", filename);;
        exit(1);
    }

    while (!fin.eof()) {
        string permissions;
        fin >> permissions;
        approvals.push_back(permissions);
    }

    fin.close();
}

/**
 * @brief Function that stores the TTL value for the tokens. It reads them from
 * a file or it uses the default value of 1.
 * 
 */
void read_ttl() {
    fstream fin;
    string input;
    fin.open("readme", ios::in);
 
    if (!fin.is_open()) {
        ttl = "1";
    } else {
        while (!fin.eof()) {
            fin >> input;
        }

        ttl = input;

        fin.close();
    }
}
