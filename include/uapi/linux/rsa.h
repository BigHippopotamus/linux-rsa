#ifndef _UAPI_LINUX_RSA_H
#define _UAPI_LINUX_RSA_H

#define RSA_PRIME_OFFSET 32
#define RSA_PRIME_MAX_VAL 2048

struct rsakey {
	__u32 e;
	__u32 d;
	__u32 n;
};

#endif /* _UAPI_LINUX_RSA_H */
