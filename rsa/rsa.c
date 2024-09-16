#include <linux/kernel.h>
#include <linux/random.h>
#include <linux/rsa.h>
#include <linux/syscalls.h>
#include <linux/types.h>

u8 is_prime(int n) {
	int i;
    for (i = 2; i*i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

u32 ex_euler_algo(u32 a, u32 m) {
    u32 m_original = m;
    s32 x = 1, y = 0;
    while (a > 1) {
        int temp = m;
        u32 q = a / m;

        m = a % m;
        a = temp;
        temp = y;

        y = x - q*y;
        x = temp;
    }

    if (x < 0) x += m_original;

    return x;
}

u32 mod_mul(u32 a, u32 b, u32 m) {
    u32 res = 0;
    a %= m;
    while (b > 0) {
        if (b & 1) res = (res + a) % m;

        b >>= 1;
        a = (a << 1) % m;
    }

    return res;
}

u32 mod_pow(u32 a, u32 b, u32 m) {
    u32 res = 1;
    a %= m;
    if (a == 0) return 0;

    while (b > 0) {
        if (b & 1) res = mod_mul(res, a, m);

        b >>= 1;
        a = mod_mul(a, a, m);
    }

    return res;
}

int get_pki(struct rsakey *key) {
	s16 a, b;
	u32 p, q;
	u16 prime;
	u16 i;
	u32 phi;
    get_random_bytes(&a, sizeof(a));
    get_random_bytes(&b, sizeof(b));
	a %= RSA_PRIME_MAX_VAL;
	a += (a < 0)*RSA_PRIME_MAX_VAL + RSA_PRIME_OFFSET;
	b %= RSA_PRIME_MAX_VAL;
	b += (b < 0)*RSA_PRIME_MAX_VAL + RSA_PRIME_OFFSET;
    if (a == b) b++;
    if (a > b) {
        a += b;
        b = a - b;
        a = a - b;
    }

    prime = 1;
    for (i = 0; i <= b; i++) {
        do {
            prime++;
        } while (!is_prime(prime));
        if (i == a) p = prime;
    }
    q = prime;

    key->n = p * q;
	phi = (p - 1) * (q - 1);

    key->e = 65537;
    key->d = ex_euler_algo(key->e, phi);

	return 0;
}

u32 encrypt(u16 m, struct rsakey *public_key) {
    return mod_pow(m, public_key->e, public_key->n);
}

u16 decrypt(u32 c, struct rsakey *private_key) {
    return (u16)mod_pow(c, private_key->d, private_key->n);
}

SYSCALL_DEFINE1(getkey, struct rsakey *, key)
{
	printk("Creating Key\n");
	return get_pki(key);
}

SYSCALL_DEFINE2(encrypt_rsa, u16, message, struct rsakey *, public_key)
{
	printk("Encrypting Message\n");
	return encrypt(message, public_key);
}

SYSCALL_DEFINE2(decrypt_rsa, u32, cipher, struct rsakey *, private_key)
{
	printk("Decrypting Message\n");
	return decrypt(cipher, private_key);
}
