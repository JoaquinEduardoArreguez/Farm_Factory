#include <stdint.h>
#pragma once

/*  El sistema está pensado para manejar sólo 5 actuadores, sin embargo, la
 * implementación se deja abierta a la posibilidad de agregar hasta 255
 * actuadores; Cada "farm" se manejará con un array de actuadores,
 * del cual se elegirá el actuador correspondiente en tiempo de ejecución a la
 * hora de llevar a cabo alguna función.
 */

/*  Los 5 actuadores básicos, definidos de antemano.

    V_PASO    ->  Válvula de paso del agua mezclada al sector del cultivo.
    V_AGUA    ->  Válvula de acceso del agua corriente.
    V_MEZCLA  ->  Válvula de acceso de la mezcla de nutrientes.
    MOT_CIRC  ->  Motor que hace circular la mezcla en el cultivo.
    MOT_MEZC  ->  Motor que remueve el agua y los nutrientes para mezclarlos.
*/
enum tipo_actuador { V_PASO, V_AGUA, V_MEZCLA, MOT_MEZC, MOT_CIRC };

enum estado_actuador { LIBRE, OCUPADO };

/*  Struct que maneja toda la Información de un actuador*/
struct actuador {
  char *nombre_actuador;
  uint8_t pin, estado;
  enum tipo_actuador tipo;
  void (*constructor)(struct actuador *, enum tipo_actuador, char *, uint8_t,
                      uint8_t, void (*actuar)(struct actuador *));
  void (*actuar)(struct actuador *);
};

/*  Constructor de actuadores, no debe usarse directamente. */
void construir_actuador(struct actuador *self, enum tipo_actuador tipo,
                        char *nombre_actuador, uint8_t pin, uint8_t estado,
                        void (*accion_actuador)(struct actuador *actuar)) {
  self->nombre_actuador = nombre_actuador;
  self->tipo = tipo;
  self->pin = pin;
  self->estado = estado;
  self->actuar = accion_actuador;
}

/*  Función que inicializa actuadores.  */
void init_actuador(struct actuador *self, char *nombre_actuador,
                   enum tipo_actuador tipo, uint8_t pin, uint8_t estado,
                   void (*accion_actuador)(struct actuador *)) {
  self->constructor = construir_actuador;
  (*self->constructor)(self, tipo, nombre_actuador, pin, estado,
                       accion_actuador);
}

/*  Struct que aglomera los actuadores de una "farm". */
struct actuadores {
  int8_t n_actuadores;
  struct actuador *array_actuadores;
};
