#include <BlynkGO.h>

namespace NixieClock {
  GObject* create();
  void setTime(struct tm &tm);
}
