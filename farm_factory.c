#include "farm.h"
#include <stdint.h>

void actualizar_sens_luz(struct sensor *self) { self->valor = 10; }
void actualizar_sens_agua(struct sensor *self) { self->valor = 20; }
void actualizar_sens_humedad(struct sensor *self) { self->valor = 30; }

struct farm farm1;
struct sensores sensores1;
struct sensor sens_agua, sens_luz, sens_humedad;
struct NPKM npkm1 = {100, 100, 100, 100};

int main(void) {
  init_sensor(&sens_agua, "agua", 0, 0, actualizar_sens_agua);
  init_sensor(&sens_humedad, "humedad", 1, 0, actualizar_sens_humedad);
  init_sensor(&sens_luz, "luz", 2, 0, actualizar_sens_luz);

  sensores1.n_sensores = 3;
  sensores1.actualizar = actualizar_sensores;
  struct sensor array_sensores[3] = {sens_agua, sens_luz, sens_humedad};
  sensores1.array_sensores = array_sensores;

  init_farm(&farm1);
  (*farm1.constructor)(&farm1, "lechuga", &sensores1, &npkm1);

  printf("%s\n", "INICIO");

  print_farm_info(&farm1);

  (*farm1.sensar)(&farm1);

  printf("%s\n", "Despues del sensado");

  print_farm_info(&farm1);

  return 0;
}
