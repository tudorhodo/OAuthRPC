/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _TEMA_H_RPCGEN
#define _TEMA_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct request_info {
	char *token_req;
	char *userid;
	int autorefresh;
};
typedef struct request_info request_info;

struct request_response {
	char *token_req;
	char *token_access;
	char *token_refresh;
};
typedef struct request_response request_response;

struct validate_info {
	char *token_access;
	char *op;
	char *resource;
};
typedef struct validate_info validate_info;

struct validate_response {
	char *token_access;
	char *token_refresh;
	char *message;
	int ttl;
};
typedef struct validate_response validate_response;

#define OAUTH_PROG 1
#define OAUTH_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define request_authorization 1
extern  struct request_response * request_authorization_1(request_info *, CLIENT *);
extern  struct request_response * request_authorization_1_svc(request_info *, struct svc_req *);
#define approve_request_token 2
extern  struct request_response * approve_request_token_1(request_info *, CLIENT *);
extern  struct request_response * approve_request_token_1_svc(request_info *, struct svc_req *);
#define request_access_token 3
extern  struct request_response * request_access_token_1(request_info *, CLIENT *);
extern  struct request_response * request_access_token_1_svc(request_info *, struct svc_req *);
#define validate_delegated_action 4
extern  struct validate_response * validate_delegated_action_1(validate_info *, CLIENT *);
extern  struct validate_response * validate_delegated_action_1_svc(validate_info *, struct svc_req *);
#define request_refresh_token 5
extern  struct validate_response * request_refresh_token_1(validate_info *, CLIENT *);
extern  struct validate_response * request_refresh_token_1_svc(validate_info *, struct svc_req *);
extern int oauth_prog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define request_authorization 1
extern  struct request_response * request_authorization_1();
extern  struct request_response * request_authorization_1_svc();
#define approve_request_token 2
extern  struct request_response * approve_request_token_1();
extern  struct request_response * approve_request_token_1_svc();
#define request_access_token 3
extern  struct request_response * request_access_token_1();
extern  struct request_response * request_access_token_1_svc();
#define validate_delegated_action 4
extern  struct validate_response * validate_delegated_action_1();
extern  struct validate_response * validate_delegated_action_1_svc();
#define request_refresh_token 5
extern  struct validate_response * request_refresh_token_1();
extern  struct validate_response * request_refresh_token_1_svc();
extern int oauth_prog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_request_info (XDR *, request_info*);
extern  bool_t xdr_request_response (XDR *, request_response*);
extern  bool_t xdr_validate_info (XDR *, validate_info*);
extern  bool_t xdr_validate_response (XDR *, validate_response*);

#else /* K&R C */
extern bool_t xdr_request_info ();
extern bool_t xdr_request_response ();
extern bool_t xdr_validate_info ();
extern bool_t xdr_validate_response ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TEMA_H_RPCGEN */
