#ifndef LOGS_H
#define LOGS_H

#include <stdint.h>
#include <stdlib.h>

#if defined(GENERIC_LINUX_TARGET)

#include <stdio.h>

#define LOG(msg) printf(msg)
#define LOG_VARIABLE(msg, x) printf(msg); printf("%ld", x)

#elif defined(RTEMS6_TARGET)

#include <rtems.h>
#include <rtems/bspIo.h>

#define LOG(msg) rtems_log(msg)
#define LOG_VARIABLE(msg, x) rtems_log_variable(msg, x);

static void rtems_log(const char *msg)
{
   while (*msg != '\0') {
      rtems_putc(*(msg++));
   }
}

#define MAX_DIGITS 64

static char* ulltoa(uint64_t value, char* str, int base) {
  char buffer[MAX_DIGITS];
  int i = 0;

  if (value == 0) {
    buffer[i++] = '0';
  } else {
    while (value > 0) {
      int digit = value % base;
      buffer[i++] = (digit < 10) ? '0' + digit : 'A' + (digit - 10);
      value /= base;
    }
  }

  int j = 0;
  while (i > 0) {
    str[j++] = buffer[--i];
  }
  str[j] = '\0';

  return str;
}

static void rtems_log_variable(const char *msg, uint64_t x)
{
   rtems_log(msg);
   char str[MAX_DIGITS];
   rtems_log(ulltoa(x, str, 10));
}

#else

#define LOG(msg)
#define LOG_VARIABLE(msg, x)

#endif



#define LOG_VARIABLE_ENDL(msg, x) LOG_VARIABLE(msg, x); LOG("\n")

#endif
