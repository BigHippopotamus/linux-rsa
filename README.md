This is a modified Linux 5.17.6 kernel with added system calls to implement RSA encryption.

To use the implemented system calls, import the ``<linux/kernel.h>``, ``<sys/syscall.h>``, and ``<linux/rsa.h>`` headers.

To obtain a private and public key pair, use system call 549, as such:
<pre>
    <code>struct rsakey private_key, public_key;
    syscall(549, &private_key);
    public_key = private_key;
    public_key.d = 0;</code>
</pre>
The value of ``e`` is always fixed to 65,537.
The values of the two primes selected will range between the 32nd prime number excluding 2 (equal to 137) and the (2047 + 32)th prime number excluding 2 (equal to 18,133).
These two values are determined by the macros ``RSA_PRIME_OFFSET`` and ``RSA_PRIME_MAX_VAL`` defined in ``include/uapi/linux/rsa.h``.
Very rarely, the two primes may be selected as 18,133 and 18,143.

Data can be encrypted using system call 550, which takes an unsigned 16-bit integer and a public key as input and outputs an unsigned 32-bit integer:
<pre>
    <code>uint16_t data = ...;
    uint32_t encrypted_data;
    encrypted_data = (uint32_t) syscall(550, data, &public_key);
    </code>
</pre>

Data can be decrypted using system call 551, which takes an unsigned 32-bit integer and a private key as input and outputs an unsigned 16-bit integer:
<pre>
    <code>uint32_t data = ...;
    uint16_t decrypted_data;
    decrypted_data = (uint16_t) syscall(551, data, &private_key);
    </code>
</pre>

This project was made purely for educational purposes and will not provide any meaningful level of security.
