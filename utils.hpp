#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <string>
#include <map>
#include <vector>
#include <fstream>

using namespace std;

typedef struct user_info {
    map<string, string> resources;
    string token_access;
    string token_refresh;
    int autorefresh;
    int ttl;
} user_info;

extern map<string, int> userIDs;
extern vector<string> resources;
extern vector<string> approvals;
extern map<string, user_info> users;
extern int currentApprovals;
extern string ttl;

extern void read_userIDs(char *filename) ;
extern void read_resources(char *filename);
extern void read_approvals(char *filename);
extern void read_ttl();
#endif
