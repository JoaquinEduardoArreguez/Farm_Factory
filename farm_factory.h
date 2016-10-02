#include <stdint.h>

#pragma once

/* Struct que almacena los niveles de nitrogeno, fósoforo , potasio y
   micromoléculas varias para un cultivo determinado.  */
struct NPKM {
  uint8_t n, p, k, m;
};

/*  Struct que maneja todos los sensores de una farm. */
struct sensores {
  uint8_t n_sensores; // Hasta 255 sensores. Ésta variable almacena la cantidad
                      // de sensores.
  void (*actualizar)(struct sensores *self);
  struct sensor *array_sensores;
};

/*  Struct Para definir sensores y la forma en que se actualizan los mismos. */
struct sensor {
  char *nombre_sensor;
  uint8_t pin, valor;
  void (*actualizar)(struct sensor *self);
  void (*constructor)(struct sensor *, uint8_t, uint8_t,
                      void (*actualizar)(struct sensor *));
};

/*  Información básica para un cultivo determinado. */
struct farm {
  char *nombre_cultivo;
  struct NPKM *mezcla;
  struct sensores *sensores;

  void (*sensar)(struct farm *self);

  void (*constructor)(struct farm *self, char *nombre_cultivo,
                      struct sensores *sensores, struct NPKM *mezcla);
};

/*  Función constructora de farms.  */
void construir_farm(struct farm *self, char *nombre_cultivo,
                    struct sensores *sensores, struct NPKM *mezcla) {
  self->nombre_cultivo = nombre_cultivo;
  self->sensores = sensores;
  self->mezcla = mezcla;
}

void construir_sens(struct sensor *self, uint8_t pin, uint8_t valor,
                    void (*actualizar)(struct sensor *)) {
  self->pin = pin;
  self->valor = valor;
  self->actualizar = actualizar;
}

/*  Actualiza todos los sensores de una "Struct farm". */
void sensar(struct farm *self) { self->sensores->actualizar(self->sensores); }

/*  Actualiza todos los sensores de un "Struct sensores".  */
void actualizar_sensores(struct sensores *self) {
  for (int i = 0; i < self->n_sensores; i++) {
    ((self->array_sensores)[i]).actualizar(&(self->array_sensores)[i]);
  }
}

void init_farm(struct farm *self) {
  self->constructor = construir_farm;
  self->sensar = sensar;
}

void init_sensor(struct sensor *self, char *nombre_sensor, uint8_t pin,
                 uint8_t valor, void (*actualizar)(struct sensor *)) {
  self->nombre_sensor = nombre_sensor;
  self->constructor = construir_sens;
  (*self->constructor)(self, pin, valor, actualizar);
}
