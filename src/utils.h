#ifndef _LOCAL_UTILS
#define _LOCAL_UTILS

/* itoa() is not a standard function, and we cannot safely call printf()
 * after suspending threads. So, we just implement our own copy. A
 * recursive approach is the easiest here.
 */
static char *local_itoa(char *buf, int i) {
  if (i < 0) {
    *buf++ = '-';
    return local_itoa(buf, -i);
  } else {
    if (i >= 10) buf = local_itoa(buf, i / 10);
    *buf++ = (i % 10) + '0';
    *buf = '\000';
    return buf;
  }
}

/* Local substitute for the atoi() function, which is not necessarily safe
 * to call once threads are suspended (depending on whether libc looks up
 * locale information,  when executing atoi()).
 */
static int local_atoi(const char *s) {
  int n = 0;
  int neg = *s == '-';
  if (neg) s++;
  while (*s >= '0' && *s <= '9') n = 10 * n + (*s++ - '0');
  return neg ? -n : n;
}

/* Local substitute for strcat */
static void local_strcat(char *dst, const char *src)
{
  while(*dst != '\0') {
    dst++;
  }

  while(*src != '\0') {
    *dst++ = *src++;
  }
  *dst = '\0';
}



#endif /* _LOCAL_UTILS */
