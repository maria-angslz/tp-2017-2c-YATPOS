#ifndef FILE_H_
#define FILE_H_

#include <stdbool.h>
#include <stddef.h>

/**
 * Devuelve el directorio home del usuario.
 * (En la VM de la cátedra, va a ser siempre /home/utnso)
 * @return Directorio home.
 */
const char *file_homedir(void);

/**
 * Devuelve el directorio base del sistema (/home/utnso/yatpos).
 * @return Directorio base.
 */
const char *file_basedir(void);

/**
 * Devuelve la ruta al directorio de recursos (Shared/rsc).
 * @return Directorio de recursos.
 */
const char *file_rscdir(void);

/**
 * Crea todos los directorios del sistema.
 * Solo debería llamarse por process_init().
 */
void file_create_sysdirs(void);

/**
 * Verifica si un archivo existe en el sistema.
 * @param path Ruta al archivo a verificar.
 * @return Valor lógico indicando si existe el archivo.
 */
bool file_exists(const char *path);

/**
 * Verifica si un archivo es un directorio.
 * @param path Ruta al archivo a verificar.
 * @return Valor lógico indicando si es un directorio.
 */
bool file_isdir(const char *path);

/**
 * Crea un directorio en el sistema.
 * También crea los directorios intermedios, si no existían.
 * @param path Ruta del directorio a crear.
 */
void file_mkdir(const char *path);

/**
 * Devuelve el tamaño de un archivo.
 * @param path Ruta al archivo.
 * @return Tamaño del archivo en bytes.
 */
size_t file_size(const char *path);

/**
 * Devuelve el nombre de un archivo.
 * @param path Ruta al archivo.
 * @return Nombre del archivo.
 */
const char *file_name(const char *path);

/**
 * Copia un archivo de un ruta (source) a otra (target).
 * Si el archivo destino ya existía, lo reemplaza.
 * @param source Ruta del archivo original.
 * @param target Ruta de la copia.
 */
void file_copy(const char *source, const char *target);

#endif /* FILE_H_ */