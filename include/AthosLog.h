#define ATH_LOG

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//*************LOGGING***********//
void log_format(const char *tag, const char *message, va_list args)
{
  time_t now;
  time(&now);
  char *date = ctime(&now);
  date[strlen(date) - 1] = '\0';
  Serial.printf("%s [%s] ", date, tag);
  Serial.printf(message, args);
  Serial.printf("\n");
}

void log_error(const char *message, ...)
{
#if defined(ATH_LOG_VERBOSE) || defined(ATH_LOG_ERROR)
  va_list args;
  va_start(args, message);
  log_format("error", message, args);
  va_end(args);
#endif
}

void log_info(const char *message, ...)
{
#if defined(ATH_LOG_VERBOSE) || defined(ATH_LOG_INFO)
  va_list args;
  va_start(args, message);
  log_format("info", message, args);
  va_end(args);
#endif
}

void log_debug(const char *message, ...)
{
#if defined(ATH_LOG_VERBOSE) || defined(ATH_LOG_DEBUG)
  va_list args;
  va_start(args, message);
  log_format("debug", message, args);
  va_end(args);
#endif
}