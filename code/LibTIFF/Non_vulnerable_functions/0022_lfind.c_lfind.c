void *
lfind(const void *key, const void *base, size_t *nmemb, size_t size,
      int(*compar)(const void *, const void *))
{
	char *element, *end;
	end = (char *)base + *nmemb * size;
	for (element = (char *)base; element < end; element += size)
		if (!compar(element, key))		
			return element;
	return NULL;
}
