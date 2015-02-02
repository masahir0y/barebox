#include <malloc.h>
#include <linux/barebox-wrapper.h>

void *kmalloc(size_t size, gfp_t flags)
{
	void *p;

	p = malloc(size);

	if (likely(p)) {
		if (flags & __GFP_ZERO)
			memset(p, 0, size);
	}

	return p;
}
