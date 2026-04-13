// Not developed by me, (ai helped), TL;DR: AI made this cuz im tired, ill do later//
// TODO: Make shell script made by **ME** to reproduce the bug //
// The patch and the bug was made by me //
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdint.h>

#define BITS_PER_LONG 64
#define BITS_TO_LONGS(nr)       (((nr) + BITS_PER_LONG - 1) / BITS_PER_LONG)
#define BITMAP_LAST_WORD_MASK(nbits) (~0UL >> (-(nbits) & (BITS_PER_LONG - 1)))

/* Vulnerable function without the clamp */
void __bitmap_shift_right_vulnerable(unsigned long *dst, const unsigned long *src,
			unsigned shift, unsigned nbits)
{
	unsigned k, lim = BITS_TO_LONGS(nbits);
	unsigned off = shift/BITS_PER_LONG, rem = shift % BITS_PER_LONG;
	unsigned long mask = BITMAP_LAST_WORD_MASK(nbits);
	for (k = 0; off + k < lim; ++k) {
		unsigned long upper, lower;

		if (!rem || off + k + 1 >= lim)
			upper = 0;
		else {
			upper = src[off + k + 1];
			if (off + k + 1 == lim - 1)
				upper &= mask;
			upper <<= (BITS_PER_LONG - rem);
		}
		lower = src[off + k];
		if (off + k == lim - 1)
			lower &= mask;
		lower >>= rem;
		dst[k] = lower | upper;
	}
	if (off) {
        printf("  [!] Vulnerability triggered: memset(&dst[%u - %u], 0, %u * 8)\n", lim, off, off);
        printf("  [!] This equals: memset(&dst[%d], 0, %u)\n", (int)lim - (int)off, off * 8);
		memset(&dst[lim - off], 0, off*sizeof(unsigned long));
    }
}

int main() {
    printf("=== Userspace Reproducer: bitmap_shift_right underflow ===\n");
    
    /* 
     * We allocate a 3-word array so we can safely underflow by 1 word
     * without actually segfaulting the OS (we just overwrite our own canary).
     */
    unsigned long *buffer = malloc(3 * sizeof(unsigned long));
    unsigned long *dst = &buffer[1]; // Set dst to the middle element
    unsigned long src = ~0UL;
    
    buffer[0] = 0xDEADBEEFCAFEBABE; // Pre-buffer metadata (canary)
    dst[0] = 0;                     // Actual bitmap
    buffer[2] = 0xDEADBEEFCAFEBABE; // Post-buffer metadata (canary)

    printf("Before shift:\n");
    printf("  Pre-canary (dst[-1]): 0x%lx\n", dst[-1]);
    
    /* Trigger the bug: nbits=64 (lim=1), shift=128 (off=2) */
    printf("\nCalling __bitmap_shift_right_vulnerable(dst, src, shift=128, nbits=64)...\n");
    __bitmap_shift_right_vulnerable(dst, &src, 128, 64);
    
    printf("\nAfter shift:\n");
    printf("  Pre-canary (dst[-1]): 0x%lx\n", dst[-1]);
    
    if (dst[-1] == 0) {
        printf("\n[!!!] FATAL BUG REPRODUCED: The pre-canary was overwritten with zeros.\n");
        printf("[!!!] In the kernel, this silently destroys the slab allocator's freelist pointer, causing a panic.\n");
    } else {
        printf("\n[-] Bug not reproduced.\n");
    }
    
    free(buffer);
    return 0;
}
