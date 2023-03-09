/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "tema.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

struct request_response *
request_authorization_1(request_info *argp, CLIENT *clnt)
{
	static struct request_response clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, request_authorization,
		(xdrproc_t) xdr_request_info, (caddr_t) argp,
		(xdrproc_t) xdr_request_response, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct request_response *
approve_request_token_1(request_info *argp, CLIENT *clnt)
{
	static struct request_response clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, approve_request_token,
		(xdrproc_t) xdr_request_info, (caddr_t) argp,
		(xdrproc_t) xdr_request_response, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct request_response *
request_access_token_1(request_info *argp, CLIENT *clnt)
{
	static struct request_response clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, request_access_token,
		(xdrproc_t) xdr_request_info, (caddr_t) argp,
		(xdrproc_t) xdr_request_response, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct validate_response *
validate_delegated_action_1(validate_info *argp, CLIENT *clnt)
{
	static struct validate_response clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, validate_delegated_action,
		(xdrproc_t) xdr_validate_info, (caddr_t) argp,
		(xdrproc_t) xdr_validate_response, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

struct validate_response *
request_refresh_token_1(validate_info *argp, CLIENT *clnt)
{
	static struct validate_response clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, request_refresh_token,
		(xdrproc_t) xdr_validate_info, (caddr_t) argp,
		(xdrproc_t) xdr_validate_response, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
