struct request_info {
    string token_req<>;
    string userid<>;
    int autorefresh;
};

struct request_response {
    string token_req<>;
    string token_access<>;
    string token_refresh<>;
};

struct validate_info {
    string token_access<>;
    string op<>;
    string resource<>;
};

struct validate_response {
    string token_access<>;
    string token_refresh<>;
    string message<>;
    int ttl;
};

program OAUTH_PROG {
    version OAUTH_VERS {
        struct request_response request_authorization(request_info req) = 1;
        struct request_response approve_request_token(request_info req) = 2;
        struct request_response request_access_token(request_info req) = 3;
        struct validate_response validate_delegated_action(validate_info req) = 4;
        struct validate_response request_refresh_token(validate_info req) = 5;
    } = 1;
} = 1;
