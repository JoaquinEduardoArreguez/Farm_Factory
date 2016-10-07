#include "actuadores.h"
#include "sensado.h"
#include <stdint.h>
#include <stdio.h>

#pragma once

/* Struct que almacena los niveles de nitrogeno, fósoforo , potasio y
   micromoléculas varias para un cultivo determinado.  */
struct NPKM {
  uint8_t n, p, k, m;
};

/*  Información básica para un cultivo determinado.

    Todos los sensores se pueden tomar al mismo tiempo cada intervalos
    de X segundos, por eso están agrupados dentro de otra estructura "sensores",
    porque pueden manejarse en conjunto.

    Leer la descripción en "actuadores.h".
    */
struct farm {
  char *nombre_cultivo;
  struct NPKM *mezcla;

  struct sensores *sensores;
  void (*sensar)(struct farm *self);

  struct actuadores *actuadores;
  void (*actuar)(struct farm *, enum tipo_actuador,
                 void (*accion_actuador)(struct actuador *));

  void (*constructor)(struct farm *, char *, struct sensores *, struct NPKM *);
};

/*  Constructor de farms, nunca debe usarse directamente.  */
void construir_farm(struct farm *self, char *nombre_cultivo,
                    struct sensores *sensores, struct NPKM *mezcla) {
  self->nombre_cultivo = nombre_cultivo;
  self->sensores = sensores;
  self->mezcla = mezcla;
}

/*  Actualiza todos los sensores de una "Struct farm". */
void sensar(struct farm *self) { self->sensores->actualizar(self->sensores); }

/*  Le envía a la farm una acción a realizar, La farm se encarga de determinar
 * el actuador que debe responder y la forma en que debe hacerlo. Para ello se
 * pasan como parámetros a "actuadores" el tipo de actuador que debería
 * responder y la función que dicho actuador debe realizar, en forma de un
 * puntero a función.(asumiendo que hay 5 definidos, en caso de definir más hay
 * que modificarlo pidiendo el nombre del actuador para identificarlo mejor).
 */
void actuar(struct farm *self, enum tipo_actuador tipo,
            void (*accion_actuador)(struct actuador *actuador)) {
  for (int i = 0; i < self->actuadores->n_actuadores; i++) {

    if (self->actuadores->array_actuadores[i].tipo == tipo &&
        self->actuadores->array_actuadores[i].estado != 0) {

      self->actuadores->array_actuadores[i].actuar = accion_actuador;
      (*self->actuadores->array_actuadores[i].actuar)(
          &self->actuadores->array_actuadores[i]);
    }
  }
}

/*  Función que inicializa una "farm".  */
void init_farm(struct farm *self) {
  self->constructor = construir_farm;
  self->sensar = sensar;
  self->actuar = actuar;
}

/*  Imprime la Información de una "farm" particular (que se pasa como
 * parámetro). */
void print_farm_info(struct farm *self) {
  printf("nombre_cultivo\t:\t%s\n", self->nombre_cultivo);
  printf("niveles optimos de mezcla:\n\tN: %d\n\tP: %d\n\tK: %d\n\tM: %d\n",
         self->mezcla->n, self->mezcla->p, self->mezcla->k, self->mezcla->m);
  printf("N sensores\t:\t%d\n", self->sensores->n_sensores);
  for (int i = 0; i < self->sensores->n_sensores; i++) {
    printf("Sensor %s\t\tpin %d\t\tvalor %d\n",
           (self->sensores->array_sensores)[i].nombre_sensor,
           (self->sensores->array_sensores)[i].pin,
           (self->sensores->array_sensores)[i].valor);
  }
}
