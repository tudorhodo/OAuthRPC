#include <rpc/rpc.h>
#include <stdio.h>

#include "tema.h"
#include "token.h"
#include "utils.hpp"
#include <iostream>

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
 * Function to request authorization from the server. It checks if the user is
 * registered in the database. If the user is registered, it calls the function
 * given in the skel to generate the request token. Otherwise it return an error
 * message. Finally, it prints the required message(begin <userId> authorization).
 * 
 * 
 * @param request - the request structure which contains the user id
 * @return The function returns the request token or a message if the user is
 *          not registered in the response structure.
 */
struct request_response * request_authorization_1_svc(request_info *request, struct svc_req *){
    struct request_response *response = (struct request_response *) malloc(sizeof(struct request_response));
    string user = request->userid;
    response->token_access  = strdup("");
    response->token_refresh  = strdup("");

    printf("BEGIN %s AUTHZ\n", request->userid);
    fflush(stdout);

    if (userIDs.find(string(request->userid)) == userIDs.end()) {
        response->token_req = strdup("USER_NOT_FOUND");
        return response;
    }

    response->token_req = (char*)generate_access_token(request->userid);

    return response;
}


/**
 * @brief 
 * 
 * Function to get the permissions from the user. It symbolically "signs" the
 * request token. It increments the index in the approval vector to simulate the
 * read of users input. It prints the required message(request token = <token>).
 * 
 * 
 * @param request - the request structure which contains the request token
 * @return The function returns the request token in the response structure.
 */
struct request_response * approve_request_token_1_svc(request_info *request, struct svc_req *) {
    struct request_response *response = (struct request_response *) malloc(sizeof(struct request_response));
    response->token_access  = strdup("");
    response->token_refresh  = strdup("");
    response->token_req = strdup(request->token_req);

    currentApprovals++;

    printf("  RequestToken = %s\n", request->token_req);
    fflush(stdout);

    return response;
}

/**
 * @brief 
 * 
 * Function to get the access token.
 * It starts by updating the permissions for this user by inserting(or updating)
 * the informations about the user in the database(tokens, autorefresh,
 * permissions, etc).
 * It checks if the user gave any permission. If it didn't(known by checking for
 * any '*' entry in the approvals line, meaning there is no permission for any
 * resource), the request is denied, the function return an error message.
 * If the user gave permissions, the function generates the access token and, if
 * required, the refresh token. It stores them in the database and in the response
 * structure and prints them.
 * 
 * 
 * @param request - the request structure, itcontains the request token and user id
 * @return The function returns the acces, refresh(if any) tokens and the TTl
 *          in the response structure  
 */
struct request_response * request_access_token_1_svc(request_info *request, struct svc_req *) {
    struct request_response *response = (struct request_response *) malloc(sizeof(struct request_response));
    map<string, string> resources;

    response->token_req  = strdup(ttl.c_str());

    if (users.find(request->userid) != users.end()) {
        vector<string> tokens = tokenize(approvals[currentApprovals]);
        map<string, string> resources;

        for (int i = 0; i < tokens.size(); i += 2) {
            resources[tokens[i]] = tokens[i + 1];
        }

        users[request->userid].resources = resources;
        users[request->userid].autorefresh = request->autorefresh;
        users[request->userid].ttl = atoi(ttl.c_str());

    } else {
        user_info info;
        vector<string> tokens = tokenize(approvals[currentApprovals]);
        map<string, string> resources;

        info.token_access = "";
        info.token_refresh = "";
        info.autorefresh = request->autorefresh;

        for (int i = 0; i < tokens.size(); i += 2) {
            resources[tokens[i]] = tokens[i + 1];
        }

        info.resources = resources;
        info.ttl = atoi(ttl.c_str());

        users[request->userid] = info;
    }

    resources = users[request->userid].resources;

    if (resources.find("*") != resources.end()) {
        response->token_access = strdup("REQUEST_DENIED");
        response->token_refresh = strdup("");

        return response;
    } else {
        users[request->userid].token_access = (char*)generate_access_token(request->token_req);
        response->token_access = strdup(users[request->userid].token_access.c_str());

        if (users[request->userid].autorefresh) {
            users[request->userid].token_refresh = (char*)generate_access_token(response->token_access);
            response->token_refresh = strdup(users[request->userid].token_refresh.c_str());
        } else {
            response->token_refresh = strdup("");
        }
    }

    printf("  AccessToken = %s\n", response->token_access);

    if (users[request->userid].autorefresh) {
        printf("  RefreshToken = %s\n", response->token_refresh);
    }

    fflush(stdout);

    return response;
}

/**
 * @brief
 * 
 * Function to validate operation. First, it transforms the operation into an
 * easier to use format(one letter, as stored in the db). Then, it searches for
 * user using the access token. If it doesn't find the user, it returns an error
 * and prints a message.
 * If the user is found, it does the verifications specified in the assignment:
 * 1. Check if the token is still valid
 * 2. Checks if the resource exists
 * 3. Check if the user has the required permission(if it doesn't have the
 * required permission or no permission on that resource)
 * If it passes all the verifications, it returns a message and prints a message.
 * Otherwise, it returns an error and prints a message. For any case but the user
 * not being found, the TTl is updated.
 * 
 * 
 * @param validate - the validation structure, it contains the access token, the
 *                  operation that needs to be checked and the resource on which
 *                  the operation is performed
 * @return The function returns the acces, refresh(if any) tokens and the TTl
 *          in the response structure
 */
struct validate_response * validate_delegated_action_1_svc(validate_info *validate, struct svc_req *) {
    struct validate_response *response = (struct validate_response *) malloc(sizeof(struct validate_response));
    char operation;
    int foundResource = 0;
    response->ttl = 0;
    response->token_access = strdup("");
    response->token_refresh = strdup("");

    if (!strcmp(validate->op,"READ")) {
        operation = 'R';
    } else if (!strcmp(validate->op,"INSERT")) {
        operation = 'I';
    } else if (!strcmp(validate->op,"DELETE")) {
        operation = 'D';
    } else if (!strcmp(validate->op,"MODIFY")) {
        operation = 'M';
    } else if (!strcmp(validate->op,"EXECUTE")) {
        operation = 'X';
    } else {
        operation = ' ';
    }

    for (auto it = users.begin(); it != users.end(); it++) {
        if (it->second.token_access == validate->token_access) {

            if (it->second.ttl < 1) {
                printf("DENY (%s,%s,%s,%d)\n", validate->op, validate->resource, "", 0);
                fflush(stdout);
                response->message = strdup("TOKEN_EXPIRED");
                return response;
            }

            for (int i = 0; i < resources.size(); i++) {
                if (validate->resource == resources[i]) {
                    foundResource = 1;
                    break;
                }
            }

            if (!foundResource) {
                it->second.ttl--;
                printf("DENY (%s,%s,%s,%d)\n", validate->op, validate->resource, validate->token_access, it->second.ttl);
                fflush(stdout);

                response->message = strdup("RESOURCE_NOT_FOUND");
                return response;
            }

            if (it->second.resources.find(validate->resource) != it->second.resources.end()) {
                const char *permission = (it->second.resources[validate->resource]).c_str();

                if (!strchr(permission, operation)) {
                    it->second.ttl--;
                    printf("DENY (%s,%s,%s,%d)\n", validate->op, validate->resource, validate->token_access, it->second.ttl);
                    fflush(stdout);
                    response->message = strdup("OPERATION_NOT_PERMITTED");
                    return response;
                }
            } else {
                it->second.ttl--;
                printf("DENY (%s,%s,%s,%d)\n", validate->op, validate->resource, validate->token_access, it->second.ttl);
                fflush(stdout);
                response->message = strdup("OPERATION_NOT_PERMITTED");
                return response;
            }

            it->second.ttl--;

            printf("PERMIT (%s,%s,%s,%d)\n", validate->op, validate->resource, validate->token_access, it->second.ttl);
            fflush(stdout);

            response->message = strdup("PERMISSION_GRANTED");
            return response;
        }
    }

    printf("DENY (%s,%s,%s,%d)\n", validate->op, validate->resource, validate->token_access, 0);
    fflush(stdout);
    response->message = strdup("PERMISSION_DENIED");
    return response;
}

/**
 * @brief
 * 
 * This function is used to refresh the access token. It searches for the user
 * and when it finds it, it generates a new access token and refresh token. It
 * also updates the TTl. It stores the new values in the datebase, prints the
 * new values and return them in the special structure.
 * 
 * 
 * @param validate - the validation structure, it contains the refresh token
 * @return The updated access and refresh tokens and the TTL
 */
struct validate_response * request_refresh_token_1_svc(validate_info *validate, struct svc_req *) {
    struct validate_response *response = (struct validate_response *) malloc(sizeof(struct validate_response));
    response->ttl = atoi(ttl.c_str());
    response->message = strdup("");

    for (auto it = users.begin(); it != users.end(); it++) {
        if (!strcmp(it->second.token_refresh.c_str(), validate->token_access)) {
            response->token_access = (char*)generate_access_token(validate->token_access);
            it->second.token_access = strdup(response->token_access);
            response->token_refresh = strdup((char*)generate_access_token(response->token_access));
            it->second.token_refresh = strdup(response->token_refresh);
            it->second.ttl = response->ttl;

            printf("BEGIN %s AUTHZ REFRESH\n", it->first.c_str());
            printf("  AccessToken = %s\n", response->token_access);
            printf("  RefreshToken = %s\n", response->token_refresh);
            fflush(stdout);

            return response;
        }
    }

    return NULL;
}
