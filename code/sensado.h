#include <stdint.h>
#pragma once

/*  Struct Para definir sensores y la forma en que se actualizan los mismos. */
struct sensor {
  char *nombre_sensor;
  uint8_t pin, valor;
  void (*actualizar)(struct sensor *);
  void (*constructor)(struct sensor *, uint8_t, uint8_t,
                      void (*actualizar)(struct sensor *));
};

/*  Constructor de sensores, nunca debe usarse directamente.  */
void construir_sens(struct sensor *self, uint8_t pin, uint8_t valor,
                    void (*actualizar)(struct sensor *)) {
  self->pin = pin;
  self->valor = valor;
  self->actualizar = actualizar;
}

/*  Función que inicializa un sensor. */
void init_sensor(struct sensor *self, char *nombre_sensor, uint8_t pin,
                 uint8_t valor, void (*actualizar)(struct sensor *)) {
  self->nombre_sensor = nombre_sensor;
  self->constructor = construir_sens;
  (*self->constructor)(self, pin, valor, actualizar);
}

/*  Struct que maneja todos los sensores de una farm. */
struct sensores {
  uint8_t n_sensores; // Hasta 255 sensores. Ésta variable almacena la cantidad
                      // de sensores.
  void (*actualizar)(struct sensores *);
  struct sensor *array_sensores;
};

/*  Actualiza todos los sensores de un "Struct sensores".  */
void actualizar_sensores(struct sensores *self) {
  for (int i = 0; i < self->n_sensores; i++) {
    ((self->array_sensores)[i]).actualizar(&(self->array_sensores)[i]);
  }
}
