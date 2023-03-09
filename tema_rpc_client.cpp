#include <stdio.h>
#include <rpc/rpc.h>
#include <fstream>
#include <vector>
#include <map>
#include "tema.h"

#define PROTOCOL "tcp"

using namespace std;
CLIENT *handle;

/*
 * Structure to store the information about the user
*/
typedef struct user_info {
    char *token_access;
    char *token_refresh;
    int ttl;
    int autorefresh;
} user_info;


map<string, user_info> users;

/**
 * @brief 
 * 
 * Function to tokenize a string by a given delimiter
 * 
 * 
 * @param str - the string to be tokenized
 * @param delim - the delimiter(',' is the default)
 * @return a vector of tokens
*/
vector<string> tokenize(string s, string del = ",")
{
    int start, end = -1*del.size();
    vector<string> tokens;

    do {
        start = end + del.size();
        end = s.find(del, start);
        tokens.push_back(s.substr(start, end - start));
    } while (end != -1);

    return tokens;
}

/**
 * @brief 
 * 
 * Function to request authorization from the server
 * Function calls the first RPC procedure from the server to the get the request
 * token. If the value is USER_NOT_FOUND, the user is not registered, function
 * prints the message and exits.
 * If the user is registered, the function calls the second RPC procedure to get
 * the permissions from the user.
 * The function calls the third RPC procedure to get the access token. If the
 * user gave no permissions, the request is denied and the function prints the
 * message and exits. Otherwise, the function stores the access token, the
 * refresh if it exists and the ttl. The function also prints the request,
 * access and refresh(if any) tokens.
 * 
 * 
 * @param user - the user for which the authorization is requested
 * @param autorefresh  - the autorefresh flag
 */

void request(string user, int autorefresh) {
    struct request_info request;
    struct request_response *response;
    user_info info;
    request.token_req = strdup("");
    request.userid = (char *) strdup(user.c_str());
    request.autorefresh = autorefresh;

    response = request_authorization_1(&request, handle);

    if (!strcmp(response->token_req, "USER_NOT_FOUND")) {
        printf("USER_NOT_FOUND\n");
        fflush(stdout);

        return;
    }

    request.token_req = strdup(response->token_req);
    response = approve_request_token_1(&request, handle);

    response = request_access_token_1(&request, handle);

    if (!strcmp(response->token_access,"REQUEST_DENIED")) {
        printf("REQUEST_DENIED\n");
        fflush(stdout);

        return;
    }

    info.token_access = strdup(response->token_access);
    info.ttl = atoi(response->token_req);
    info.token_refresh = strdup(response->token_refresh);
    info.autorefresh = autorefresh;

    users[user] = info;

    if (autorefresh) {
        printf("%s -> %s,%s\n", request.token_req, users[user].token_access, users[user].token_refresh);
    } else {
        printf("%s -> %s\n", request.token_req, users[user].token_access);
    }

    fflush(stdout);
}

/**
 * @brief 
 * 
 * Function that validates an operation done on a resource by a user.
 * First, it checks if the user is currently registered anf has any saved value
 * for the access token. If not, the default value(empty string) is used.
 * If the user is registered, it checks if the access token has the autorefresh
 * function activated. If it is, it checks if the TTL has reached 0. If it has,
 * it calls another RPC procedure to refresh the access token. It updates the
 * TTL, access and refresh tokens and sets the acces token for the validation
 * procedure to the new value.
 * It continues with the validation procedure. It calls the validation procedure
 * using the correct arguments(acces token, resource and operation).
 * After getting the respinse, it prints the message and exits. If the permission
 * was not denied, it means the request is valid and the TTL for the tokens is
 * updated.
 * 
 * 
 * @param user - the user from which action is requested
 * @param op  - the operation wanted to be performed
 * @param resource - the resource on which the operation is performed
 */

void validate(string user, string op, string resource) {
    struct user_info info;
    struct validate_info request;
    struct validate_response *response;

    if (users.find(user) == users.end()) {
        request.token_access = strdup("");
    } else {
        info = users[user];

        if (info.autorefresh) {
            if (info.ttl <= 0) {
                struct validate_info request_refresh;
                struct validate_response *response_refresh;
                request_refresh.token_access = strdup(info.token_refresh);
                request_refresh.resource = strdup("");
                request_refresh.op = strdup("");

                response_refresh = request_refresh_token_1(&request_refresh, handle);

                users[user].token_access = strdup(response_refresh->token_access);
                users[user].token_refresh = strdup(response_refresh->token_refresh);
                users[user].ttl = response_refresh->ttl;

                info = users[user];
            }
        }

        request.token_access = strdup(info.token_access);
    }

    request.op = strdup(op.c_str());

    request.resource = (char *)resource.c_str();

    response = validate_delegated_action_1(&request, handle);

    printf("%s\n", response->message);
    fflush(stdout);


    if (strcmp(response->message, "PERMISSION_DENIED")) {
        if (users.find(user) != users.end()) {
            users[user].ttl--;
        }
    }
}

/**
 * @brief 
 * 
 * Main function. It reads the input from the file given as argument and calls
 * the correct procedures acording to the input.
 * 
 */

int main(int argc, char const *argv[])
{
	int *sum;

	if (argc != 3) {
		fprintf(stderr, "Usage:\n\t%s <server addr> client.in\n",
			argv[0]);
		return -1;
	}

	handle = clnt_create(argv[1], OAUTH_PROG, OAUTH_VERS, PROTOCOL);
	if (!handle) {
		perror("Failed to create client handle");
		clnt_pcreateerror(argv[0]);
		return -2;
	}

    fstream fin;

    fin.open(argv[2], ios::in);

    if (!fin.is_open()) {
        fprintf(stderr, "Error opening file %s\n", argv[1]);;
        return -1;
    }

    while(!fin.eof()) {
        string command;
        vector<string> tokens;

        fin >> command;

        tokens = tokenize(command);

        if (tokens[1] == "REQUEST") {
            request(tokens[0], atoi(tokens[2].c_str()));
        } else {
            validate(tokens[0], tokens[1], tokens[2]);
        }
    }

    fin.close();

	clnt_destroy(handle);
	xdr_free((xdrproc_t)xdr_int, (char *)sum);

	return 0;
}
