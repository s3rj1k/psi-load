#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

guint
read_psi_avg10_kind_some_value(const char *file)
{
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  guint avg = 0;
    
  fp = fopen(file, "r");
  if (fp == NULL) {
    return 0;
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    double x = 0, y = 0;

    int ret = sscanf(line, "some avg10=%lf", &x);
    if (ret > 0) {
      y = round(x + 0.4);
      avg = (guint) y;

      break;
    }
  }

  free(line);
  fclose(fp);

  return avg;
}
