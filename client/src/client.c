#include "client.h"

int main(void) {
  /*---------------------------------------------------PARTE
   * 2-------------------------------------------------------------*/
  int conexion;
  char *ip;
  char *puerto;
  char *valor;

  t_log *logger;
  t_config *config;

  /* ---------------- LOGGING ---------------- */

  logger = iniciar_logger();

  // Usando el logger creado previamente
  // Escribi: "Hola! Soy un log"
  log_info(logger, "Soy un log!");

  /* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

  config = iniciar_config();

  // Usando el config creado previamente, leemos los valores del config y los
  // dejamos en las variables 'ip', 'puerto' y 'valor'
  ip = config_get_string_value(config, "IP");
  puerto = config_get_string_value(config, "PUERTO");
  valor = config_get_string_value(config, "CLAVE");

  // Loggeamos el valor de config
  log_info(logger, ip);
  log_info(logger, puerto);
  log_info(logger, valor);

  /* ---------------- LEER DE CONSOLA ---------------- */

  leer_consola(logger);

  /*---------------------------------------------------PARTE
   * 3-------------------------------------------------------------*/

  // ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor
  // esté corriendo para poder conectarnos a él

  // Creamos una conexión hacia el servidor
  conexion = crear_conexion(ip, puerto);

  // Enviamos al servidor el valor de CLAVE como mensaje
  enviar_mensaje(valor, conexion);

  // Armamos y enviamos el paquete
  paquete(conexion);

  terminar_programa(conexion, logger, config);

  /*---------------------------------------------------PARTE
   * 5-------------------------------------------------------------*/
  // Proximamente
}

t_log *iniciar_logger(void) {
  t_log *nuevo_logger;
  nuevo_logger = log_create("tp0.log", "client", true, LOG_LEVEL_INFO);
  return nuevo_logger;
}

t_config *iniciar_config(void) {
  t_config *nuevo_config;
  nuevo_config = config_create("cliente.config");
  return nuevo_config;
}

void leer_consola(t_log *logger) {
  char *leido;

  leido = readline(">");
  while (!string_is_empty(leido)) {
    log_info(logger, leido);
    free(leido);
    leido = readline(">");
  }

  free(leido);
}

void paquete(int conexion) {
  char *leido;
  t_paquete *paquete = crear_super_paquete();

  printf("Empaquetar:\n");
  leido = readline(">");
  while (!string_is_empty(leido)) {
    agregar_a_paquete(paquete, leido, strlen(leido) + 1);
    free(leido);
    leido = readline(">");
  }
  free(leido);

  enviar_paquete(paquete, conexion);
  eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log *logger, t_config *config) {
  liberar_conexion(conexion);
  log_destroy(logger);
  config_destroy(config);
}
