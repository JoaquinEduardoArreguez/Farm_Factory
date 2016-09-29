#include <stdint.h>

/* Struct que almacena los niveles de nitrogeno, fósoforo , potasio y
   micromoléculas varias para un cultivo determinado.  */
struct NPKM {
  uint8_t n, p, k, m;
};

/*  Struct para valores de sensores de niveles de agua y luz. */
struct sensor {
  uint8_t sens_agua, pin_sens_agua, sens_luz, pin_sens_luz;
}

/*  Información básica para un cultivo determinado. */
struct farm {
  uint8_t *nombre_cultivo;
  struct NPKM *mezcla;
  struct sensor *sensor;
  void (*constructor)(uint8_t *nombre_cultivo, uint8_t nivel_luz,
                      struct NPKM *mezcla);
  void (*sensar)(struct sensor *sensor);
};

/*  Función constructora de farms.  */
void constructor(struct farm *self, uint8_t *nombre_cultivo,
                 struct sensor *sensor, struct NPKM *mezcla) {
  self->nombre_cultivo = nombre_cultivo;
  self->sensor = sensor;
  self->mezcla = mezcla;
}

/*  Dado un conjunto sensor determinado, actualiza sus datos. */
void sensar(struct sensor *sensor) {
  sensor->sens_agua = get_sens_agua(self->pin_sens_agua);
  sensor->sens_luz = get_sens_luz(self->pin_sens_luz);
}

void main(void) {}
